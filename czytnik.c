#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>

#include "powszechne.h"
#include "dzielone.h"


/*
Zmienna** zmienne = NULL;               // zmienne
size_t liczbaZmiennych = 0;             // liczba używanych zmiennych
size_t pojemnoscZmiennych = 0;          // przypisane zmiennym miejsce
*/

Zmienna** wWObszaruPowszechnego = NULL;
Zmienna** wWWykonywanegoObszaruGlownego = NULL;
Zmienna** wWWykonywanegoObszaru = NULL;
Ozin* oziny = NULL;

Zmienna** wWWykonywanegoPocztu = NULL;
unsigned char* poczet = NULL;
size_t odnosnikPolecenia = 0;

ZmiennaILiczba* stosWyjsc = NULL;
size_t liczbaStosuWyjsc = 0;
size_t pojemnoscStosuWyjsc = 0;

char** nazwyZmiennych = NULL;
size_t liczbaNazwZmiennych = 0;
size_t pojemnoscNazwZmiennych = 0;

const char hasloCzp[] = { 0x50, 0x49, 0x45, 0x52, 0x57, 0x53, 0x5A, 0x59 }; // "PIERWSZY" zapisane szesnastkowo
FILE* zrodlo;
char* poczatek = NULL;
char* poczatekPola = NULL;
char* obecny = NULL;
char* koniec = NULL;

FILE* pekCzytelnegoPocztu;
unsigned int dlugoscTrzymakaNazwTymczasowych;
char* trzymakNazwTymaczasowych = NULL;

void (*zwolnijPamiecZMpz)(void *, size_t);

char* slowaKluczowe[] = {"###", "jesli", "poki"};
char* nazwyPolecenSlowKluczowych[] = {"###", "jesli_klc", "poki_klc"};
char* dzialania[] = {"==", "!=", ">=", "<=", "||", "&&", "+=", "-=", "*=", "/=", "%=", "><", "<>", "+", "-", "*", "/", "%", "=", "!", ">", "<", "."}; // działania idą w kolejności malejącej długości (najpierw sprawdzamy najdłuższe)
char* nazwyPolecenDzialan[] = {"rowne_dz", "rozne_dz", "wieksze_badz_rowne_dz", "mniejsze_badz_rowne_dz", "lub_dz", "i_dz", "ndodaj_dz", "nodejmij_dz", "nmnoz_dz", "ndziel_dz", "nreszta_dz", "wierzch_dz", "spod_dz", "dodaj_dz", "odejmij_dz", "mnoz_dz", "dziel_dz", "reszta_dz", "nadaj_dz", "nie_dz", "wieksze_dz", "mniejsze_dz", "kropka_dz"};

Polecenie* polecenia = NULL;
char** nazwyPolecen = NULL;
size_t* dlugosciWywodowPolecen = NULL;
size_t liczbaPolecen = 0;
size_t pojemnoscPolecen = 8;

HMODULE* dllDoZwolnienia = NULL;
unsigned int liczbaDllDoZwolnienia = 0;

uintptr_t nastepnaPotegaDwojki(uintptr_t x)
{
    if (x == 0) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
#if UINTPTR_MAX > 0xFFFFFFFF
    x |= x >> 32;
#endif
    return x + 1;
}

void niezbywalnyBlad(const char* wiadomosc)
{
    fprintf(stderr, "\033[1;31m[NIEZBYWALNY BLAD] %s\033[0m\n", wiadomosc);
    /*for(size_t i = 1; i < liczbaZmiennych; i++)
    {
        if(zmienne[i]) free(zmienne[i]);
    }
    if(zmienne) free(zmienne);*/
    for(size_t i = 1; i < liczbaNazwZmiennych; i++)
    {
        if(nazwyZmiennych[i]) free(nazwyZmiennych[i]);
    }
    if(nazwyZmiennych) free(nazwyZmiennych);
    for(int i = 0; i < liczbaDllDoZwolnienia; i++)
    {
        if(dllDoZwolnienia[i]) FreeLibrary(dllDoZwolnienia[i]);
    }
    if(dllDoZwolnienia) free(dllDoZwolnienia);
    if(poczatek) free(poczatek);
    if(polecenia) free(polecenia);
    if(trzymakNazwTymaczasowych) free(trzymakNazwTymaczasowych);
    if(zrodlo) fclose(zrodlo);
    if(pekCzytelnegoPocztu) fclose(pekCzytelnegoPocztu);
    getchar();
    exit(EXIT_FAILURE);
}


Zmienna** utworzZmienna(size_t rozmiar, size_t pojemnosc, size_t rod, void* zawartosc);


void dodajZawartoscDoZmiennej(Zmienna** wWZmiennej, size_t rozmiar, void* zaw)
{
    if(!wWZmiennej) return;
    Zmienna* wZmiennej = *wWZmiennej;
    size_t wymagane = wZmiennej->rozmiar + rozmiar;
    if(wymagane > wZmiennej->pojemnosc)
    {
        size_t pojemnosc = nastepnaPotegaDwojki(wymagane);
        wZmiennej = (Zmienna*)realloc(wZmiennej, sizeof(Zmienna) + pojemnosc + wZmiennej->cechy * sizeof(Ozin));
        if(!wZmiennej) niezbywalnyBlad("Brak pamieci");
        if(wZmiennej->cechy)
        {
            Ozin* starePolozenieCech = cechy(wZmiennej);
            wZmiennej->pojemnosc = pojemnosc;
            memmove(cechy(wZmiennej), starePolozenieCech, wZmiennej->cechy * sizeof(Ozin));
        }
        else wZmiennej->pojemnosc = pojemnosc;
        *wWZmiennej = wZmiennej;
    }
    memcpy(zawartosc(wZmiennej) + wZmiennej->rozmiar, zaw, rozmiar);
    wZmiennej->rozmiar = wymagane;
}

void ustawZawartoscZmiennej(Zmienna** wWZmiennej, size_t rozmiar, void* zaw)
{
    if(!wWZmiennej) return;
    Zmienna* wZmiennej = *wWZmiennej;
    if(rozmiar > wZmiennej->pojemnosc)
    {
        size_t pojemnosc = nastepnaPotegaDwojki(rozmiar);
        wZmiennej = (Zmienna*)realloc(wZmiennej, sizeof(Zmienna) + pojemnosc + wZmiennej->cechy * sizeof(Ozin));
        if(!wZmiennej) niezbywalnyBlad("Brak pamieci");
        if(wZmiennej->cechy)
        {
            Ozin* starePolozenieCech = cechy(wZmiennej);
            wZmiennej->pojemnosc = pojemnosc;
            memmove(cechy(wZmiennej), starePolozenieCech, wZmiennej->cechy * sizeof(Ozin));
        }
        else wZmiennej->pojemnosc = pojemnosc;
        *wWZmiennej = wZmiennej;
    }
    memcpy(zawartosc(wZmiennej), zaw, rozmiar);
    wZmiennej->rozmiar = rozmiar;
}

Zmienna** utworzOdbicieZmiennej(Zmienna** odbijana)
{
    Zmienna* z = *odbijana;
    return utworzZmienna(z->rozmiar, z->pojemnosc, z->rod, zawartosc(z));
    /*if(z->rod == 1)
    {
        size_t r = z->rozmiar / sizeof(Ozin);
        Zmienna** w = utworzZmienna(z->rozmiar, z->pojemnosc, z->rod, zawartosc(z));
        Ozin* ozinyOdbijanej = (Ozin*)zawartosc(z);
        Ozin* ozinyOdbicia = (Ozin*)zawartosc(*w);
        for(size_t i = 1; i < r; i++)
        {
            if(*(nazwyZmiennych[ozinyOdbijanej[i].odnosnikNazwy]) != '*') ozinyOdbicia[i].wWZmiennej = utworzOdbicieZmiennej(ozinyOdbijanej[i].wWZmiennej);
        }
        return w;
    }
    else
    {
        return utworzZmienna(z->rozmiar, z->pojemnosc, z->rod, zawartosc(z));
    }*/
}

/*void odbijZmienna(Zmienna** miejsce, Zmienna** odbijana)
{
    if((*odbijana)->rod == 1)
    {
        size_t r = (*odbijana)->rozmiar / sizeof(Ozin);
        Ozin* ozinyOdbijanej = (Ozin*)zawartosc(*odbijana);
        ustawZawartoscZmiennej(miejsce, sizeof(Ozin), ozinyOdbijanej);
        for(size_t i = 1; i < r; i++)
        {
            Zmienna* z = *(ozinyOdbijanej[i].wWZmiennej);
            size_t w = utworzZmiennaWObszarze(miejsce, ozinyOdbijanej[i].odnosnikNazwy, z->rozmiar, z->pojemnosc, z->rod, zawartosc(z));
            if(z->rod == 1) odbijZmienna(pozyskajZmiennaZObszaru(miejsce, w), ozinyOdbijanej[i].wWZmiennej);
        }
    }
    else
    {
        ustawZawartoscZmiennej(miejsce, (*odbijana)->rozmiar, zawartosc(*odbijana));
        (*miejsce)->rod = (*odbijana)->rod;
    }
}*/


size_t dodajNazweZmiennej(char* nazwa, size_t dlugosc)
{
    if(!nazwa) return 0;
    if(dlugosc == 0) dlugosc = strlen(nazwa);
    if(liczbaNazwZmiennych >= pojemnoscNazwZmiennych)
    {
        pojemnoscNazwZmiennych *= 2;
        nazwyZmiennych = (char**)realloc(nazwyZmiennych, pojemnoscNazwZmiennych * sizeof(char*));
        if(!nazwyZmiennych) niezbywalnyBlad("Brak pamieci");
    }
    char* pamiec = (char*)malloc(dlugosc + 1);
    if(!pamiec) niezbywalnyBlad("Brak pamieci");
    memcpy(pamiec, nazwa, dlugosc);
    pamiec[dlugosc] = '\0';
    nazwyZmiennych[liczbaNazwZmiennych] = pamiec;
    return liczbaNazwZmiennych++;
}

// $ - obszary główne
// ! - zmienne wczytywane
// = - zmienne tymczasowe (do obliczeń)
// @ - poczet obszaru
// * - wartość pozyskiwana
size_t znajdzLubDodajNazweZmiennej(char* nazwa, size_t dlugosc)
{
    if(nazwa == NULL) return 0;
    if(dlugosc == 0) dlugosc = strlen(nazwa);
    if(dlugosc == 0) return 0;
    for(size_t i = 1; i < liczbaNazwZmiennych; i++)
    {
        if(memcmp(nazwa, nazwyZmiennych[i], dlugosc) == 0 && nazwyZmiennych[i][dlugosc] == '\0') return i;
    }
    if(liczbaNazwZmiennych >= pojemnoscNazwZmiennych)
    {
        pojemnoscNazwZmiennych *= 2;
        nazwyZmiennych = (char**)realloc(nazwyZmiennych, pojemnoscNazwZmiennych * sizeof(char*));
        if(nazwyZmiennych == NULL) niezbywalnyBlad("Brak pamieci");
    }
    char* pamiec = (char*)malloc(dlugosc + 1);
    if(!pamiec) niezbywalnyBlad("Brak pamieci");
    strncpy(pamiec, nazwa, dlugosc);
    pamiec[dlugosc] = '\0';
    nazwyZmiennych[liczbaNazwZmiennych] = pamiec;
    return liczbaNazwZmiennych++;
}

Zmienna** utworzZmienna(size_t rozmiar, size_t pojemnosc, size_t rod, void* zawartosc)
{
    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zawartosc, rozmiar);
    Zmienna** wskaznik = (Zmienna**)malloc(sizeof(Zmienna*));
    if(!wskaznik) niezbywalnyBlad("Brak pamieci");
    *wskaznik = zmienna;
    return wskaznik;
}

Zmienna** utworzZmiennaZCechami(size_t rozmiar, size_t pojemnosc, size_t rod, size_t cechy, char* zawartosc, Ozin* zawartoscCech)
{
    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc + cechy * sizeof(Ozin));
    if(zmienna == NULL) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = cechy;
    char* w = (char*)zmienna + sizeof(Zmienna);
    memcpy(w, zawartosc, rozmiar);
    memcpy(w + pojemnosc, zawartoscCech, cechy * sizeof(Ozin));
    Zmienna** wskaznik = (Zmienna**)malloc(sizeof(Zmienna*));
    if(!wskaznik) niezbywalnyBlad("Brak pamieci");
    *wskaznik = zmienna;
    return wskaznik;
}

/*size_t utworzZmiennaWObszarze(Zmienna** wWObszaru, size_t odnosnikNazwy, size_t rozmiar, size_t pojemnosc, size_t rod, void* zaw)
{
    if(!wWObszaru) return 0;
    size_t liczbaPrzeszukiwanychOzinow = (*wWObszaru)->rozmiar / sizeof(Ozin);

    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zaw, rozmiar);
    Zmienna** wskaznik = (Zmienna**)malloc(sizeof(Zmienna*));
    if(!wskaznik) niezbywalnyBlad("Brak pamieci");
    *wskaznik = zmienna;
    Ozin o = {wskaznik, odnosnikNazwy};
    dodajZawartoscDoZmiennej(wWObszaru, sizeof(Ozin), &o);
    return liczbaPrzeszukiwanychOzinow;
}*/

size_t utworzZmiennaWObszarze(Zmienna** wWObszaru, char* nazwa, size_t rozmiar, size_t pojemnosc, size_t rod, void* zaw)
{
    if(!wWObszaru) return 0;
    size_t liczbaPrzeszukiwanychOzinow = (*wWObszaru)->rozmiar / sizeof(Ozin);

    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zaw, rozmiar);
    Zmienna** wskaznik = (Zmienna**)malloc(sizeof(Zmienna*));
    if(!wskaznik) niezbywalnyBlad("Brak pamieci");
    *wskaznik = zmienna;
    Ozin o = {wskaznik, znajdzLubDodajNazweZmiennej(nazwa, 0)};
    dodajZawartoscDoZmiennej(wWObszaru, sizeof(Ozin), &o);
    return liczbaPrzeszukiwanychOzinow;
}

size_t znajdzLubUtworzZmiennaWObszarze(Zmienna** wWObszaru, char* nazwa, size_t rozmiar, size_t pojemnosc, size_t rod, void* zaw)
{
    if(!wWObszaru) return 0;
    size_t liczbaPrzeszukiwanychOzinow = (*wWObszaru)->rozmiar / sizeof(Ozin);
    Ozin* przeszukiwaneOziny = (Ozin*)zawartosc(*wWObszaru);

    for(size_t i = 1; i < liczbaPrzeszukiwanychOzinow; i++)
    {
        if(przeszukiwaneOziny[i].wWZmiennej == NULL) continue;
        Zmienna* z = *(przeszukiwaneOziny[i].wWZmiennej);
        if(z->rod == rod && z->rozmiar == rozmiar && memcmp(zaw, zawartosc(z), rozmiar) == 0) return i;
    }

    if(wWWykonywanegoObszaruGlownego && wWWykonywanegoObszaruGlownego != wWObszaru)
    {
        size_t liczbaPrzeszukiwanychOzinowGlownych = (*wWWykonywanegoObszaruGlownego)->rozmiar / sizeof(Ozin);
        przeszukiwaneOziny = (Ozin *)zawartosc(*wWWykonywanegoObszaruGlownego);
        for(size_t i = 1; i < liczbaPrzeszukiwanychOzinowGlownych; i++)
        {
            Zmienna** wWZ = przeszukiwaneOziny[i].wWZmiennej;
            if(wWZ == NULL) continue;
            Zmienna *z = *(wWZ);
            if(z->rod == rod && z->rozmiar == rozmiar && memcmp(zaw, zawartosc(z), rozmiar) == 0)
            {
                size_t r = strlen(nazwa) + 1;
                char* poszerzona = malloc(r + 1);
                *poszerzona = '*';
                memcpy(poszerzona + 1, nazwa, r);
                Ozin o = {wWZ, znajdzLubDodajNazweZmiennej(poszerzona, r)};
                dodajZawartoscDoZmiennej(wWObszaru, sizeof(Ozin), &o);
                //printf("Znaleziono w glownym %p\n", wWZ);
                return liczbaPrzeszukiwanychOzinow;
            }
        }
    }

    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zaw, rozmiar);
    Zmienna** wskaznik = (Zmienna**)malloc(sizeof(Zmienna*));
    if(!wskaznik) niezbywalnyBlad("Brak pamieci");
    *wskaznik = zmienna;
    Ozin o = {wskaznik, znajdzLubDodajNazweZmiennej(nazwa, 0)};
    dodajZawartoscDoZmiennej(wWObszaru, sizeof(Ozin), &o);
    return liczbaPrzeszukiwanychOzinow;
}

Zmienna** znajdzZmiennaWObszarze(Zmienna** wWObszaru, size_t odnosnikNazwy)
{
    if(!wWObszaru) return 0;
    size_t liczbaPrzeszukiwanychOzinow = (*wWObszaru)->rozmiar / sizeof(Ozin);
    Ozin* przeszukiwaneOziny = (Ozin*)zawartosc(*wWObszaru);

    for(size_t i = 1; i < liczbaPrzeszukiwanychOzinow; i++)
    {
        if(przeszukiwaneOziny[i].odnosnikNazwy == odnosnikNazwy) return przeszukiwaneOziny[i].wWZmiennej;
    }
    return NULL;
}


void wypiszStosWyjsc()
{
    for(size_t i = 0; i < liczbaStosuWyjsc; i++)
    {
        if((*(stosWyjsc[i].wWZmiennej))->rod == 1) printf("\033[1;32mObszar: %p (l: %d)\033[0m\n", stosWyjsc[i].wWZmiennej, stosWyjsc[i].liczba);
        else if((*(stosWyjsc[i].wWZmiennej))->rod == 2) printf("\033[1;32mPoczet: %p (l: %d)\033[0m\n", stosWyjsc[i].wWZmiennej, stosWyjsc[i].liczba);
    }
    printf("\033[1;31mObecny obszar: %p\033[0m\n", wWWykonywanegoObszaru);
    printf("\033[1;31mObecny poczet: %p (l: %zu)\033[0m\n", wWWykonywanegoPocztu, odnosnikPolecenia);
}

void przelaczNaPoczet(Zmienna** wWPocztu)
{
    if(!wWPocztu) return;
    //printf("\033[1;34mPrzelaczam poczet z %p na %p (l: %zu)\033[0m\n", wWWykonywanegoPocztu, wWPocztu, odnosnikPolecenia);
    if(wWWykonywanegoPocztu)
    {
        if(liczbaStosuWyjsc >= pojemnoscStosuWyjsc)
        {
            pojemnoscStosuWyjsc *= 2;
            stosWyjsc = realloc(stosWyjsc, pojemnoscStosuWyjsc * sizeof(ZmiennaILiczba));
            if(!stosWyjsc) niezbywalnyBlad("Brak pamieci");
        }
        stosWyjsc[liczbaStosuWyjsc].wWZmiennej = wWWykonywanegoPocztu;
        stosWyjsc[liczbaStosuWyjsc].liczba = odnosnikPolecenia;
        liczbaStosuWyjsc++;
    }
    wWWykonywanegoPocztu = wWPocztu;
    odnosnikPolecenia = 0;
    poczet = zawartosc(*wWPocztu) + sizeof(Zmienna**); // pomijamy wskaźnik obszaru na początku
    //printf("\033[1;34mLiczba wartosci na stosie wyjsc: %zu\033[0m\n", liczbaStosuWyjsc);
}

void przelaczNaObszar(Zmienna** wWObszaru)
{
    if(!wWObszaru) return;
    Zmienna** wWPocztu = pozyskajZmiennaZObszaru(*wWObszaru, 0);
    przelaczNaPoczet(wWPocztu);
    //printf("\033[1;34mPrzelaczam obszar z %p na %p (l: %d)\033[0m\n", wWWykonywanegoObszaru, wWObszaru, (wWPocztu) ? 1 : 0);
    if(wWWykonywanegoObszaru)
    {
        if(liczbaStosuWyjsc >= pojemnoscStosuWyjsc)
        {
            pojemnoscStosuWyjsc *= 2;
            stosWyjsc = realloc(stosWyjsc, pojemnoscStosuWyjsc * sizeof(ZmiennaILiczba));
            if(!stosWyjsc) niezbywalnyBlad("Brak pamieci");
        }
        stosWyjsc[liczbaStosuWyjsc].wWZmiennej = wWWykonywanegoObszaru;
        stosWyjsc[liczbaStosuWyjsc].liczba = (wWPocztu) ? 1 : 0;
        liczbaStosuWyjsc++;
    }
    wWWykonywanegoObszaru = wWObszaru;
    oziny = (Ozin*)zawartosc(*wWObszaru);
    //printf("\033[1;34mLiczba wartosci na stosie wyjsc: %zu\033[0m\n", liczbaStosuWyjsc);
}

void zejdz()
{
    if(liczbaStosuWyjsc)
    {
        liczbaStosuWyjsc--;
        Zmienna** z = stosWyjsc[liczbaStosuWyjsc].wWZmiennej;
        size_t l = stosWyjsc[liczbaStosuWyjsc].liczba;
        if((*z)->rod == 1)
        {
            //printf("\033[1;34mSchodze na obszar: %p (l: %zu)\033[0m\n", z, l);
            wWWykonywanegoObszaru = z;
            oziny = (Ozin*)(zawartosc(*z));
            if(l) zejdz(); // jeśli zeszliśmy na obszar i ma liczbę to musimy zejść jeszcze niżej by wczytać poczet
        }
        else if((*z)->rod == 2)
        {
            //printf("\033[1;34mSchodze na poczet: %p (l: %zu)\033[0m\n", z, l);
            wWWykonywanegoPocztu = z;
            odnosnikPolecenia = l;
            poczet = zawartosc(*z) + sizeof(Zmienna**); // każdy poczet zaczyna się wskaźnikiem na obszar którego nie chcemy
        }
        //printf("\033[1;34mLiczba wartosci na stosie wyjsc: %zu\033[0m\n", liczbaStosuWyjsc);
    }
    //else printf("\033[1;34mStos wyjsc jest pusty, nie mozna zejsc\033[0m\n");
}


void sciagnijObszary(Zmienna**** wWWWObszaruPowszechnego, Zmienna**** wWWWWykonywanegoObszaruGlownego, Zmienna**** wWWWWykonywanegoObszaru, Ozin*** wWOzinow)
{
    if(wWWWObszaruPowszechnego) *wWWWObszaruPowszechnego = &wWObszaruPowszechnego;
    if(wWWWWykonywanegoObszaruGlownego) *wWWWWykonywanegoObszaruGlownego = &wWWykonywanegoObszaruGlownego;
    if(wWWWWykonywanegoObszaru) *wWWWWykonywanegoObszaru = &wWWykonywanegoObszaru;
    if(wWOzinow) *wWOzinow = &oziny;
}

void sciagnijNazwyZmiennych(char**** wWNazwZmiennych, size_t** wWLiczbyNazwZmiennych, size_t** wWPojemnosciNazwZmiennych)
{
    if(wWNazwZmiennych) *wWNazwZmiennych = &nazwyZmiennych;
    if(wWLiczbyNazwZmiennych) *wWLiczbyNazwZmiennych = &liczbaNazwZmiennych;
    if(wWPojemnosciNazwZmiennych) *wWPojemnosciNazwZmiennych = &pojemnoscNazwZmiennych;
}

void sciagnijPoczet(Zmienna**** wWWWWykonywanegoPocztu, unsigned char*** wWPocztu, size_t** wWOdnosnikaPolecenia)
{
    if(wWWWWykonywanegoPocztu) *wWWWWykonywanegoPocztu = &wWWykonywanegoPocztu;
    if(wWPocztu) *wWPocztu = &poczet;
    if(wWOdnosnikaPolecenia) *wWOdnosnikaPolecenia = &odnosnikPolecenia;
}

void sciagnijPolecenia(Polecenie*** wWPolecen, char**** wWNazwPolecen, size_t*** wWDlugosciWywodowPolecen, size_t** wWLiczbyPolecen, size_t** wWPojemnosciPolecen)
{
    if(wWPolecen) *wWPolecen = &polecenia;
    if(wWNazwPolecen) *wWNazwPolecen = &nazwyPolecen;
    if(wWDlugosciWywodowPolecen) *wWDlugosciWywodowPolecen = &dlugosciWywodowPolecen;
    if(wWLiczbyPolecen) *wWLiczbyPolecen = &liczbaPolecen;
    if(wWPojemnosciPolecen) *wWPojemnosciPolecen = &pojemnoscPolecen;
}

void sciagnijZarzadzanieZmiennymi(void (**wWDodaj)(Zmienna**, size_t, void*), void (**wWUstaw)(Zmienna**, size_t, void*), Zmienna** (**wWOdbij)(Zmienna**))
{
    if(wWDodaj) *wWDodaj = &dodajZawartoscDoZmiennej;
    if(wWUstaw) *wWUstaw = &ustawZawartoscZmiennej;
    if(wWOdbij) *wWOdbij = &utworzOdbicieZmiennej;
}

void sciagnijZarzadzanieStosemWyjsc(void (**wWPrzelaczNaPoczet)(Zmienna**), void (**wWPrzelaczNaObszar)(Zmienna**), void (**wWZejdz)())
{
    if(wWPrzelaczNaPoczet) *wWPrzelaczNaPoczet = &przelaczNaPoczet;
    if(wWPrzelaczNaObszar) *wWPrzelaczNaObszar = &przelaczNaObszar;
    if(wWZejdz) *wWZejdz = &zejdz;
}


size_t walkujGalazPodwojna(GalazPodwojna* galaz, size_t liczbaZmiennychTymczasowych);
void walkujRozgalezienie(Rozgalezienie* rozgalezienie, size_t liczbaZmiennychTymczasowych);

size_t walkujGalazPodwojna(GalazPodwojna* galaz, size_t liczbaZmiennychTymczasowych)
{
    if(!galaz) return 0;
    if(galaz->rodzaj == zmienniana)
    {
        return (size_t)galaz->wartosc;
    }
    if(galaz->rodzaj == wykonaniana)
    {
        //printf("Rozpoczeto wpisywanie wykonania (liczba zmiennych tymczasowych: %zu)\n", liczbaZmiennychTymczasowych);
        snprintf(trzymakNazwTymaczasowych, dlugoscTrzymakaNazwTymczasowych, "=t%zu", liczbaZmiennychTymczasowych);
        size_t nazwa = znajdzLubDodajNazweZmiennej(trzymakNazwTymaczasowych, 0);
        size_t wynik = znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, nazwyZmiennych[nazwa], sizeof(size_t), sizeof(size_t), 0, &nazwa);
        size_t z1 = walkujGalazPodwojna((GalazPodwojna*)(galaz->lewa), liczbaZmiennychTymczasowych + 1);
        //size_t gGWartosci = pozyskajOdnosnikNazwyZObszaru(*pozyskajZmiennaZObszaru(*wWWykonywanegoObszaru, z1), 0);
        Rozgalezienie* rozgalezienie = (Rozgalezienie*)(galaz->prawa);
        size_t r = rozgalezienie->rozmiar;
        //if(gGWartosci < r) niezbywalnyBlad("Przekroczono dozwolona liczbe wartosci przekazywanych wywolania");
        //printf("GG wartosc: %zu, Rozmiar: %zu\n", gGWartosci, r);
        size_t* wartosci = (size_t*)malloc(sizeof(size_t) * r);
        for(size_t i = 0; i < r; i++)
        {
            wartosci[i] = walkujGalazPodwojna(rozgalezienie->galezie[i], liczbaZmiennychTymczasowych + 2 + i);
        }
        unsigned char odnosnikPolecenia = 0; // 0 to nieprawidłowe polecenie (w tablicy ma wartość NULL)
        for(unsigned char j = 1; j < liczbaPolecen; j++)
        {
            if(strcmp(nazwyPolecen[j], "wywolaj_dz") == 0)
            {
                odnosnikPolecenia = j;
                break;
            }
        }
        if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia obslugujacego dzialanie");
        size_t calkowita = (r + 2) * sizeof(size_t); // całkowity rozmiar to wynik + wywoływana + wartości
        dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(odnosnikPolecenia), &odnosnikPolecenia);
        dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(calkowita), &calkowita);
        dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(wynik), &wynik);
        dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(z1), &z1);
        fprintf(pekCzytelnegoPocztu, "%s %zu %zu %zu", nazwyPolecen[odnosnikPolecenia], calkowita, wynik, z1);
        for(size_t i = 0; i < r; i++)
        {
            dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(wartosci[i]), &(wartosci[i]));
            fprintf(pekCzytelnegoPocztu, " %zu", wartosci[i]);
        }
        fprintf(pekCzytelnegoPocztu, "\n");
        //printf("Wpisano wykonanie %s\n", nazwyPolecen[odnosnikPolecenia]);
        return wynik;
    }
    if(galaz->rodzaj == poleceniana)
    {
        char* zawartosc = (char*)galaz->wartosc;
        size_t dlugosc = dlugosciWywodowPolecen[*zawartosc];
        dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, 1 + dlugosc, zawartosc);
        fprintf(pekCzytelnegoPocztu, "%s", nazwyPolecen[*zawartosc]);
        for(size_t i = 0; i < dlugosc; i += 8)
        {
            fprintf(pekCzytelnegoPocztu, " %zu", *((size_t*)(zawartosc + i + 1)));
        }
        fprintf(pekCzytelnegoPocztu, "\n");
        free(zawartosc);
        return 0;
    }
    for(size_t i = 1; i < liczbaSlowKluczowych; i++) // pierwsze słowo kluczowe pomijamy bo jest ono przetwarzane nieco inaczej
    {
        if(kluczowe[i] == galaz->rodzaj)
        {
            //printf("Rozpoczeto wpisywanie slowa kluczowego: %s (liczba zmiennych tymczasowych: %zu)\n", nazwyPolecenSlowKluczowych[i], liczbaZmiennychTymczasowych);
            //getchar();
            if(!galaz->lewa || !galaz->prawa) niezbywalnyBlad("Blad wczytywania slowa kluczowego (brak galezi)");
            size_t poczatekPocztuSlowaKluczowego = (*wWWykonywanegoPocztu)->rozmiar - sizeof(Zmienna**); // zapisujemy gdzie zaczynamy bo wałkowanie lewej gałęzi może i najpewniej go przesunie

            size_t z1 = walkujGalazPodwojna((GalazPodwojna*)(galaz->lewa), liczbaZmiennychTymczasowych);

            unsigned char odnosnikPolecenia = 0; // 0 to nieprawidłowe polecenie (w tablicy ma wartość NULL)
            for(unsigned char j = 1; j < liczbaPolecen; j++)
            {
                if(strcmp(nazwyPolecen[j], nazwyPolecenSlowKluczowych[i]) == 0)
                {
                    odnosnikPolecenia = j;
                    break;
                }
            }
            if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia obslugujacego slowo kluczowe");
            dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(odnosnikPolecenia), &odnosnikPolecenia);
            dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(z1), &z1);
            fprintf(pekCzytelnegoPocztu, "%s %zu\n", nazwyPolecenSlowKluczowych[i], z1);
            size_t miejsceSkoku = (*wWWykonywanegoPocztu)->rozmiar; // zapisujemy gdzie wpiszemy wartość skoku kiedy ją znajdziemy
            (*wWWykonywanegoPocztu)->rozmiar += sizeof(size_t); // a na razie pomijamy tą wartość
            
            walkujRozgalezienie((Rozgalezienie*)(galaz->prawa), liczbaZmiennychTymczasowych);

            //printf("Wpisano slowo kluczowe %s (%s)\n", nazwyPolecen[odnosnikPolecenia], nazwyPolecenSlowKluczowych[i]);
            if(i == 2) // jeśli to polecenie 'póki' musi mieć skok
            {
                // teraz szukamy polecenia skoku i dorzucamy je do pocztu
                odnosnikPolecenia = 0;
                for(unsigned char j = 1; j < liczbaPolecen; j++)
                {
                    if(strcmp(nazwyPolecen[j], "skok_wjk") == 0)
                    {
                        odnosnikPolecenia = j;
                        break;
                    }
                }
                if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia obslugujacego wyjatkowe");
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(odnosnikPolecenia), &odnosnikPolecenia);
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(poczatekPocztuSlowaKluczowego), &poczatekPocztuSlowaKluczowego);
                fprintf(pekCzytelnegoPocztu, "skok_wjk %zu\n", poczatekPocztuSlowaKluczowego);
                //printf("\nWpisano wyjatkowe %s (%s)\n", nazwyPolecen[odnosnikPolecenia], nazwyPolecenSlowKluczowych[i]);
            }
            // teraz wiedząc gdzie jest koniec wpisujemy wartość skoku
            size_t wartoscSkoku = (*wWWykonywanegoPocztu)->rozmiar - sizeof(size_t);
            memcpy(zawartosc(*wWWykonywanegoPocztu) + miejsceSkoku, &wartoscSkoku, sizeof(wartoscSkoku));
            //getchar();
            return 0;
        }
    }
    for(size_t i = 0; i < liczbaDzialan; i++)
    {
        if(dzialaniowe[i] == galaz->rodzaj)
        {
            //printf("Rozpoczeto wpisywanie dzialania: %s (liczba zmiennych tymczasowych: %zu)\n", nazwyPolecenDzialan[i], liczbaZmiennychTymczasowych);
            //getchar();
            snprintf(trzymakNazwTymaczasowych, dlugoscTrzymakaNazwTymczasowych, "=t%zu", liczbaZmiennychTymczasowych);
            size_t nazwa = znajdzLubDodajNazweZmiennej(trzymakNazwTymaczasowych, 0);
            size_t wynik = znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, nazwyZmiennych[nazwa], sizeof(size_t), sizeof(size_t), 0, &nazwa);
            size_t z1 = walkujGalazPodwojna((GalazPodwojna*)(galaz->lewa), liczbaZmiennychTymczasowych + 1);
            size_t z2 = walkujGalazPodwojna((GalazPodwojna*)(galaz->prawa), liczbaZmiennychTymczasowych + 2);

            unsigned char odnosnikPolecenia = 0; // 0 to nieprawidłowe polecenie (w tablicy ma wartość NULL)
            for(unsigned char j = 1; j < liczbaPolecen; j++)
            {
                if(strcmp(nazwyPolecen[j], nazwyPolecenDzialan[i]) == 0)
                {
                    odnosnikPolecenia = j;
                    break;
                }
            }
            if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia obslugujacego dzialanie");
            dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(odnosnikPolecenia), &odnosnikPolecenia);
            dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(wynik), &wynik);
            fprintf(pekCzytelnegoPocztu, "%s %zu", nazwyPolecenDzialan[i], wynik); // wpisujemy nazwę polecenia i odnośnik zmiennej wynikowej
            if(galaz->lewa) // jeśli mamy lewą stronę dopisujemy ją
            {
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(z1), &z1);
                fprintf(pekCzytelnegoPocztu, " %zu", z1);
            }
            if(galaz->prawa) // jeśli mamy prawą stronę dopisujemy ją
            {
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(z2), &z2);
                fprintf(pekCzytelnegoPocztu, " %zu", z2);
            }
            fprintf(pekCzytelnegoPocztu, "\n");
            //printf("Wpisano dzialanie %s (%s)\n", nazwyPolecen[odnosnikPolecenia], nazwyPolecenDzialan[i]);
            //getchar();
            return wynik;
        }
    }
    niezbywalnyBlad("Nie rozpoznano galezi");
    return 0;
}

void walkujRozgalezienie(Rozgalezienie* rozgalezienie, size_t liczbaZmiennychTymczasowych)
{
    if(!rozgalezienie) return;
    for(size_t i = 0; i < rozgalezienie->rozmiar; i++)
    {
        walkujGalazPodwojna(rozgalezienie->galezie[i], liczbaZmiennychTymczasowych);
    }
}


Czastka wczytujNastepne(char** wskaznik);

void pocztuj(char** wskaznik, boolean czyGlowny)
{
    char* polozenie = *wskaznik;
    //printf("Zaladowany tekst:\n%s\n", polozenie);

    Czastka* czastkiObszaru = (Czastka*)malloc(sizeof(Czastka));
    size_t pojemnoscCzastekObszaru = 1;
    size_t rozmiarCzastekObszaru = 0;

    int otwarteSpiecia = 0;

    while(TRUE)
    {
        while(polozenie < koniec && isspace((unsigned char)(*polozenie))) polozenie++;
        if(polozenie >= koniec)
        {
            *wskaznik = polozenie;
            break;
        }
        Czastka c = wczytujNastepne(&polozenie);
        if(c.rodzaj == 0)
        {
            if(*((char*)c.zawartosc) == '{') otwarteSpiecia++;
            else if(*((char*)c.zawartosc) == '}')
            {
                otwarteSpiecia--;
                if(otwarteSpiecia < 0)
                {
                    if(czyGlowny) niezbywalnyBlad("Proba zamkniecia nieistniejacego spiecia");
                    else break; // jeśli zamkniemy więcej spięć niż jest to kończymy
                }
            }
        }
        else if(c.rodzaj == 1 && (char*)c.zawartosc == slowaKluczowe[0])
        {
            if(czyGlowny) break;
            else niezbywalnyBlad("Nie zamknieto spiecia"); // jeśli napotkamy ### to błąd
        }

        if(rozmiarCzastekObszaru >= pojemnoscCzastekObszaru)
        {
            pojemnoscCzastekObszaru *= 2;
            czastkiObszaru = realloc(czastkiObszaru, sizeof(Czastka) * pojemnoscCzastekObszaru);
            if(czastkiObszaru == NULL) niezbywalnyBlad("Brak pamieci");
        }
        czastkiObszaru[rozmiarCzastekObszaru++] = c;
    }

    /*printf("Wczytano czastki:\n");
    for(size_t i = 0; i < rozmiarCzastekObszaru; i++)
    {
        if(czastkiObszaru[i].rodzaj == 2) printf("Czastka %zu: %hhu - %zu\n", i, czastkiObszaru[i].rodzaj, (size_t)czastkiObszaru[i].zawartosc);
        else if(czastkiObszaru[i].rodzaj == 0) printf("Czastka %zu: %hhu - %c\n", i, czastkiObszaru[i].rodzaj, *((char*)czastkiObszaru[i].zawartosc));
        else if(czastkiObszaru[i].rodzaj == 4) printf("Czastka %zu: %hhu - %s\n", i, czastkiObszaru[i].rodzaj, nazwyPolecen[*((char*)czastkiObszaru[i].zawartosc)]);
        else printf("Czastka %zu: %hhu - %s\n", i, czastkiObszaru[i].rodzaj, (char*)czastkiObszaru[i].zawartosc);
    }

    if(czyGlowny) printf("Reszta:\n%s\n", polozenie);*/

    //getchar();

    //printf("Rozpoczeto robienie drzewa\n");
    Rozgalezienie* drzewo = robDrzewo(czastkiObszaru, rozmiarCzastekObszaru, poczatekPola, koniec);
    //printf("Drzewo:\n");
    //wypiszDrzewo(drzewo, 0);
    //getchar();

    pekCzytelnegoPocztu = fopen("poczet.txt", "w+");
    if(pekCzytelnegoPocztu == NULL)
    {
        perror("fopen (poczet.txt)");
        niezbywalnyBlad("Nie mozna otworzyc peku pocztu (w)");
    }

    walkujRozgalezienie(drzewo, 1);

    fclose(pekCzytelnegoPocztu);
    pekCzytelnegoPocztu = NULL;
    //printf("Zakonczono walkowanie\n");
    //getchar();

    free(czastkiObszaru);
    zetnijRozgalezienie(drzewo);
    *wskaznik = polozenie;
}


// zaklepuje pamięć i tworzy podstawowe zmienne rodowe
void przygotowaniePodstawowe()
{
    const int liczbaZmiennychPodstawowych = 6;

    size_t p = nastepnaPotegaDwojki(liczbaZmiennychPodstawowych + 1);

    Ozin pustyOzin = {NULL, 0};
    wWObszaruPowszechnego = utworzZmienna(sizeof(pustyOzin), nastepnaPotegaDwojki(p * sizeof(Ozin)), 1, &pustyOzin);

    pojemnoscNazwZmiennych = p;
    nazwyZmiennych = (char**)malloc(pojemnoscNazwZmiennych * sizeof(char*));
    nazwyZmiennych[0] = NULL;
    liczbaNazwZmiennych = 1;

    // Tworzenie zmiennej obszaru
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "obs", 0, 1, 1, NULL);

    // Tworzenie zmiennej pocztu
    Zmienna** pustyPoczet = NULL;
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "pct", sizeof(Zmienna**), sizeof(Zmienna**), 2, &pustyPoczet);

    // Tworzenie zmiennej liczby
    unsigned char pustaLiczba = 0;
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "lic", 1, 1, 3, &pustaLiczba);

    // Tworzenie zmiennej pisma
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "pis", 0, 1, 4, NULL);

    // Tworzenie zmiennej tak-lub-nie (boolean)
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "tln", 1, 1, 5, &pustaLiczba);
}

void przygotowanieDlaCzp()
{
    /*const int r = sizeof(hasloCzp)/sizeof(char);
    unsigned char bajty[r];
    fread(bajty, 1, r, zrodlo);
    if(memcmp(bajty, hasloCzp, r) != 0)
    {
        niezbywalnyBlad("Pek .czp uszkodzony lub niepoprawny");
    }
    przygotowaniePodstawowe();
    uintptr_t rozmiar = 2 * sizeof(uintptr_t) + 1;
    uintptr_t pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* obszarDomyslny = malloc(sizeof(Zmienna) + pojemnosc);
    if(obszarDomyslny == NULL) niezbywalnyBlad("Brak pamieci");
    obszarDomyslny->cechy = 0;
    obszarDomyslny->rod = 1;
    obszarDomyslny->rozmiar = rozmiar;
    obszarDomyslny->pojemnosc = pojemnosc;
    uintptr_t* w = (uintptr_t*)((char*)obszarDomyslny + sizeof(Zmienna));
    *w = 0;
    w = (uintptr_t*)((char*)w + sizeof(uintptr_t));
    *w = 1;
    *((char*)w + sizeof(uintptr_t)) = 2;
    zmienne[liczbaZmiennych] = obszarDomyslny;*/
}

size_t przygotowanieDlaCz()
{
    przygotowaniePodstawowe();
    char zaw[sizeof(Zmienna**) + 2] = {0};
    zaw[sizeof(Zmienna**)] = 2;
    zaw[sizeof(Zmienna**) + 1] = 3;
    return utworzZmiennaWObszarze(wWObszaruPowszechnego, "$domyslny", sizeof(zaw), 0, 2, &zaw);
}


size_t wczytujJakoNazwa(char** wskaznik);
size_t wczytujZmienne(char** wskaznik);

size_t wczytujJakoObszarGlowny(char** wskaznik)
{
    //printf("Rozpoczeto wczytywanie obszaru glownego\n");

    Zmienna** wWPocztuGlownego = utworzZmienna(0, sizeof(Zmienna**), 2, NULL);
    Ozin pierwszyOzin = {wWPocztuGlownego, 0};
    size_t odnosnikWWObszaruGlownego = utworzZmiennaWObszarze(wWObszaruPowszechnego, "$glowny", sizeof(pierwszyOzin), sizeof(pierwszyOzin) * 2, 1, &pierwszyOzin);
    Zmienna** wWObszaruGlownego = pozyskajZmiennaZObszaru(*wWObszaruPowszechnego, odnosnikWWObszaruGlownego);
    dodajZawartoscDoZmiennej(wWPocztuGlownego, sizeof(wWObszaruGlownego), &wWObszaruGlownego);
    //wypiszStosWyjsc();
    Zmienna** staryObszarGlowny = wWWykonywanegoObszaruGlownego;
    wWWykonywanegoObszaruGlownego = wWObszaruGlownego;
    przelaczNaObszar(wWObszaruGlownego);
    //wypiszStosWyjsc();

    pocztuj(wskaznik, TRUE);

    unsigned char konczace = 4;
    dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(konczace), &konczace);
    //fprintf(pekCzytelnegoPocztu, "%s\n", nazwyPolecen[konczace]);
    //wypiszStosWyjsc();
    zejdz();
    wWWykonywanegoObszaruGlownego = staryObszarGlowny;
    return odnosnikWWObszaruGlownego;
}

size_t wczytujJakoObszar(char** wskaznik) // nie pojawi się w pierwszym teście
{
    char* polozenie = *wskaznik;

    // Sprawdzanie czy zaczyna się na "obszar"
    if(6 > koniec - polozenie) return 0;
    if(memcmp("obszar", polozenie, 6) != 0) return 0;
    if(isalpha((unsigned char)polozenie[6])) return 0;
    polozenie += 6;

    while(polozenie < koniec && isspace((unsigned char)(*polozenie))) polozenie++;
    if(*polozenie != '(') return 0;
    polozenie++;

    Ozin pierwszyOzin = {NULL, 0};
    size_t odnosnikWWObszaru = utworzZmiennaWObszarze(wWWykonywanegoObszaru, "!obs", sizeof(pierwszyOzin), sizeof(pierwszyOzin), 1, &pierwszyOzin);
    Zmienna** wWObszaru = pozyskajZmiennaZObszaru(*wWWykonywanegoObszaru, odnosnikWWObszaru);
    size_t liczba = 0;

    przelaczNaObszar(wWObszaru);

    while(TRUE)
    {
        while(polozenie < koniec && isspace((unsigned char)(*polozenie))) polozenie++;
        if(*polozenie == ')')
        {
            polozenie++;
            break;
        }
        while(polozenie < koniec && isspace((unsigned char)(*polozenie))) polozenie++;
        size_t odnosnikNazwy = wczytujJakoNazwa(&polozenie);
        if(!odnosnikNazwy) niezbywalnyBlad("Blad skladni: nieprawidlowe przekazanie nazw wartosci przenoszonych obszaru");
        liczba++;
        while(polozenie < koniec && isspace((unsigned char)(*polozenie))) polozenie++;
        if(*polozenie == ')')
        {
            polozenie++;
            break;
        }
        else if(*polozenie == ',') polozenie++;
        else niezbywalnyBlad("Blad skladni: nieprawidlowe przekazanie nazw wartosci przenoszonych obszaru");
    }

    ((Ozin*)(zawartosc(*wWObszaru)))->odnosnikNazwy = liczba;
    //printf("LICZBA ZMIENNYCH: %zu\n", ((Ozin*)zawartosc(*wWObszaru))->odnosnikNazwy);

    while(polozenie < koniec && isspace((unsigned char)(*polozenie))) polozenie++;

    if(*polozenie == '{')
    {
        polozenie++;
        Zmienna** wWPocztu = utworzZmienna(0, sizeof(Zmienna**), 2, NULL);
        dodajZawartoscDoZmiennej(wWPocztu, sizeof(wWObszaru), &wWObszaru);
        przelaczNaPoczet(wWPocztu);
        pocztuj(&polozenie, FALSE);
        //wypiszStosWyjsc();
        unsigned char odnosnikPolecenia = 0; // 0 to nieprawidłowe polecenie (w tablicy ma wartość NULL)
        for(unsigned char j = 1; j < liczbaPolecen; j++)
        {
            if(strcmp(nazwyPolecen[j], "opusc_wjk") == 0)
            {
                odnosnikPolecenia = j;
                break;
            }
        }
        if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia");
        dodajZawartoscDoZmiennej(wWPocztu, sizeof(odnosnikPolecenia), &odnosnikPolecenia);

        zejdz();
        ((Ozin*)zawartosc(*wWObszaru))->wWZmiennej = wWPocztu;
    }

    zejdz();
    *wskaznik = polozenie;
    return odnosnikWWObszaru;
}

size_t wczytujJakoLiczbe(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(!isdigit((unsigned char)(*polozenie))) return 0;
    char* pierwotne = polozenie;
    while(polozenie < koniec && isdigit((unsigned char)(*polozenie))) polozenie++;

    size_t dlugosc = polozenie - pierwotne;
    //printf("Wczytujemy liczbe: ");
    //fwrite(pierwotne, 1, dlugosc, stdout);
    //printf("\n");

    char* liczba = malloc(dlugosc + 1);
    if(!liczba) niezbywalnyBlad("Brak pamieci");

    memcpy(liczba, pierwotne, dlugosc);
    liczba[dlugosc] = '\0';
    //printf("Zapisano liczbe: %s. Zaraz wszystko sie posypie X~X\n", liczba);

    mpz_t liczba_mpz;
    mpz_init(liczba_mpz);
    //printf("Jeszcze zyjemy!\n");

    if(mpz_set_str(liczba_mpz, liczba, 10) != 0)
    {
        mpz_clear(liczba_mpz);
        free(liczba);
        niezbywalnyBlad("Przeksztalcanie liczby nie powiodlo sie");
    }
    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, liczba_mpz);
    if(!surowa && mpz_cmp_ui(liczba_mpz, 0) != 0)
    {
        mpz_clear(liczba_mpz);
        free(liczba);
        niezbywalnyBlad("Blad eksportu liczby");
    }
    //size_t wynik = utworzZmienna(rozmiar, 1, 2, surowa);
    size_t wynik = znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, "!lic", rozmiar, 1, 3, surowa);
    //printf("Utworzono zmienna na miejscu %zu\n", wynik);
    //printf("Przed mpz_clear\n");
    mpz_clear(liczba_mpz);
    //printf("Przed free(liczba)\n");
    free(liczba);
    zwolnijPamiecZMpz(surowa, rozmiar);
    //printf("Przed przesunieciem wskaznika\n");
    *wskaznik += dlugosc;
    //printf("Liczba wczytana! ^-^\n");
    return wynik;
}

size_t wczytujJakoPismo(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(*polozenie != '"') return 0;
    char* pierwotne = ++polozenie;
    while(*polozenie != '"') polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    //size_t wynik = utworzZmienna(dlugosc, 1, 3, pierwotne);
    size_t wynik = znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, "!pis", dlugosc, 1, 4, pierwotne);
    *wskaznik += dlugosc + 2;
    return wynik;
}

size_t wczytujJakoTln(char** wskaznik)
{
    char* polozenie = *wskaznik;
    size_t pozostale = koniec - polozenie;
    if(pozostale < 3) return 0;
    if(memcmp(polozenie, "nie", 3) == 0 && (pozostale == 3 || !isalpha(polozenie[3])))
    {
        *wskaznik += 3;
        unsigned char wartosc = 0;
        //return utworzZmienna(sizeof(wartosc), 1, 4, &wartosc);
        return znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, "!tln", sizeof(wartosc), 1, 5, &wartosc);
    }
    if(memcmp(polozenie, "tak", 3) == 0 && (pozostale == 3 || !isalpha(polozenie[3])))
    {
        *wskaznik += 3;
        unsigned char wartosc = 1;
        //return utworzZmienna(sizeof(wartosc), 1, 4, &wartosc);
        return znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, "!tln", sizeof(wartosc), 1, 5, &wartosc);
    }
    return 0;
}

size_t wczytujJakoNazwa(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(!isalpha((unsigned char)(*polozenie)) && *polozenie != '_') return 0;
    char* pierwotne = polozenie;
    while(isalpha(*polozenie) || *polozenie == '_' || isdigit(*polozenie)) polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    size_t nazwa = znajdzLubDodajNazweZmiennej(pierwotne, dlugosc);
    *wskaznik += dlugosc;
    //return utworzZmienna(sizeof(size_t), 0, 0, &(nazwyZmiennych[nazwa])); // utworzenie zmiennej odnosnik do nazwy (a nie nazwę)
    return znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, nazwyZmiennych[nazwa], sizeof(size_t), 0, 0, &nazwa); // utworzenie zmiennej odnosnik do nazwy (a nie nazwę)
}

char* wczytujPolecenie(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(*polozenie != '$') return NULL;
    polozenie++;
    while(polozenie < koniec && isspace(*polozenie)) polozenie++;
    if(!isalpha(*polozenie) && *polozenie != '_') return NULL;
    char* poczatekNazwyPolecenia = polozenie;
    while(isalpha(*polozenie) || *polozenie == '_' || isdigit(*polozenie)) polozenie++;
    size_t dlugosc = polozenie - poczatekNazwyPolecenia;
    unsigned char odnosnikPolecenia = 0; // 0 to nieprawidłowe polecenie (w tablicy ma wartość NULL)
    for(unsigned char j = 1; j < liczbaPolecen; j++)
    {
        if(strncmp(nazwyPolecen[j], poczatekNazwyPolecenia, dlugosc) == 0)
        {
            odnosnikPolecenia = j;
            break;
        }
    }
    if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia");
    size_t pozostalaDlugoscWywodu = dlugosciWywodowPolecen[odnosnikPolecenia];
    size_t polozenieWywodu = sizeof(odnosnikPolecenia);
    char* pamiec;
    if(pozostalaDlugoscWywodu == SIZE_MAX)
    {
        if(*polozenie == '$')
        {
            polozenie++;
            while(polozenie < koniec && isspace(*polozenie)) polozenie++;
            if(!isdigit(*polozenie)) niezbywalnyBlad("Nieprawidlowa wartosc w wywodzie polecenia");
            char* poczatekLiczby = polozenie;
            while(polozenie < koniec && isdigit(*polozenie)) polozenie++;
            errno = 0;
            pozostalaDlugoscWywodu = strtoull(poczatekLiczby, &polozenie, 10);
            if(polozenie == poczatekLiczby || errno == ERANGE || pozostalaDlugoscWywodu > SIZE_MAX) niezbywalnyBlad("Blad wczytywania wartosci wywodu polecenia");
        }
        else niezbywalnyBlad("Polecenie wymaga podania surowej dlugosci");
        pamiec = malloc(polozenieWywodu + sizeof(pozostalaDlugoscWywodu) + pozostalaDlugoscWywodu);
        if(!pamiec) niezbywalnyBlad("Brak pamieci");
        memcpy(pamiec + polozenieWywodu, &pozostalaDlugoscWywodu, sizeof(pozostalaDlugoscWywodu));
        polozenieWywodu += sizeof(pozostalaDlugoscWywodu);
    }
    else
    {
        pamiec = malloc(polozenieWywodu + pozostalaDlugoscWywodu);
        if(!pamiec) niezbywalnyBlad("Brak pamieci");
    }
    pamiec[0] = odnosnikPolecenia;
    while(pozostalaDlugoscWywodu)
    {
        while(polozenie < koniec && isspace(*polozenie)) polozenie++;
        if(pozostalaDlugoscWywodu >= sizeof(size_t))
        {
            size_t zmienna;
            if(*polozenie == '$')
            {
                polozenie++;
                while(polozenie < koniec && isspace(*polozenie)) polozenie++;
                if(!isdigit(*polozenie)) niezbywalnyBlad("Nieprawidlowa wartosc w wywodzie polecenia");
                char* poczatekLiczby = polozenie;
                while(polozenie < koniec && isdigit(*polozenie)) polozenie++;
                errno = 0;
                zmienna = strtoull(poczatekLiczby, &polozenie, 10);
                if(polozenie == poczatekLiczby || errno == ERANGE || zmienna > SIZE_MAX) niezbywalnyBlad("Blad wczytywania wartosci wywodu polecenia");
                memcpy(pamiec + polozenieWywodu, &zmienna, sizeof(zmienna));
                polozenieWywodu += sizeof(zmienna);
                pozostalaDlugoscWywodu -= sizeof(zmienna);
                continue;
            }
            zmienna = wczytujZmienne(&polozenie);
            if(zmienna)
            {
                memcpy(pamiec + polozenieWywodu, &zmienna, sizeof(zmienna));
                polozenieWywodu += sizeof(zmienna);
                pozostalaDlugoscWywodu -= sizeof(zmienna);
                continue;
            }
        }
        else niezbywalnyBlad("Nieprawidlowy wywod polecenia");
    }
    *wskaznik = polozenie;
    return pamiec;
}

char* wczytujKluczowe(char** wskaznik)
{
    char* polozenie = *wskaznik;
    for(int i = 0; i < sizeof(slowaKluczowe)/sizeof(slowaKluczowe[0]); i++)
    {
        size_t dlugosc = strlen(slowaKluczowe[i]);
        if(dlugosc > koniec - polozenie) continue;
        if(memcmp(slowaKluczowe[i], polozenie, dlugosc) != 0) continue;
        if(isalpha((unsigned char)polozenie[dlugosc])) continue;
        *wskaznik += dlugosc;
        return slowaKluczowe[i];
    }
    return NULL;
}

char* wczytujDzialania(char** wskaznik)
{
    char* polozenie = *wskaznik;
    for(int i = 0; i < sizeof(dzialania)/sizeof(dzialania[0]); i++)
    {
        size_t dlugosc = strlen(dzialania[i]);
        if(dlugosc > koniec - polozenie) continue;
        if(memcmp(dzialania[i], polozenie, dlugosc) != 0) continue;
        *wskaznik += dlugosc;
        return dzialania[i];
    }
    return NULL;
}

size_t (*konajkiDoZmiennych[])(char**) = {wczytujJakoObszar, wczytujJakoLiczbe, wczytujJakoPismo, wczytujJakoTln, wczytujJakoNazwa};

size_t wczytujZmienne(char** wskaznik)
{
    size_t wynik;
    for(int i = 0; i < sizeof(konajkiDoZmiennych) / sizeof(konajkiDoZmiennych[0]); i++)
    {
        wynik = konajkiDoZmiennych[i](wskaznik);
        if(wynik) return wynik;
    }
    return 0;
}

Czastka wczytujNastepne(char** wskaznik)
{
    uintptr_t wynik;
    char* polozenie = *wskaznik;
    wynik = (uintptr_t)wczytujPolecenie(wskaznik);
    if(wynik)
    {
        Czastka c = {4, wynik, polozenie};
        //printf("Wczytano polecenie: %s\n", nazwyPolecen[*((char*)wynik)]);
        return c;
    }
    wynik = (uintptr_t)wczytujKluczowe(wskaznik);
    if(wynik)
    {
        Czastka c = {1, wynik, polozenie};
        //printf("Wczytano kluczowe: %s\n", (char*)wynik);
        return c;
    }
    wynik = (uintptr_t)wczytujDzialania(wskaznik);
    if(wynik)
    {
        Czastka c = {3, wynik, polozenie};
        //printf("Wczytano dzialanie: %s\n", (char*)wynik);
        return c;
    }
    wynik = wczytujZmienne(wskaznik);
    if(wynik)
    {
        Czastka c = {2, wynik, polozenie};
        //printf("Wczytano zmienna: %zu (nazwa: %s)\n", (size_t)wynik, nazwyZmiennych[pozyskajOdnosnikNazwyZObszaru(*wWWykonywanegoObszaru, (size_t)wynik)]);
        return c;
    }
    //printf("Wczytano nieznany: %c\n", **wskaznik);
    Czastka c = {0, (uintptr_t)((*wskaznik)++), polozenie};
    return c;
}


void wzywanie(char* nazwa)
{
    HMODULE uchwytDll = LoadLibrary(nazwa);
    if(!uchwytDll) niezbywalnyBlad("Nie udalo sie wczytac uchwytu");
    //printf("Pomyslnie pozyskano uchwyt\n");

    //char*** wskaznikNazwPrzenoszonychPolecen = (char***)GetProcAddress(uchwytDll, "nazwyPrzenoszonychPolecen");
    char** nazwyPrzenoszonychPolecen = (char**)GetProcAddress(uchwytDll, "nazwyPrzenoszonychPolecen");
    size_t* dlugosciWywodowPrzenoszonychPolecen = (size_t*)GetProcAddress(uchwytDll, "dlugosciWywodowPrzenoszonychPolecen");
    size_t* wskaznikLiczbyPrzenoszonychPolecen = (size_t*)GetProcAddress(uchwytDll, "liczbaPrzenoszonychPolecen");

    if(nazwyPrzenoszonychPolecen && dlugosciWywodowPrzenoszonychPolecen && wskaznikLiczbyPrzenoszonychPolecen) // jeśli zosatły wczytane przenosimy polecenia (ich obecność nie jest obowiązkowa)
    {
        //printf("Pomyslnie pozyskano nazwy, rozpoczynam wczytywanie\n");
        //char** nazwyPrzenoszonychPolecen = *wskaznikNazwPrzenoszonychPolecen;
        size_t liczbaPrzenoszonychPolecen = *wskaznikLiczbyPrzenoszonychPolecen;

        for(size_t i = 0; i < liczbaPrzenoszonychPolecen; i++)
        {
            Polecenie p = (Polecenie)GetProcAddress(uchwytDll, nazwyPrzenoszonychPolecen[i]);
            if(p)
            {
                if(liczbaPolecen >= pojemnoscPolecen)
                {
                    pojemnoscPolecen *= 2;
                    polecenia = (Polecenie*)realloc(polecenia, sizeof(Polecenie) * pojemnoscPolecen);
                    nazwyPolecen = (char**)realloc(nazwyPolecen, sizeof(char*) * pojemnoscPolecen);
                    dlugosciWywodowPolecen = (size_t*)realloc(dlugosciWywodowPolecen, sizeof(size_t) * pojemnoscPolecen);
                    if(!polecenia || !nazwyPolecen || !dlugosciWywodowPolecen) niezbywalnyBlad("Brak pamieci");
                }
                polecenia[liczbaPolecen] = p;
                nazwyPolecen[liczbaPolecen] = nazwyPrzenoszonychPolecen[i];
                dlugosciWywodowPolecen[liczbaPolecen++] = dlugosciWywodowPrzenoszonychPolecen[i];
                //printf("Pomyslnie pozyskano polecenie %s\n", nazwyPrzenoszonychPolecen[i]);
            }
        }
    }

    //printf("Koniec wczytywania polecen\n");

    DostosujKon dostosuj = (DostosujKon)GetProcAddress(uchwytDll, "dostosuj"); // jeśli taka funkcja jest to ją wywołujemy
    if(dostosuj) dostosuj(sciagnijObszary, sciagnijNazwyZmiennych, sciagnijPoczet, sciagnijPolecenia, sciagnijZarzadzanieZmiennymi, sciagnijZarzadzanieStosemWyjsc);
    //if(dostosuj) printf("Wywolano dostosowywanie\n");

    dllDoZwolnienia = (HMODULE*)realloc(dllDoZwolnienia, sizeof(HMODULE) * (liczbaDllDoZwolnienia + 1));
    dllDoZwolnienia[liczbaDllDoZwolnienia++] = uchwytDll;
    //printf("Koniec wczytywania\n");
}

void wcielanie(char* nazwa)
{
    FILE* pek = fopen(nazwa, "rb");
    if(!pek) niezbywalnyBlad("Nie udalo sie otworzyc peku o podanej nazwie");
    fseek(pek, 0, SEEK_END);
    long rozmiar = ftell(pek);
    if (rozmiar < 0)
    {
        fclose(pek);
        niezbywalnyBlad("Nie udalo sie odczytac rozmiaru peku");
    }
    rewind(pek);
    size_t calosc = koniec - poczatek;
    size_t doObecnego = obecny - poczatek;
    size_t doKonca = koniec - obecny;
    size_t wymagany = rozmiar + 1 + doKonca;
    if(calosc < wymagany)
    {
        poczatek = realloc(poczatek, wymagany + 1);
        if(!poczatek) niezbywalnyBlad("Brak pamieci");
        memmove(poczatek + rozmiar + 1, poczatek + doObecnego, doKonca);
        if(fread(poczatek, 1, rozmiar, pek) != (size_t)rozmiar) niezbywalnyBlad("Blad odczytu peku");
        koniec = poczatek + wymagany;
        poczatek[rozmiar] = '\n';
        *koniec = '\0';
        obecny = poczatek;
    }
    else
    {
        *(--obecny) = '\n';
        obecny -= rozmiar;
        if(fread(obecny, 1, rozmiar, pek) != (size_t)rozmiar) niezbywalnyBlad("Blad odczytu peku");
    }
    fclose(pek);
}


void wezwij()
{
    //printf("Rozpoczeto wzywanie\n");
    Zmienna** z = oziny[*((size_t*)(poczet + odnosnikPolecenia))].wWZmiennej;
    size_t r = (*z)->rozmiar;
    char* p = malloc(r + 1);
    memcpy(p, zawartosc(*z), r);
    p[r] = '\0';
    wzywanie(p);
    free(p);
    odnosnikPolecenia += sizeof(size_t);
}

void tlumacz()
{
    //printf("Rozpoczeto tlumaczenie\n");

    dlugoscTrzymakaNazwTymczasowych = snprintf(NULL, 0, "!t%zu", SIZE_MAX) + 1;
    trzymakNazwTymaczasowych = malloc(dlugoscTrzymakaNazwTymczasowych);
    if(!trzymakNazwTymaczasowych) niezbywalnyBlad("Brak pamieci");

    poczatekPola = obecny;

    size_t odonosnikObszaruGlownego = wczytujJakoObszarGlowny(&obecny);
    //printf("Zwrocono obszar glowny o odnosniku %zu\n", odonosnikObszaruGlownego);

    wWWykonywanegoObszaruGlownego = pozyskajZmiennaZObszaru(*wWObszaruPowszechnego, odonosnikObszaruGlownego);

    free(trzymakNazwTymaczasowych);
    trzymakNazwTymaczasowych = NULL;
}

void czytaj()
{
    //przelaczNaObszar(zmienne[23], NULL);
    //Zmienna** znalezionyObszar = znajdzZmiennaWObszarze(wWObszaruPowszechnego, znajdzLubDodajNazweZmiennej("$glowny", 0));
    //printf("Zaraz przelaczymy na obszar %p\n", wWWykonywanegoObszaruGlownego);
    przelaczNaObszar(wWWykonywanegoObszaruGlownego);
    //printf("Przelaczano na obszar glowny\n");
    //niezbywalnyBlad("Koniec testu");
}

void zakoncz()
{
    /*for(size_t i = 1; i < liczbaZmiennych; i++)
    {
        if(zmienne[i]) free(zmienne[i]);
    }
    if(zmienne) free(zmienne);*/
    for(size_t i = 1; i < liczbaNazwZmiennych; i++)
    {
        if(nazwyZmiennych[i]) free(nazwyZmiennych[i]);
    }
    if(nazwyZmiennych) free(nazwyZmiennych);
    for(int i = 0; i < liczbaDllDoZwolnienia; i++)
    {
        if(dllDoZwolnienia[i]) FreeLibrary(dllDoZwolnienia[i]);
    }
    if(dllDoZwolnienia) free(dllDoZwolnienia);
    if(poczatek) free(poczatek);
    if(polecenia) free(polecenia);
    if(trzymakNazwTymaczasowych) free(trzymakNazwTymaczasowych);
    if(zrodlo) fclose(zrodlo);
    if(pekCzytelnegoPocztu) fclose(pekCzytelnegoPocztu);
    printf("\033[1;32mPomyslnie zakonczono dzialanie\033[0m\n");
    getchar();
    exit(EXIT_SUCCESS);
}

void pozyskaj_reszte()
{
    Zmienna** z = oziny[*((size_t*)(poczet + odnosnikPolecenia))].wWZmiennej;
    ustawZawartoscZmiennej(z, (size_t)(koniec - obecny), obecny);
    (*z)->rod = 4;
    odnosnikPolecenia += sizeof(size_t);
    //printf("Pozyskano reszte\n");
}

void ustaw_reszte()
{
    Zmienna** z = oziny[*((size_t*)(poczet + odnosnikPolecenia))].wWZmiennej;
    size_t r = (*z)->rozmiar;
    if(koniec - poczatek < (*z)->rozmiar)
    {
        poczatek = realloc(poczatek, r + 1);
        if(!poczatek) niezbywalnyBlad("Brak pamieci");
    }
    memcpy(poczatek, zawartosc(*z), r);
    obecny = poczatek;
    koniec = poczatek + r;
    *koniec = '\0';
    odnosnikPolecenia += sizeof(size_t);
    //printf("Ustawiono reszte\n");
}

void wciel()
{
    //printf("Rozpoczeto wcielanie\n");
    Zmienna** z = oziny[*((size_t*)(poczet + odnosnikPolecenia))].wWZmiennej;
    size_t r = (*z)->rozmiar;
    char* p = malloc(r + 1);
    memcpy(p, zawartosc(*z), r);
    p[r] = '\0';
    wcielanie(p);
    free(p);
    odnosnikPolecenia += sizeof(size_t);
}

int main(int argc, char *argv[])
{
    if(argc < 2) niezbywalnyBlad("Nie podano zrodla");
    zrodlo = fopen(argv[1], "rb");
    if(zrodlo == NULL) niezbywalnyBlad("Nie mozna otworzyc zrodla");

    size_t dlugosc = strlen(argv[1]);

    size_t poczetPrzygotowawczy = 0;
    if(dlugosc > 4 && strcmp(argv[1] + dlugosc - 4, ".czp") == 0) przygotowanieDlaCzp();
    else if(dlugosc > 3 && strcmp(argv[1] + dlugosc - 3, ".cz") == 0) poczetPrzygotowawczy = przygotowanieDlaCz();
    else niezbywalnyBlad("Wymagany pek .czp lub .cz");
    //printf("Zapisano poczet przygotowawczy na miejscu %zu\n", poczetPrzygotowawczy);

    char sciezka[MAX_PATH];
    strncpy(sciezka, argv[1], sizeof(sciezka));
    sciezka[sizeof(sciezka) - 1] = '\0';
    char dysk[_MAX_DRIVE], kat[_MAX_DIR];
    _splitpath(sciezka, dysk, kat, NULL, NULL);
    char folder[MAX_PATH];
    snprintf(folder, sizeof(folder), "%s%s", dysk, kat);
    SetCurrentDirectoryA(folder);

    mp_get_memory_functions(NULL, NULL, &zwolnijPamiecZMpz);

    pojemnoscStosuWyjsc = 2;
    stosWyjsc = realloc(stosWyjsc, pojemnoscStosuWyjsc * sizeof(ZmiennaILiczba));
    if(!stosWyjsc) niezbywalnyBlad("Brak pamieci");

    polecenia = (Polecenie*)malloc(sizeof(Polecenie) * pojemnoscPolecen);
    if(!polecenia) niezbywalnyBlad("Brak pamieci");
    polecenia[0] = NULL;
    polecenia[1] = wezwij;
    polecenia[2] = tlumacz;
    polecenia[3] = czytaj;
    polecenia[4] = zakoncz;
    polecenia[5] = pozyskaj_reszte;
    polecenia[6] = ustaw_reszte;
    polecenia[7] = wciel;

    nazwyPolecen = (char**)malloc(sizeof(char*) * pojemnoscPolecen);
    nazwyPolecen[0] = NULL;
    nazwyPolecen[1] = "wezwij_dom";
    nazwyPolecen[2] = "tlumacz_dom";
    nazwyPolecen[3] = "czytaj_dom";
    nazwyPolecen[4] = "zakoncz_dom";
    nazwyPolecen[5] = "pozyskaj_reszte_dom";
    nazwyPolecen[6] = "ustaw_reszte_dom";
    nazwyPolecen[7] = "wciel_dom";

    dlugosciWywodowPolecen = (size_t*)malloc(sizeof(size_t) * pojemnoscPolecen);
    dlugosciWywodowPolecen[0] = 0;
    dlugosciWywodowPolecen[1] = sizeof(size_t);
    dlugosciWywodowPolecen[2] = 0;
    dlugosciWywodowPolecen[3] = 0;
    dlugosciWywodowPolecen[4] = 0;
    dlugosciWywodowPolecen[5] = sizeof(size_t);
    dlugosciWywodowPolecen[6] = sizeof(size_t);
    dlugosciWywodowPolecen[7] = sizeof(size_t);

    liczbaPolecen = 8;

    //printf("Wzywanie podstawy.dll\n");
    wzywanie("podstawy.dll");

    //przelaczNaObszar(zmienne[obszarPrzygotowawczy], NULL);
    przelaczNaObszar(wWObszaruPowszechnego);
    przelaczNaPoczet(pozyskajZmiennaZObszaru(*wWObszaruPowszechnego, poczetPrzygotowawczy));

    fseek(zrodlo, 0, SEEK_END);
    size_t rozmiar = ftell(zrodlo);
    rewind(zrodlo);

    poczatek = (char*)malloc(rozmiar + 1);
    if(!poczatek) niezbywalnyBlad("Brak pamieci");
    fread(poczatek, 1, rozmiar, zrodlo);
    poczatek[rozmiar] = '\0';
    obecny = poczatek;
    koniec = poczatek + rozmiar;

    //printf("Rozpoczeto wykonywanie\n");
    while(TRUE)
    {
        //printf("Zaraz wykonamy polecenie %s (%d)", nazwyPolecen[poczet[odnosnikPolecenia]], poczet[odnosnikPolecenia]);
        //getchar();
        polecenia[poczet[odnosnikPolecenia++]]();
        //printf("Powrot do petli w main\n");
        //getchar();
    }

    //getchar();
    return 0;
}