#ifndef POWSZECHNE_H
#define POWSZECHNE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>

typedef void (*Polecenie)();

typedef struct
{
    size_t rozmiar;                     // liczba bytów używana przez zawartość
    size_t rod;                         // ród, wskazuje na inną zmienną która jest zmienną rodową lub na siebie jeśli sama nią jest
    size_t cechy;                       // liczba cech (zmiennych przypisanych do tej zmiennej)
    size_t pojemnosc;                   // ilość przypisanej pamięci
} Zmienna;

typedef struct
{
    size_t odnosnik;
    size_t nazwa;
} Ozin; // odnosnik zmiennej i nazwy

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


typedef void (*SciagnijZmienneKon)(Zmienna***, size_t*, size_t*);
typedef void (*SciagnijNazwyZmiennychKon)(char***, size_t*, size_t*);

typedef void (*DostosujKon)(SciagnijZmienneKon, SciagnijNazwyZmiennychKon);


#ifdef __cplusplus
}
#endif

#endif