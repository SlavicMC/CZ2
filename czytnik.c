#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

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
char* poczet = NULL;
size_t odnosnikPolecenia = 0;

ZmiennaILiczba* stosWyjsc = NULL;
size_t liczbaStosuWyjsc = 0;
size_t pojemnoscStosuWyjsc = 0;

char** nazwyZmiennych = NULL;
size_t liczbaNazwZmiennych = 0;
size_t pojemnoscNazwZmiennych = 0;

FILE* zrodlo;
const char hasloCzp[] = { 0x50, 0x49, 0x45, 0x52, 0x57, 0x53, 0x5A, 0x59 }; // "PIERWSZY" zapisane szesnastkowo
char* poczatek = NULL;
char* koniec = NULL;

FILE* pekCzytelnegoPocztu;
unsigned int dlugoscTrzymakaNazwTymczasowych;
char* trzymakNazwTymaczasowych = NULL;

void (*zwolnijPamiecZMpz)(void *, size_t);

//size_t liczbaOzinow;
//size_t pojemnoscOzinow;
/*
size_t* wskaznikLiczbyOzinow = NULL;
size_t* wskaznikPojemnosciOzinow = NULL;
Ozin* oziny;
*/
//size_t dlugoscPocztu = 0;
//size_t* wskaznikOdnosnikaPolecenia;
/*size_t* wskaznikDlugosciPocztu = NULL;
size_t* wskaznikPojemnosciPocztu = NULL;
char* poczet;*/

char* slowaKluczowe[] = {"###", "jesli", "poki"};
char* nazwyPolecenSlowKluczowych[] = {"###", "jesli_klc", "poki_klc"};
char* dzialania[] = {"==", "!=", ">=", "<=", "||", "&&", "+=", "-=", "*=", "/=", "%=", "+", "-", "*", "/", "%", "=", "!", ">", "<"}; // działania idą w kolejności malejącej długości (najpierw sprawdzamy najdłuższe)
char* nazwyPolecenDzialan[] = {"rowne_dz", "rozne_dz", "wieksze_badz_rowne_dz", "mniejsze_badz_rowne_dz", "lub_dz", "i_dz", "ndodaj_dz", "nodejmij_dz", "nmnoz_dz", "ndziel_dz", "nreszta_dz", "dodaj_dz", "odejmij_dz", "mnoz_dz", "dziel_dz", "reszta_dz", "nadaj_dz", "nie_dz", "wieksze_dz", "mniejsze_dz"};

Polecenie* polecenia = NULL;
char** nazwyPolecen = NULL;
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
    fprintf(stderr, "[NIEZBYWALNY BLAD] %s\n", wiadomosc);
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
    /*if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(!zmienne) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    return liczbaZmiennych++;*/
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
    /*if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(zmienne == NULL) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    return liczbaZmiennych++;*/
    Zmienna** wskaznik = (Zmienna**)malloc(sizeof(Zmienna*));
    if(!wskaznik) niezbywalnyBlad("Brak pamieci");
    *wskaznik = zmienna;
    return wskaznik;
}

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

    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zaw, rozmiar);
    /*if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(!zmienne) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    if(wskaznikPojemnosciOzinow)
    {
        if(liczbaOzinow >= *wskaznikPojemnosciOzinow)
        {
            *wskaznikPojemnosciOzinow = nastepnaPotegaDwojki(liczbaOzinow + 1);
            oziny = (Ozin*)realloc(oziny, *wskaznikPojemnosciOzinow * sizeof(Ozin));
            if(!oziny) niezbywalnyBlad("Brak pamieci");
        }
        oziny[liczbaOzinow].odnosnik = liczbaZmiennych++;
        oziny[liczbaOzinow].nazwa = znajdzLubDodajNazweZmiennej(nazwa, 0);
        //printf("Utworzono zmienna: %zu (%s)\n", liczbaOzinow, nazwa);
        return (*wskaznikLiczbyOzinow)++;
    }
    printf("ZWROCONO ODNOSNIK ZMIENNEJ, NIE OZINA\n");
    return liczbaZmiennych++;*/
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


/*void sciagnijZmienne(Zmienna*** wskaznikZmiennych, size_t* wskaznikLiczbyZmiennych, size_t* wskaznikPojemnosciZmiennych)
{
    if(wskaznikZmiennych) wskaznikZmiennych = &zmienne;
    if(wskaznikLiczbyZmiennych) wskaznikLiczbyZmiennych = &liczbaZmiennych;
    if(wskaznikPojemnosciZmiennych) wskaznikPojemnosciZmiennych = &pojemnoscZmiennych;
}*/
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

/*void sciagnijPoczet(char*** wWPocztu, size_t*** wWWOdnosnikaPolecenia, size_t*** wWWDlugosciPocztu, size_t*** wWWPojemnosciPocztu)
{
    if(wWPocztu) *wWPocztu = &poczet;
    if(wWWOdnosnikaPolecenia) *wWWOdnosnikaPolecenia = &wskaznikOdnosnikaPolecenia;
    if(wWWDlugosciPocztu) *wWWDlugosciPocztu = &wskaznikDlugosciPocztu;
    if(wWWPojemnosciPocztu) *wWWPojemnosciPocztu = &wskaznikPojemnosciPocztu;
}*/

void sciagnijPoczet(Zmienna**** wWWWWykonywanegoPocztu, char*** wWPocztu, size_t** wWOdnosnikaPolecenia)
{
    if(wWWWWykonywanegoPocztu) *wWWWWykonywanegoPocztu = &wWWykonywanegoPocztu;
    if(wWPocztu) *wWPocztu = &poczet;
    if(wWOdnosnikaPolecenia) *wWOdnosnikaPolecenia = &odnosnikPolecenia;
}

void sciagnijZarzadzanieZmiennymi(void (**wWDodaj)(Zmienna**, size_t, void*), void (**wwUstaw)(Zmienna**, size_t, void*))
{
    if (wWDodaj) *wWDodaj = &dodajZawartoscDoZmiennej;
    if (wwUstaw) *wwUstaw = &ustawZawartoscZmiennej;
}


size_t walkujGalazPodwojna(GalazPodwojna* galaz, size_t liczbaZmiennychTymczasowych);
void walkujRozgalezienie(Rozgalezienie* rozgalezienie, size_t liczbaZmiennychTymczasowych);

size_t walkujGalazPodwojna(GalazPodwojna* galaz, size_t liczbaZmiennychTymczasowych)
{
    if(!galaz) return 0;
    if(galaz->rodzaj == zmienniana)
    {
        return galaz->wartosc;
    }
    for(size_t i = 1; i < liczbaSlowKluczowych; i++) // pierwsze słowo kluczowe pomijamy bo jest ono przetwarzane nieco inaczej
    {
        if(kluczowe[i] == galaz->rodzaj)
        {
            printf("Rozpoczeto wpisywanie slowa kluczowego: %s (liczba zmiennych tymczasowych: %zu)", nazwyPolecenSlowKluczowych[i], liczbaZmiennychTymczasowych);
            getchar();
            if(!galaz->lewa || !galaz->prawa) niezbywalnyBlad("Blad wczytywania slowa kluczowego (brak galezi)");
            size_t poczatekPocztuSlowaKluczowego = (*wWWykonywanegoPocztu)->rozmiar; // zapisujemy gdzie zaczynamy bo wałkowanie lewej gałęzi może i najpewniej go przesunie

            size_t z1 = walkujGalazPodwojna((GalazPodwojna*)(galaz->lewa), liczbaZmiennychTymczasowych);

            /*size_t dlugoscPocztu = *wskaznikDlugosciPocztu;
            size_t pojemnoscPocztu = *wskaznikPojemnosciPocztu;
            size_t wymaganaDlugosc = dlugoscPocztu + 1 + sizeof(size_t)*2;
            if(pojemnoscPocztu < wymaganaDlugosc)
            {
                pojemnoscPocztu = nastepnaPotegaDwojki(wymaganaDlugosc);
                poczet = (char*)realloc(poczet, pojemnoscPocztu);
                if(poczet == NULL) niezbywalnyBlad("Brak pamieci");
            }*/

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
            //memcpy(poczet + dlugoscPocztu, &odnosnikPolecenia, 1);
            //dlugoscPocztu++;
            //memcpy(poczet + dlugoscPocztu, &z1, sizeof(size_t));
            //dlugoscPocztu += sizeof(size_t);
            dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(odnosnikPolecenia), &odnosnikPolecenia);
            dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(z1), &z1);
            fprintf(pekCzytelnegoPocztu, "%s %zu\n", nazwyPolecenSlowKluczowych[i], z1);
            size_t miejsceSkoku = (*wWWykonywanegoPocztu)->rozmiar; // zapisujemy gdzie wpiszemy wartość skoku kiedy ją znajdziemy
            (*wWWykonywanegoPocztu)->rozmiar += sizeof(size_t); // a na razie pomijamy tą wartość

            /*// odswieżamy wskaźniki przed wałkowaniem rozgałęzienia
            *wskaznikDlugosciPocztu = dlugoscPocztu;
            *wskaznikPojemnosciPocztu = pojemnoscPocztu;*/
            
            walkujRozgalezienie((Rozgalezienie*)(galaz->prawa), liczbaZmiennychTymczasowych);

            /*// odświeżamy zmienne po wałkowaniu
            dlugoscPocztu = *wskaznikDlugosciPocztu;
            pojemnoscPocztu = *wskaznikPojemnosciPocztu;*/

            printf("Wpisano slowo kluczowe %s (%s)", nazwyPolecen[odnosnikPolecenia], nazwyPolecenSlowKluczowych[i]);
            if(i == 2) // jeśli to polecenie 'póki' musi mieć skok
            {
                // poszerzamy pamięć jeśli to konieczne
                /*size_t wymaganaDlugosc = dlugoscPocztu + 1 + sizeof(size_t);
                if(pojemnoscPocztu < wymaganaDlugosc)
                {
                    pojemnoscPocztu = nastepnaPotegaDwojki(wymaganaDlugosc);
                    poczet = (char*)realloc(poczet, pojemnoscPocztu);
                    if(poczet == NULL) niezbywalnyBlad("Brak pamieci");
                }*/

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
                /*memcpy(poczet + dlugoscPocztu, &odnosnikPolecenia, 1);
                dlugoscPocztu++;*/
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(odnosnikPolecenia), &odnosnikPolecenia);
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(poczatekPocztuSlowaKluczowego), &poczatekPocztuSlowaKluczowego);
                fprintf(pekCzytelnegoPocztu, "skok_wjk %zu\n", poczatekPocztuSlowaKluczowego);
                /*memcpy(poczet + dlugoscPocztu, &poczatekPocztuSlowaKluczowego, sizeof(size_t));
                dlugoscPocztu += sizeof(size_t);*/
            }
            // teraz wiedząc gdzie jest koniec wpisujemy wartość skoku
            memcpy(zawartosc(*wWWykonywanegoPocztu) + miejsceSkoku, &((*wWWykonywanegoPocztu)->rozmiar), sizeof(size_t));
            printf("\nWpisano wyjatkowe %s (%s)", nazwyPolecen[odnosnikPolecenia], nazwyPolecenSlowKluczowych[i]);
            getchar();
            return 0;
        }
    }
    for(size_t i = 0; i < liczbaDzialan; i++)
    {
        if(dzialaniowe[i] == galaz->rodzaj)
        {
            printf("Rozpoczeto wpisywanie dzialania: %s (liczba zmiennych tymczasowych: %zu)", nazwyPolecenDzialan[i], liczbaZmiennychTymczasowych);
            getchar();
            snprintf(trzymakNazwTymaczasowych, dlugoscTrzymakaNazwTymczasowych, "=t%zu", liczbaZmiennychTymczasowych);
            size_t nazwa = znajdzLubDodajNazweZmiennej(trzymakNazwTymaczasowych, 0);
            size_t wynik = znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, nazwyZmiennych[nazwa], sizeof(size_t), sizeof(size_t), 0, &nazwa);
            size_t z1 = walkujGalazPodwojna((GalazPodwojna*)(galaz->lewa), liczbaZmiennychTymczasowych + 1);
            size_t z2 = walkujGalazPodwojna((GalazPodwojna*)(galaz->prawa), liczbaZmiennychTymczasowych + 2);

            /*size_t dlugoscPocztu = *wskaznikDlugosciPocztu;
            size_t pojemnoscPocztu = *wskaznikPojemnosciPocztu;
            size_t wymaganaDlugosc = dlugoscPocztu + 1 + sizeof(size_t) * 3; // wydłużamy poczet o odnośnik polecenia i 3 argumenty
            if(pojemnoscPocztu < wymaganaDlugosc)
            {
                pojemnoscPocztu = nastepnaPotegaDwojki(wymaganaDlugosc);
                poczet = (char*)realloc(poczet, pojemnoscPocztu);
                if(poczet == NULL) niezbywalnyBlad("Brak pamieci");
            }*/

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
            /*memcpy(poczet + dlugoscPocztu, &odnosnikPolecenia, 1);
            dlugoscPocztu++;
            memcpy(poczet + dlugoscPocztu, &wynik, sizeof(size_t));
            dlugoscPocztu += sizeof(size_t);*/
            if(galaz->lewa) // jeśli mamy lewą stronę dopisujemy ją
            {
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(z1), &z1);
                fprintf(pekCzytelnegoPocztu, " %zu", z1);
                /*memcpy(poczet + dlugoscPocztu, &z1, sizeof(size_t));
                dlugoscPocztu += sizeof(size_t);*/
            }
            if(galaz->prawa) // jeśli mamy prawą stronę dopisujemy ją
            {
                dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(z2), &z2);
                fprintf(pekCzytelnegoPocztu, " %zu", z2);
                /*memcpy(poczet + dlugoscPocztu, &z2, sizeof(size_t));
                dlugoscPocztu += sizeof(size_t);*/
            }
            fprintf(pekCzytelnegoPocztu, "\n");
            /**wskaznikDlugosciPocztu = dlugoscPocztu;
            *wskaznikPojemnosciPocztu = pojemnoscPocztu;*/
            printf("Wpisano dzialanie %s (%s)", nazwyPolecen[odnosnikPolecenia], nazwyPolecenDzialan[i]);
            getchar();
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


void przelaczNaPoczet(Zmienna** wWPocztu)
{
    if(!wWPocztu) return;
    printf("Przelaczam poczet z %p na %p\n", wWWykonywanegoPocztu, wWPocztu);
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
}

void przelaczNaObszar(Zmienna** wWObszaru)
{
    if(!wWObszaru) return;
    przelaczNaPoczet(((Ozin*)zawartosc(*wWObszaru))->wWZmiennej);
    printf("Przelaczam obszar z %p na %p\n", wWWykonywanegoObszaru, wWObszaru);
    if(wWWykonywanegoObszaru)
    {
        if(liczbaStosuWyjsc >= pojemnoscStosuWyjsc)
        {
            pojemnoscStosuWyjsc *= 2;
            stosWyjsc = realloc(stosWyjsc, pojemnoscStosuWyjsc * sizeof(ZmiennaILiczba));
            if(!stosWyjsc) niezbywalnyBlad("Brak pamieci");
        }
        stosWyjsc[liczbaStosuWyjsc].wWZmiennej = wWWykonywanegoObszaru;
        stosWyjsc[liczbaStosuWyjsc].liczba = odnosnikPolecenia;
        liczbaStosuWyjsc++;
    }
    wWWykonywanegoObszaru = wWObszaru;
    oziny = (Ozin*)zawartosc(*wWObszaru);
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
            wWWykonywanegoObszaru = z;
            oziny = (Ozin*)(zawartosc(*z));
            zejdz(); // jeśli zeszliśmy na obszar musimy zejść jeszcze niżej by wczytać poczet
        }
        else if((*z)->rod == 2)
        {
            wWWykonywanegoPocztu = z;
            odnosnikPolecenia = l;
            poczet = zawartosc(*z) + sizeof(Zmienna**); // każdy poczet zaczyna się wskaźnikiem na obszar którego nie chcemy
        }
    }
}

/*void przelaczNaObszar(Zmienna* obszar, size_t* wskaznikPojemnosci)
{
    char* dane = zawartosc(obszar);

    //memcpy(&liczbaOzinow, dane, sizeof(size_t));
    //pojemnoscOzinow = liczbaOzinow;

    wskaznikLiczbyOzinow = (size_t*)dane;
    if(wskaznikPojemnosci != NULL) *wskaznikPojemnosci = *wskaznikLiczbyOzinow;
    //memcpy(&dlugoscPocztu, dane + sizeof(size_t), sizeof(size_t));
    wskaznikPojemnosciPocztu = (size_t*)(dane + sizeof(size_t));
    oziny = (Ozin*)(dane + sizeof(size_t) * 2);
    poczet = dane + sizeof(size_t) * 2 + *wskaznikLiczbyOzinow * sizeof(Ozin);
    for(size_t i = 0; i < *wskaznikLiczbyOzinow; i++)
    {
        printf("%s: %zu\n", nazwyZmiennych[oziny[i].nazwa], oziny[i].odnosnik);
    }
    printf("Pierwszy znak pocztu: %d (%s)\n", poczet[0], nazwyPolecen[poczet[0]]);
}*/

// zaklepuje pamięć i tworzy podstawowe zmienne rodowe
void przygotowaniePodstawowe()
{
    const int liczbaZmiennychPodstawowych = 6;

    size_t p = nastepnaPotegaDwojki(liczbaZmiennychPodstawowych + 1);
    /*pojemnoscZmiennych = nastepnaPotegaDwojki(liczbaZmiennychPodstawowych + 1);
    zmienne = (Zmienna**)malloc(pojemnoscZmiennych*sizeof(Zmienna*));
    zmienne[0] = NULL;
    liczbaZmiennych = 1;*/

    Ozin pustyOzin = {NULL, 0};
    wWObszaruPowszechnego = utworzZmienna(sizeof(pustyOzin), nastepnaPotegaDwojki(p * sizeof(Ozin)), 1, &pustyOzin);

    pojemnoscNazwZmiennych = p;
    nazwyZmiennych = (char**)malloc(pojemnoscNazwZmiennych * sizeof(char*));
    nazwyZmiennych[0] = NULL;
    liczbaNazwZmiennych = 1;

    // Tworzenie zmiennej obszaru
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "obs", 0, 1, 1, NULL);
    //dodajNazweZmiennej("obs", 3);

    // Tworzenie zmiennej pocztu
    Zmienna** pustyPoczet = NULL;
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "pct", sizeof(Zmienna**), sizeof(Zmienna**), 2, &pustyPoczet);

    // Tworzenie zmiennej liczby
    unsigned char pustaLiczba = 0;
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "lic", 1, 1, 3, &pustaLiczba);
    //dodajNazweZmiennej("lic", 3);

    // Tworzenie zmiennej pisma
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "pis", 0, 1, 4, NULL);
    //dodajNazweZmiennej("pis", 3);

    // Tworzenie zmiennej tak-lub-nie (boolean)
    utworzZmiennaWObszarze(wWObszaruPowszechnego, "tln", 1, 1, 5, &pustaLiczba);
    //dodajNazweZmiennej("tln", 3);
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
    /*size_t w1 = 0;
    size_t w2 = 2;
    char p1 = 2;
    char p2 = 3;
    char zaw[sizeof(size_t) * 2 + 2];

    memcpy(zaw, &w1, sizeof(size_t));
    memcpy(zaw + sizeof(size_t), &w2, sizeof(size_t));
    memcpy(zaw + sizeof(size_t) * 2, &p1, sizeof(char));
    memcpy(zaw + sizeof(size_t) * 2 + sizeof(char), &p2, sizeof(char));
    size_t odnosnikZmiennej = utworzZmienna(sizeof(size_t) * 2 + 2, 0, 1, zaw);
    size_t odnosnikNazwy = dodajNazweZmiennej("$domyslny", 9);
    if(odnosnikZmiennej == odnosnikNazwy) return odnosnikZmiennej;
    else niezbywalnyBlad("Pomieszanie odnosnikow");
    return 0;*/
    char zaw[sizeof(Zmienna**) + 2] = {0};
    zaw[sizeof(Zmienna**)] = 2;
    zaw[sizeof(Zmienna**) + 1] = 3;
    return utworzZmiennaWObszarze(wWObszaruPowszechnego, "$domyslny", sizeof(zaw), 0, 2, &zaw);
}


Czastka wczytujNastepne(char** wskaznik);
size_t wczytujJakoNazwa(char** wskaznik);

size_t wczytujJakoObszarGlowny(char** wskaznik)
{
    printf("Rozpoczeto wczytywanie obszaru glownego\n");
    char* polozenie = *wskaznik;
    printf("Zaladowany tekst:\n%s\n", polozenie);
    
    /*size_t liczbaOzinowObszaru = 1;
    size_t pojemnoscOzinowObszaru = 1;
    Ozin* ozinyObszaru = (Ozin*)malloc(sizeof(Ozin) * pojemnoscOzinowObszaru);
    ozinyObszaru[0].odnosnikNazwy = 0;
    ozinyObszaru[0].wWZmiennej = NULL;*/

    /*Ozin* stareOziny = oziny;
    size_t* staryWskaznikLiczbyOzinow = wskaznikLiczbyOzinow;
    size_t* staryWskaznikPojemnosciOzinow = wskaznikPojemnosciOzinow;*/

    Zmienna** wWPocztuGlownego = utworzZmienna(0, sizeof(Zmienna**), 2, NULL);
    Ozin pierwszyOzin = {wWPocztuGlownego, znajdzLubDodajNazweZmiennej("@poczet", 7)};
    size_t odnosnikWWObszaruGlownego = utworzZmiennaWObszarze(wWObszaruPowszechnego, "$glowny", sizeof(pierwszyOzin), sizeof(pierwszyOzin) * 2, 1, &pierwszyOzin);
    Zmienna** wWObszaruGlownego = pozyskajZmiennaZObszaru(*wWObszaruPowszechnego, odnosnikWWObszaruGlownego);
    dodajZawartoscDoZmiennej(wWPocztuGlownego, sizeof(wWObszaruGlownego), &wWObszaruGlownego);
    przelaczNaObszar(wWObszaruGlownego);
    //przelaczNaPoczet(wWPocztuGlownego);

    /*oziny = ozinyObszaru;
    wskaznikLiczbyOzinow = &liczbaOzinowObszaru;
    wskaznikPojemnosciOzinow = &pojemnoscOzinowObszaru;*/

    Czastka* czastkiObszaru = (Czastka*)malloc(sizeof(Czastka));
    size_t pojemnoscCzastekObszaru = 1;
    size_t rozmiarCzastekObszaru = 0;

    while(TRUE)
    {
        while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
        if(polozenie >= koniec)
        {
            *wskaznik = polozenie;
            break;
        }
        Czastka c = wczytujNastepne(&polozenie);
        if(c.rodzaj == 1 && (char*)c.zawartosc == slowaKluczowe[0]) break; // kończymy jeśli napotkamy ###
        if(rozmiarCzastekObszaru >= pojemnoscCzastekObszaru)
        {
            pojemnoscCzastekObszaru *= 2;
            czastkiObszaru = realloc(czastkiObszaru, sizeof(Czastka) * pojemnoscCzastekObszaru);
            if(czastkiObszaru == NULL) niezbywalnyBlad("Brak pamieci");
        }
        czastkiObszaru[rozmiarCzastekObszaru++] = c;
    }

    printf("Wczytano czastki:\n");
    for(size_t i = 0; i < rozmiarCzastekObszaru; i++)
    {
        if(czastkiObszaru[i].rodzaj == 2) printf("Czastka %zu: %hhu - %zu\n", i, czastkiObszaru[i].rodzaj, (size_t)czastkiObszaru[i].zawartosc);
        else if(czastkiObszaru[i].rodzaj == 0) printf("Czastka %zu: %hhu - %c\n", i, czastkiObszaru[i].rodzaj, *((char*)czastkiObszaru[i].zawartosc));
        else printf("Czastka %zu: %hhu - %s\n", i, czastkiObszaru[i].rodzaj, (char*)czastkiObszaru[i].zawartosc);
    }

    getchar();

    printf("Rozpoczeto robienie drzewa\n");
    Rozgalezienie* drzewo = robDrzewo(czastkiObszaru, rozmiarCzastekObszaru);
    printf("Drzewo:\n");
    wypiszDrzewo(drzewo, 0);
    getchar();

    //pekCzytelnegoPocztu = fopen("C:\\Users\\_USER_\\Desktop\\CZ2\\poczet.txt", "w");
    pekCzytelnegoPocztu = fopen("poczet.txt", "w+");
    if(pekCzytelnegoPocztu == NULL)
    {
        perror("fopen (poczet.txt)");
        niezbywalnyBlad("Nie mozna otworzyc peku pocztu (w)");
    }
    /*fclose(pekCzytelnegoPocztu);
    pekCzytelnegoPocztu = fopen("C:\\Users\\_USER_\\Desktop\\CZ2\\poczet.txt", "a");
    if(pekCzytelnegoPocztu == NULL)
    {
        perror("fopen (poczet.txt)");
        niezbywalnyBlad("Nie mozna otworzyc peku pocztu (a)");
    }*/

    /*char* staryPoczet = poczet;
    size_t* staryWskaznikDlugosciPocztu = wskaznikDlugosciPocztu;

    size_t dlugoscPocztu = 0;
    size_t pojemnoscPocztu = 16;
    wskaznikDlugosciPocztu = &dlugoscPocztu;
    wskaznikPojemnosciPocztu = &pojemnoscPocztu;
    poczet = malloc(pojemnoscPocztu);*/

    walkujRozgalezienie(drzewo, 1);
    unsigned char konczace = 4;
    dodajZawartoscDoZmiennej(wWWykonywanegoPocztu, sizeof(konczace), &konczace);
    fprintf(pekCzytelnegoPocztu, "%s\n", nazwyPolecen[konczace]);

    fclose(pekCzytelnegoPocztu);
    pekCzytelnegoPocztu = NULL;
    printf("Zakonczono walkowanie\n");

    /*ozinyObszaru = oziny;
    oziny = stareOziny;
    wskaznikLiczbyOzinow = staryWskaznikLiczbyOzinow;
    wskaznikPojemnosciOzinow = staryWskaznikPojemnosciOzinow;

    size_t r = 2*sizeof(size_t) + liczbaOzinowObszaru * sizeof(Ozin) + dlugoscPocztu;
    char* pamiec = malloc(r + 1);
    memcpy(pamiec, &liczbaOzinowObszaru, sizeof(size_t));
    memcpy(pamiec + sizeof(size_t), &dlugoscPocztu, sizeof(size_t));
    memcpy(pamiec + 2 * sizeof(size_t), ozinyObszaru, liczbaOzinowObszaru * sizeof(Ozin));
    memcpy(pamiec + 2 * sizeof(size_t) + liczbaOzinowObszaru * sizeof(Ozin), poczet, dlugoscPocztu);
    char odnosnikPoleceniaKonczacego = 4;
    memcpy(pamiec + r, &odnosnikPoleceniaKonczacego, 1);
    dlugoscPocztu++;
    size_t wynik = znajdzLubUtworzZmiennaWObszarze(oziny, "$glowny", r + 1, 0, 1, pamiec);*/

    //free(pamiec);
    //free(ozinyObszaru);
    free(czastkiObszaru);
    //wskaznikDlugosciPocztu = staryWskaznikDlugosciPocztu;
    //poczet = staryPoczet;
    //printf("Przywrocono stary poczet\n");

    zetnijRozgalezienie(drzewo);
    zejdz();
    //printf("Scieto drzewo\n");

    //printf("%d\n", wskaznikLiczbyOzinow == &liczbaOzinowObszaru);
    /*for(size_t i = 0; i < liczbaOzinowObszaru; i++)
    {
        printf("%s: %zu\n", nazwyZmiennych[oziny[i].nazwa], oziny[i].odnosnik);
    }*/

    //printf("Uwolniono oziny\n");
    //printf("Uwolniono czastki\n");

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

    // Wczytywanie zbioru wartości przenoszonych (argumentów)
    size_t liczbaZmiennychObszaru = 1;
    size_t pojemnoscZmiennychObszaru = 1;
    Ozin* zmienneObszaru = (Ozin*)malloc(sizeof(Ozin) * pojemnoscZmiennychObszaru);
    //zmienneObszaru[0].odnosnik = 0;
    //zmienneObszaru[0].nazwa = 0;
    while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
    if(polozenie[0] == '(')
    {
        polozenie++;
        while(TRUE)
        {
            while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
            if(polozenie[0] == ')') 
            {
                polozenie++;
                break;
            }
            size_t odnosnik = wczytujJakoNazwa(&polozenie);
            if(odnosnik == 0) niezbywalnyBlad("Blad skladni: nieprawidlowe przekazanie nazw wartosci przenoszonych obszaru");

            if(liczbaZmiennychObszaru >= pojemnoscZmiennychObszaru)
            {
                pojemnoscZmiennychObszaru *= 2;
                zmienneObszaru = (Ozin*)realloc(zmienneObszaru, sizeof(Ozin)* pojemnoscZmiennychObszaru);
                if(zmienneObszaru == NULL) niezbywalnyBlad("Brak pamieci");
            }
            //zmienneObszaru[liczbaZmiennychObszaru].odnosnik = odnosnik;
            //zmienneObszaru[liczbaZmiennychObszaru++].nazwa = zawartosc(zmienne[odnosnik]);

            while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;

            if(polozenie[0] == ',') 
            {
                polozenie++;
            }
            else if(polozenie[0] == ')') 
            {
                polozenie++;
                break;
            }
            else niezbywalnyBlad("Blad skladni: nieprawidlowe przekazanie nazw wartosci przenoszonych obszaru");
        }
    }

    // Wczytywanie spiecia
    // if(polozenie[0] != '{' && polozenie - poczatek != 0) return NULL;
    if(polozenie[0] == '{')
    {
        polozenie++;
        Czastka* czastkiObszaru = (Czastka*)malloc(sizeof(Czastka));
        size_t pojemnoscCzastekObszaru = 1;
        size_t rozmiarCzastekObszaru = 0;
        while(TRUE)
        {
            while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
            if(polozenie >= koniec || polozenie[0] == '}')
            {
                if(polozenie[0] == '}') polozenie += 1;
                *wskaznik = polozenie;
                break;
            }
            Czastka c = wczytujNastepne(&polozenie);
            if(pojemnoscCzastekObszaru == rozmiarCzastekObszaru)
            {
                pojemnoscCzastekObszaru *= 2;
                czastkiObszaru = realloc(czastkiObszaru, sizeof(Czastka) * pojemnoscCzastekObszaru);
                if(czastkiObszaru == NULL) niezbywalnyBlad("Brak pamieci");
            }
            czastkiObszaru[rozmiarCzastekObszaru++] = c;
        }

        Rozgalezienie* drzewo = robDrzewo(czastkiObszaru, rozmiarCzastekObszaru);
    }
    

    /*Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + 2*sizeof(uintptr_t));
    zmienna->rod = 1;
    zmienna->rozmiar = 2*sizeof(uintptr_t);
    zmienna->pojemnosc = 2*sizeof(uintptr_t);
    zmienna->cechy = 0;
    memset((char*)zmienna + sizeof(Zmienna), 0, 2 * sizeof(uintptr_t));
    return zmienna;*/
    return 0;
}

size_t wczytujJakoLiczbe(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(!isdigit((unsigned char)polozenie[0])) return 0;
    char* pierwotne = polozenie;
    while(polozenie < koniec && isdigit((unsigned char)polozenie[0])) polozenie++;

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
    if(polozenie[0] != '"') return 0;
    char* pierwotne = ++polozenie;
    while(polozenie[0] != '"') polozenie++;
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
    if(!isalpha((unsigned char)polozenie[0])) return 0;
    char* pierwotne = polozenie;
    while(isalpha((unsigned char)polozenie[0])) polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    size_t nazwa = znajdzLubDodajNazweZmiennej(pierwotne, dlugosc);
    *wskaznik += dlugosc;
    //return utworzZmienna(sizeof(size_t), 0, 0, &(nazwyZmiennych[nazwa])); // utworzenie zmiennej odnosnik do nazwy (a nie nazwę)
    return znajdzLubUtworzZmiennaWObszarze(wWWykonywanegoObszaru, nazwyZmiennych[nazwa], sizeof(size_t), 0, 0, &nazwa); // utworzenie zmiennej odnosnik do nazwy (a nie nazwę)
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
    wynik = (uintptr_t)wczytujKluczowe(wskaznik);
    if(wynik)
    {
        Czastka c = {1, wynik};
        printf("Wczytano kluczowe: %s\n", (char*)wynik);
        return c;
    }
    wynik = (uintptr_t)wczytujDzialania(wskaznik);
    if(wynik)
    {
        Czastka c = {3, wynik};
        printf("Wczytano dzialanie: %s\n", (char*)wynik);
        return c;
    }
    wynik = wczytujZmienne(wskaznik);
    if(wynik)
    {
        Czastka c = {2, wynik};
        printf("Wczytano zmienna: %zu (nazwa: %s)\n", (size_t)wynik, nazwyZmiennych[pozyskajOdnosnikNazwyZObszaru(*wWWykonywanegoObszaru, (size_t)wynik)]);
        return c;
    }
    printf("Wczytano nieznany: %c\n", **wskaznik);
    Czastka c = {0, (uintptr_t)((*wskaznik)++)};
    return c;
}


void wzywanie(char* nazwa)
{
    HMODULE uchwytDll = LoadLibrary(nazwa);
    if(!uchwytDll) niezbywalnyBlad("Nie udalo sie wczytac polecen");
    printf("Pomyslnie pozyskano uchwyt\n");

    //char*** wskaznikNazwPrzenoszonychPolecen = (char***)GetProcAddress(uchwytDll, "nazwyPrzenoszonychPolecen");
    char** nazwyPrzenoszonychPolecen = (char**)GetProcAddress(uchwytDll, "nazwyPrzenoszonychPolecen");
    size_t* wskaznikLiczbyPrzenoszonychPolecen = (size_t*)GetProcAddress(uchwytDll, "liczbaPrzenoszonychPolecen");

    if(nazwyPrzenoszonychPolecen && wskaznikLiczbyPrzenoszonychPolecen) // jeśli oba zosatły wczytane przenosimy polecenia (ich obecność nie jest obowiązkowa)
    {
        printf("Pomyslnie pozyskano nazwy, rozpoczynam wczytywanie\n");
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
                    if(!polecenia || !nazwyPolecen) niezbywalnyBlad("Brak pamieci");
                }
                polecenia[liczbaPolecen] = p;
                printf("Pomyslnie pozyskano polecenie %s\n", nazwyPrzenoszonychPolecen[i]);
                nazwyPolecen[liczbaPolecen++] = nazwyPrzenoszonychPolecen[i];
            }
        }
    }

    printf("Koniec wczytywania polecen\n");

    DostosujKon dostosuj = (DostosujKon)GetProcAddress(uchwytDll, "dostosuj"); // jeśli taka funkcja jest to ją wywołujemy
    if(dostosuj) dostosuj(sciagnijObszary, sciagnijNazwyZmiennych, sciagnijPoczet, sciagnijZarzadzanieZmiennymi);
    if(dostosuj) printf("Wywolano dostosowywanie\n");

    dllDoZwolnienia = (HMODULE*)realloc(dllDoZwolnienia, sizeof(HMODULE) * (liczbaDllDoZwolnienia + 1));
    dllDoZwolnienia[liczbaDllDoZwolnienia++] = uchwytDll;
    printf("Koniec wczytywania\n");
}


void wezwij()
{

}

void tlumacz()
{
    printf("Rozpoczeto tlumaczenie\n");
    fseek(zrodlo, 0, SEEK_END);
    size_t rozmiar = ftell(zrodlo);
    rewind(zrodlo);

    poczatek = (char*)malloc(rozmiar + 1);
    if(!poczatek) niezbywalnyBlad("Brak pamieci");
    fread(poczatek, 1, rozmiar, zrodlo);
    poczatek[rozmiar] = '\0';
    char* obecny = poczatek;
    koniec = poczatek + rozmiar;

    dlugoscTrzymakaNazwTymczasowych = snprintf(NULL, 0, "!t%zu", SIZE_MAX) + 1;
    trzymakNazwTymaczasowych = malloc(dlugoscTrzymakaNazwTymczasowych);
    if(!trzymakNazwTymaczasowych) niezbywalnyBlad("Brak pamieci");

    size_t odonosnikObszaruGlownego = wczytujJakoObszarGlowny(&obecny);
    printf("Zwrocono obszar glowny o odnosniku %zu\n", odonosnikObszaruGlownego);

    free(trzymakNazwTymaczasowych);
    trzymakNazwTymaczasowych = NULL;
}

void czytaj()
{
    //przelaczNaObszar(zmienne[23], NULL);
    Zmienna** znalezionyObszar = znajdzZmiennaWObszarze(wWObszaruPowszechnego, znajdzLubDodajNazweZmiennej("$glowny", 0));
    printf("Zaraz przelaczymy na obszar %p\n", znalezionyObszar);
    przelaczNaObszar(znalezionyObszar);
    printf("Przelaczano na obszar glowny\n");
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
    printf("Pomyslnie zakonczono dzialanie\n");
    getchar();
    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        niezbywalnyBlad("Nie podano zrodla");
    }
    zrodlo = fopen(argv[1], "rb");
    if(zrodlo == NULL)
    {
        niezbywalnyBlad("Nie mozna otworzyc zrodla");
    }

    size_t dlugosc = strlen(argv[1]);

    size_t poczetPrzygotowawczy = 0;
    if(dlugosc > 4 && strcmp(argv[1] + dlugosc - 4, ".czp") == 0) przygotowanieDlaCzp();
    else if(dlugosc > 3 && strcmp(argv[1] + dlugosc - 3, ".cz") == 0) poczetPrzygotowawczy = przygotowanieDlaCz();
    else niezbywalnyBlad("Wymagany pek .czp lub .cz");
    printf("Zapisano poczet przygotowawczy na miejscu %zu\n", poczetPrzygotowawczy);

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

    nazwyPolecen = (char**)malloc(sizeof(char*) * pojemnoscPolecen);
    nazwyPolecen[0] = NULL;
    nazwyPolecen[1] = "wezwij_dom";
    nazwyPolecen[2] = "tlumacz_dom";
    nazwyPolecen[3] = "czytaj_dom";
    nazwyPolecen[4] = "zakoncz_dom";

    liczbaPolecen = 5;

    printf("Wzywanie podstawy.dll\n");
    wzywanie("podstawy.dll");

    //przelaczNaObszar(zmienne[obszarPrzygotowawczy], NULL);
    przelaczNaObszar(wWObszaruPowszechnego);
    przelaczNaPoczet(pozyskajZmiennaZObszaru(*wWObszaruPowszechnego, poczetPrzygotowawczy));

    printf("Rozpoczeto wykonywanie\n");
    while(TRUE)
    {
        printf("Zaraz wykonamy polecenie %s (%d)", nazwyPolecen[poczet[odnosnikPolecenia]], poczet[odnosnikPolecenia]);
        getchar();
        polecenia[poczet[odnosnikPolecenia++]]();
        printf("Powrot do petli w main\n");
        getchar();
    }

    getchar();
    return 0;
}