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

typedef struct
{
    unsigned char rodzaj;               // 0 - nieznany, 1 - słowo kluczowe, 2 - zmienna, 3 - działanie
    void* zawartosc;                    // wskaźnik lub odnośnik do danych
} Czastka;

typedef struct
{
    int rodzaj;
    Zmienna* wartosc;
    void* lewa;
    void* prawa;
} GalazPodwojna;

typedef struct {
    size_t rozmiar;
    size_t pojemnosc;
    GalazPodwojna** galezie;
} Rozgalezienie;

char* slowaKluczowe[] = {"jesli", "poki"};
char* dzialania[] = {"+", "-", "*", "/", "="};

Rozgalezienie* robDrzewo(Czastka* cz, size_t l);
void zetnijGalaz(GalazPodwojna pien);

Rozgalezienie* utworzRozgalezienie();
void dodajGalaz(Rozgalezienie* rozgalezienie, GalazPodwojna instr);
void zetnijRozgalezienie(Rozgalezienie* rozgalezienie);