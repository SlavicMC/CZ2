#ifndef DZIELONE_H
#define DZIELONE_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>

typedef struct
{
    size_t rozmiar;                     // liczba bytów używana przez zawartość
    size_t rod;                         // ród, wskazuje na inną zmienną która jest zmienną rodową lub na siebie jeśli sama nią jest
    size_t cechy;                       // liczba cech (zmiennych przypisanych do tej zmiennej)
    size_t pojemnosc;                   // ilość przypisanej pamięci
} Zmienna;

typedef struct {
    size_t odnosnik;
    char* nazwa;
} OdnosnikZmiennejINazwa;

typedef struct
{
    unsigned char rodzaj;               // 0 - nieznany, 1 - słowo kluczowe, 2 - zmienna, 3 - działanie
    uintptr_t zawartosc;                // wskaźnik lub odnośnik do danych
} Czastka;

typedef struct
{
    int rodzaj;
    size_t wartosc;
    void* lewa;
    void* prawa;
} GalazPodwojna;

typedef struct {
    size_t rozmiar;
    size_t pojemnosc;
    GalazPodwojna** galezie;
} Rozgalezienie;

extern char* slowaKluczowe[];
extern char* dzialania[];

Rozgalezienie* robDrzewo(Czastka* cz, size_t l);
void zetnijGalaz(GalazPodwojna* galaz);

Rozgalezienie* utworzRozgalezienie();
Rozgalezienie* pojedynczeRozgalezienie(GalazPodwojna* galaz);
void dodajGalaz(Rozgalezienie* rozgalezienie, GalazPodwojna* galaz);
void zetnijRozgalezienie(Rozgalezienie* rozgalezienie);

const char* nazwaRodzaju(int rodzaj);
void wypiszGalaz(GalazPodwojna* galaz, int wciecie);
void wypiszDrzewo(Rozgalezienie* rozgalezienie, int wciecie);

GalazPodwojna* utworzDzialanie(int dz, GalazPodwojna* lewy, GalazPodwojna* prawy);
GalazPodwojna* utworzWyrazenieKluczowe(int dz, GalazPodwojna* lewy, Rozgalezienie* prawy);
GalazPodwojna* utworzZmiennaJakoGalaz(size_t z);

#endif