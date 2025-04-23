#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>

#include "dzielone.h"

typedef void (*Polecenie)();


Zmienna** zmienne = NULL;               // zmienne
size_t liczbaZmiennych = 0;             // liczba używanych zmiennych
size_t pojemnoscZmiennych = 0;          // przypisane zmiennym miejsce

char** nazwyZmiennych = NULL;
size_t liczbaNazwZmiennych = 0;
size_t pojemnoscNazwZmiennych = 0;

FILE* zrodlo;
const char hasloCzp[] = { 0x50, 0x49, 0x45, 0x52, 0x57, 0x53, 0x5A, 0x59 }; // "PIERWSZY" zapisane szesnastkowo
char* poczatek = NULL;
char* koniec = NULL;

size_t liczbaOzinow = 0;
OdnosnikZmiennejINazwa* oziny;
size_t dlugoscPocztu = 0;
size_t* wskaznikOdnosnikaPolecenia;
char* poczet;

char* slowaKluczowe[] = {"###", "jesli", "poki"};
char* dzialania[] = {"+", "-", "*", "/", "="};

char* nazwyPolecen[] = {"tlumacz", "czytaj"};
size_t liczbaPolecen = 0;
size_t pojemnoscPolecen = 8;
Polecenie* polecenia = NULL;
//HMODULE* dllDoZwolnienia = NULL;

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
    if(zrodlo) fclose(zrodlo);
    for(size_t i = 1; i < liczbaZmiennych; i++)
    {
        free(zmienne[i]);
    }
    free(zmienne);
    for(size_t i = 1; i < liczbaNazwZmiennych; i++)
    {
        free(nazwyZmiennych[i]);
    }
    free(nazwyZmiennych);
    free(poczatek);
    free(polecenia);
    getchar();
    exit(1);
}

size_t utworzZmienna(size_t rozmiar, size_t pojemnosc, size_t rod, void* zawartosc)
{
    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zawartosc, rozmiar);
    if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(!zmienne) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    return liczbaZmiennych++;
}

size_t utworzZmiennaZCechami(size_t rozmiar, size_t pojemnosc, size_t rod, size_t cechy, char* zawartosc, size_t* zawartoscCech)
{
    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc + cechy * sizeof(size_t));
    if(zmienna == NULL) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = cechy;
    char *w = (char*)zmienna + sizeof(Zmienna);
    memcpy(w, zawartosc, rozmiar);
    memcpy(w + pojemnosc, zawartoscCech, cechy * sizeof(size_t));
    if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(zmienne == NULL) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    return liczbaZmiennych++;
}

// $ - obszary główne
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

size_t znajdzLubDodajNazweZmiennej(char* nazwa, size_t dlugosc)
{
    if(nazwa == NULL) return 0;
    if(dlugosc == 0) dlugosc = strlen(nazwa);
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

char* zawartosc(Zmienna* zmienna)
{
    return (char*)zmienna + sizeof(Zmienna);
}

void przelaczNaObszar(Zmienna* obszar)
{
    char* dane = zawartosc(obszar);
    memcpy(&liczbaOzinow, dane, sizeof(size_t));
    memcpy(&dlugoscPocztu, dane + sizeof(size_t), sizeof(size_t));
    oziny = (OdnosnikZmiennejINazwa*)(dane + sizeof(size_t) * 2);
    poczet = dane + sizeof(size_t) * 2 + sizeof(OdnosnikZmiennejINazwa) * liczbaOzinow;
}

void przygotowaniePodstawowe()
{
    const int liczbaZmiennychPodstawowych = 5;

    pojemnoscZmiennych = nastepnaPotegaDwojki(liczbaZmiennychPodstawowych + 1);
    zmienne = (Zmienna**)malloc(pojemnoscZmiennych*sizeof(Zmienna*));
    zmienne[0] = NULL;
    liczbaZmiennych = 1;

    pojemnoscNazwZmiennych = pojemnoscZmiennych;
    nazwyZmiennych = (char**)malloc(pojemnoscNazwZmiennych*sizeof(char*));
    nazwyZmiennych[0] = NULL;
    liczbaNazwZmiennych = 1;

    // Tworzenie zmiennej obszaru
    size_t pustyObszar[2] = {0};
    utworzZmienna(sizeof(pustyObszar), sizeof(pustyObszar), 1, (char*)pustyObszar);
    dodajNazweZmiennej("obs", 3);

    // Tworzenie zmiennej liczby
    char pustaLiczba = 0;
    utworzZmienna(1, 1, 2, &pustaLiczba);
    dodajNazweZmiennej("lic", 3);

    // Tworzenie zmiennej pisma
    char pustePismo[1];
    utworzZmienna(0, 1, 3, pustePismo);
    dodajNazweZmiennej("pis", 3);

    // Tworzenie zmiennej tak-lub-nie (boolean)
    char pustyTln = 0;
    utworzZmienna(1, 1, 4, &pustyTln);
    dodajNazweZmiennej("tln", 3);
}

void przygotowanieDlaCzp()
{
    const int r = sizeof(hasloCzp)/sizeof(char);
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
    zmienne[liczbaZmiennych] = obszarDomyslny;
}

size_t przygotowanieDlaCz()
{
    przygotowaniePodstawowe();
    size_t w1 = 0;
    size_t w2 = 2;
    char p1 = 0;
    char p2 = 1;
    char zawartosc[sizeof(size_t) * 2 + 2];

    memcpy(zawartosc, &w1, sizeof(size_t));
    memcpy(zawartosc + sizeof(size_t), &w2, sizeof(size_t));
    memcpy(zawartosc + sizeof(size_t) * 2, &p1, sizeof(char));
    memcpy(zawartosc + sizeof(size_t) * 2 + sizeof(char), &p2, sizeof(char));
    size_t odnosnikZmiennej = utworzZmienna(sizeof(size_t) * 2 + 2, 0, 1, zawartosc);
    size_t odnosnikNazwy = dodajNazweZmiennej("$domyslny", 9);
    if(odnosnikZmiennej == odnosnikNazwy) return odnosnikZmiennej;
    else niezbywalnyBlad("Pomieszanie odnosnikow");
    return 0;
}

Czastka wczytujNastepne(char** wskaznik);
size_t wczytujJakoNazwa(char** wskaznik);

size_t wczytujJakoObszarGlowny(char** wskaznik)
{
    printf("Rozpoczeto wczytywanie obszaru glownego\n");
    char* polozenie = *wskaznik;
    
    size_t liczbaZmiennychObszaru = 1;
    size_t pojemnoscZmiennychObszaru = 1;
    OdnosnikZmiennejINazwa* zmienneObszaru = (OdnosnikZmiennejINazwa*)malloc(sizeof(OdnosnikZmiennejINazwa) * pojemnoscZmiennychObszaru);
    zmienneObszaru[0].odnosnik = 0;
    zmienneObszaru[0].nazwa = NULL;

    Czastka* czastkiObszaru = (Czastka*)malloc(sizeof(Czastka));
    size_t pojemnoscCzastekObszaru = 1;
    size_t rozmiarCzastekObszaru = 0;

    while(TRUE)
    {
        while(polozenie[0] == ' ' || polozenie[0] == '\n') polozenie++;
        if(polozenie >= koniec)
        {
            *wskaznik = polozenie;
            printf("Wyjebalo sie X_X");
            break;
        }
        printf("Zapierdalam");
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
        else printf("Czastka %zu: %hhu - %p\n", i, czastkiObszaru[i].rodzaj, (void*)czastkiObszaru[i].zawartosc);
    }

    printf("Rozpoczeto robienie drzewa\n");
    Rozgalezienie* drzewo = robDrzewo(czastkiObszaru, rozmiarCzastekObszaru);
    printf("Drzewo:\n");
    wypiszDrzewo(drzewo, 0);
    zetnijRozgalezienie(drzewo);
    free(zmienneObszaru);
    free(czastkiObszaru);

    return 0;
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
    OdnosnikZmiennejINazwa* zmienneObszaru = (OdnosnikZmiennejINazwa*)malloc(sizeof(OdnosnikZmiennejINazwa) * pojemnoscZmiennychObszaru);
    zmienneObszaru[0].odnosnik = 0;
    zmienneObszaru[0].nazwa = NULL;
    while(polozenie[0] == ' ' || polozenie[0] == '\n') polozenie++;
    if(polozenie[0] == '(')
    {
        polozenie++;
        while(TRUE)
        {
            while(polozenie[0] == ' ' || polozenie[0] == '\n') polozenie++;
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
                zmienneObszaru = (OdnosnikZmiennejINazwa*)realloc(zmienneObszaru, sizeof(OdnosnikZmiennejINazwa)* pojemnoscZmiennychObszaru);
                if(zmienneObszaru == NULL) niezbywalnyBlad("Brak pamieci");
            }
            zmienneObszaru[liczbaZmiennychObszaru].odnosnik = odnosnik;
            zmienneObszaru[liczbaZmiennychObszaru++].nazwa = zawartosc(zmienne[odnosnik]);

            while(polozenie[0] == ' ' || polozenie[0] == '\n') polozenie++;

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
            while(polozenie[0] == ' ' || polozenie[0] == '\n') polozenie++;
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
    while(isdigit((unsigned char)polozenie[0]) && polozenie < koniec) polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    char* liczba = malloc(dlugosc + 1);
    memcpy(liczba, pierwotne, dlugosc);
    liczba[dlugosc] = '\0';
    mpz_t liczba_mpz;
    mpz_init(liczba_mpz);
    mpz_set_str(liczba_mpz, liczba, 10);
    uintptr_t rozmiar;
    unsigned char* surowa = (unsigned char*)mpz_export(NULL, &rozmiar, 1, 1, 0, 0, liczba_mpz);
    size_t wynik = utworzZmienna(rozmiar, 1, 2, surowa);
    mpz_clear(liczba_mpz);
    free(liczba);
    if(surowa) free(surowa);
    *wskaznik += dlugosc;
    return wynik;
}

size_t wczytujJakoPismo(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(polozenie[0] != '"') return 0;
    char* pierwotne = ++polozenie;
    while(polozenie[0] != '"') polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    size_t wynik = utworzZmienna(dlugosc, 1, 3, pierwotne);
    *wskaznik += dlugosc + 2;
    return wynik;
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
    return utworzZmienna(sizeof(char*), 0, 0, &(nazwyZmiennych[nazwa])); // utworzenie zmiennej przechowującej wskaźnik do nazwy (a nie nazwę)
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

size_t (*konajkiDoZmiennych[])(char**) = {wczytujJakoObszar, wczytujJakoLiczbe, wczytujJakoPismo, wczytujJakoNazwa};

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
        return c;
    }
    wynik = (uintptr_t)wczytujDzialania(wskaznik);
    if(wynik)
    {
        Czastka c = {3, wynik};
        return c;
    }
    wynik = wczytujZmienne(wskaznik);
    if(wynik)
    {
        Czastka c = {2, wynik};
        return c;
    }
    Czastka c = {0, (uintptr_t)*wskaznik};
    return c;
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

    size_t odonosnikObszaruGlownego = wczytujJakoObszarGlowny(&obecny);
}

void czytaj()
{
    niezbywalnyBlad("Koniec testu");
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

    int dlugosc = strlen(argv[1]);

    size_t obszarPrzygotowawczy = 0;
    if(dlugosc > 4 && strcmp(argv[1] + dlugosc - 4, ".czp") == 0) przygotowanieDlaCzp();
    else if(dlugosc > 3 && strcmp(argv[1] + dlugosc - 3, ".cz") == 0) obszarPrzygotowawczy = przygotowanieDlaCz();
    else
    {
        niezbywalnyBlad("Wymagany pek .czp lub .cz");
    }

    polecenia = (Polecenie*)malloc(sizeof(Polecenie*) * pojemnoscPolecen);
    if(!polecenia) niezbywalnyBlad("Brak pamieci");
    polecenia[0] = tlumacz;
    polecenia[1] = czytaj;
    liczbaPolecen = 2;

    size_t odnosnikaPolecenia = 0;
    wskaznikOdnosnikaPolecenia = &odnosnikaPolecenia;

    przelaczNaObszar(zmienne[obszarPrzygotowawczy]);

    printf("Rozpoczeto wykonywanie\n");
    while(TRUE)
    {
        polecenia[poczet[++(*wskaznikOdnosnikaPolecenia)-1]]();
    }

    getchar();
    return 0;
}