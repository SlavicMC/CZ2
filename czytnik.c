#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>

#include "dzielone.h"

typedef void (*Polecenie)();


Zmienna** zmienne;                      // zmienne
size_t liczbaZmiennych = 0;             // liczba używanych zmiennych
size_t pojemnoscZmiennych = 0;          // przypisane zmiennym miejsce

FILE* zrodlo;
const char hasloCzp[] = { 0x50, 0x49, 0x45, 0x52, 0x57, 0x53, 0x5A, 0x59 }; // "PIERWSZY" zapisane szesnastkowo
char* poczatek = NULL;
char* koniec = NULL;

char** nazwyPolecen = {"tlumacz", "czytaj"};
int liczbaPolecen = 0;
Polecenie* polecenia = NULL;
HMODULE* dllDoZwolnienia = NULL;

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
    getchar();
    exit(1);
}

void przygotowaniePodstawowe()
{
    const int liczbaZmiennychPodstawowych = 5;
    zmienne = (Zmienna**)malloc((liczbaZmiennychPodstawowych + 1)*sizeof(Zmienna*));
    pojemnoscZmiennych = liczbaZmiennychPodstawowych + 1;

    // Tworzenie zmiennej obszaru
    Zmienna* zmiennaObszaru = (Zmienna*)malloc(sizeof(Zmienna) + 2*sizeof(uintptr_t));
    zmiennaObszaru->rod = 1;
    zmiennaObszaru->rozmiar = 2*sizeof(uintptr_t);
    zmiennaObszaru->pojemnosc = 2*sizeof(uintptr_t);
    zmiennaObszaru->cechy = 0;
    memset((char*)zmiennaObszaru + sizeof(Zmienna), 0, 2 * sizeof(uintptr_t));

    // Tworzenie zmiennej liczby
    Zmienna* zmiennaLiczby = (Zmienna*)malloc(sizeof(Zmienna) + 1);
    zmiennaLiczby->rod = 2;
    zmiennaLiczby->rozmiar = 1;
    zmiennaLiczby->pojemnosc = 1;
    zmiennaLiczby->cechy = 0;
    memset((char*)zmiennaLiczby + sizeof(Zmienna), 0, 1);

    // Tworzenie zmiennej pisma
    Zmienna* zmiennaPisma = (Zmienna*)malloc(sizeof(Zmienna) + 1);
    zmiennaPisma->rod = 3;
    zmiennaPisma->rozmiar = 0;
    zmiennaPisma->pojemnosc = 1;
    zmiennaPisma->cechy = 0;

    // Tworzenie zmiennej tak-lub-nie (boolean)
    Zmienna* zmiennaTln = (Zmienna*)malloc(sizeof(Zmienna) + 1);
    zmiennaTln->rod = 4;
    zmiennaTln->rozmiar = 1;
    zmiennaTln->pojemnosc = 1;
    zmiennaTln->cechy = 0;
    memset((char*)zmiennaTln + sizeof(Zmienna), 0, 1);

    zmienne[0] = NULL;
    zmienne[1] = zmiennaObszaru;
    zmienne[2] = zmiennaLiczby;
    zmienne[3] = zmiennaPisma;
    zmienne[4] = zmiennaTln;

    liczbaZmiennych = 5;
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

void przygotowanieDlaCz()
{
    przygotowaniePodstawowe();
    uintptr_t rozmiar = 2 * sizeof(uintptr_t) + 2;
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
    *((char*)w + sizeof(uintptr_t)) = 1;
    *((char*)w + sizeof(uintptr_t) + 1) = 2;
    zmienne[liczbaZmiennych] = obszarDomyslny;
}

Czastka wczytujNastepne(char** wskaznik);

Zmienna* wczytujJakoObszar(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(polozenie[0] != '{' && polozenie - poczatek != 0) return NULL;
    if(polozenie[0] == '{') polozenie++;
    Czastka* czastkiObszaru = (Czastka*)malloc(sizeof(Czastka));
    size_t pojemnoscObszaru = 1;
    size_t rozmiarObszaru = 0;
    while(TRUE)
    {
        while(polozenie[0] == ' ' || polozenie[0] == '\n') polozenie++;
        if(polozenie >= koniec) break;
        if (polozenie >= koniec || polozenie[0] == '}')
        {
            if(polozenie[0] == '}') polozenie += 1;
            *wskaznik = polozenie;
            break;
        }
        Czastka c = wczytujNastepne(&polozenie);
        if(pojemnoscObszaru == rozmiarObszaru)
        {
            pojemnoscObszaru *= 2;
            czastkiObszaru = realloc(czastkiObszaru, sizeof(Czastka) * pojemnoscObszaru);
            if(czastkiObszaru == NULL) niezbywalnyBlad("Brak pamieci");
        }
        czastkiObszaru[rozmiarObszaru++] = c;
    }
    

    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + 2*sizeof(uintptr_t));
    zmienna->rod = 1;
    zmienna->rozmiar = 2*sizeof(uintptr_t);
    zmienna->pojemnosc = 2*sizeof(uintptr_t);
    zmienna->cechy = 0;
    memset((char*)zmienna + sizeof(Zmienna), 0, 2 * sizeof(uintptr_t));
    return zmienna;
}

Zmienna* wczytujJakoLiczbe(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(!isdigit((unsigned char)polozenie[0])) return NULL;
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
    uintptr_t pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + rozmiar);
    zmienna->cechy = 0;
    zmienna->rod = 2;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rozmiar = rozmiar;
    memcpy((char*)zmienna + sizeof(Zmienna), surowa, rozmiar);
    mpz_clear(liczba_mpz);
    free(liczba);
    free(surowa);
    *wskaznik += dlugosc;
    return zmienna;
}

Zmienna* wczytujJakoPismo(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(polozenie[0] != '"') return NULL;
    char* pierwotne = ++polozenie;
    while(polozenie[0] != '"') polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    uintptr_t pojemnosc = nastepnaPotegaDwojki(dlugosc);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(zmienna == NULL) niezbywalnyBlad("Brak pamieci");
    zmienna->cechy = 0;
    zmienna->rod = 3;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rozmiar = dlugosc;
    memcpy((char*)zmienna + sizeof(Zmienna), pierwotne, dlugosc);
    *wskaznik += dlugosc + 2;
    return zmienna;
}

Zmienna* wczytujJakoNazwa(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(!isalpha((unsigned char)polozenie[0])) return NULL;
    char* pierwotne = polozenie;
    while(isalpha((unsigned char)polozenie[0])) polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    uintptr_t pojemnosc = nastepnaPotegaDwojki(dlugosc);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(zmienna == NULL) niezbywalnyBlad("Brak pamieci");
    zmienna->cechy = 0;
    zmienna->rod = 0;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rozmiar = dlugosc;
    memcpy((char*)zmienna + sizeof(Zmienna), pierwotne, dlugosc);
    *wskaznik += dlugosc;
    return zmienna;
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

Zmienna* (*konajkiDoZmiennych[])(char**) = {wczytujJakoObszar, wczytujJakoLiczbe, wczytujJakoPismo, wczytujJakoNazwa};

Czastka wczytujNastepne(char** wskaznik)
{
    void* wynik;
    wynik = wczytujKluczowe(wskaznik);
    if(wynik != NULL)
    {
        Czastka c = {1, wynik};
        return c;
    }
    wynik = wczytujDzialania(wskaznik);
    if(wynik != NULL)
    {
        Czastka c = {3, wynik};
        return c;
    }
    for(int i = 0; i < sizeof(konajkiDoZmiennych) / sizeof(konajkiDoZmiennych[0]); i++)
    {
        wynik = konajkiDoZmiennych[i](wskaznik);
        if(wynik != NULL)
        {
            Czastka c = {2, wynik};
            return c;
        }
    }
    Czastka c = {0, *wskaznik};
    return c;
}

void tlumacz()
{
    fseek(zrodlo, 0, SEEK_END);
    size_t rozmiar = ftell(zrodlo);
    rewind(zrodlo);

    poczatek = (char*)malloc(rozmiar + 1);
    if(!poczatek) niezbywalnyBlad("Brak pamieci");
    fread(poczatek, 1, rozmiar, zrodlo);
    char* obecny = poczatek;
    koniec = poczatek + rozmiar;

    Zmienna* zmienna = wczytujJakoObszar(obecny);
}

void czytaj()
{
    
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

    if(dlugosc > 4 && strcmp(argv[1] + dlugosc - 4, ".czp") == 0) przygotowanieDlaCzp();
    else if(dlugosc > 3 && strcmp(argv[1] + dlugosc - 3, ".cz") == 0) przygotowanieDlaCz();
    else
    {
        niezbywalnyBlad("Wymagany pek .czp lub .cz");
    }

    polecenia = (Polecenie*)malloc(sizeof(Polecenie*) * 8);
    if(polecenia == NULL) niezbywalnyBlad("Brak pamieci");
    polecenia[0] = tlumacz;
    polecenia[0] = czytaj;

    return 0;
}