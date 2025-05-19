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
    Zmienna** wWZmiennej;
    size_t odnosnikNazwy;
} Ozin; // odnosnik zmiennej i nazwy

typedef struct
{
    Zmienna** wWZmiennej;
    size_t liczba;
} ZmiennaILiczba;

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


//typedef void (*SciagnijZmienneKon)(Zmienna***, size_t*, size_t*);
typedef void (*SciagnijObszarPowszechnyKon)(Zmienna****);
typedef void (*SciagnijNazwyZmiennychKon)(char***, size_t*, size_t*);
typedef void (*SciagnijPoczetKon)(char***, size_t**);

typedef void (*DostosujKon)(SciagnijObszarPowszechnyKon, SciagnijNazwyZmiennychKon, SciagnijPoczetKon);


static inline char* zawartosc(Zmienna* zmienna)
{
    return (char*)zmienna + sizeof(Zmienna);
}

static inline Ozin* cechy(Zmienna* zmienna)
{
    return (Ozin*)((char*)zmienna + sizeof(Zmienna) + zmienna->pojemnosc);
}

static inline Zmienna** pozyskajZmiennaZObszaru(Zmienna* obszar, size_t odnosnik)
{
    return ((Ozin*)((char*)obszar + sizeof(Zmienna) + odnosnik * sizeof(Ozin)))->wWZmiennej;
}

static inline size_t pozyskajOdnosnikNazwyZObszaru(Zmienna* obszar, size_t odnosnik)
{
    return ((Ozin*)((char*)obszar + sizeof(Zmienna) + odnosnik * sizeof(Ozin)))->odnosnikNazwy;
}

#ifdef __cplusplus
}
#endif

#endif