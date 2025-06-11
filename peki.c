#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>

#include "powszechne.h"

char*** wskaznikNazwZmiennych;

Ozin** wskaznikOzinow = NULL;

unsigned char** wskaznikPocztu = NULL;
size_t* wskaznikOdnosnikaPolecenia = NULL;

char*** wskaznikNazwPolecen = NULL;
size_t** wskaznikDlugosciWywodowPolecen = NULL;
void (*dodajZawartoscDoZmiennej)(Zmienna**, size_t, void*) = NULL;
void (*ustawZawartoscZmiennej)(Zmienna**, size_t, void*) = NULL;
Zmienna** (*utworzOdbicieZmiennej)(Zmienna**) = NULL;

void (*przelaczNaPoczet)(Zmienna**) = NULL;
void (*przelaczNaObszar)(Zmienna**) = NULL;
void (*zejdz)() = NULL;

void (*zwolnijPamiecZMpz)(void *, size_t);


void __declspec(dllexport) pobierz_zawartosc_z_peku_pek()
{
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    size_t r = (*z1)->rozmiar;
    char* p = malloc(r + 1);
    memcpy(p, zawartosc(*z1), r);
    p[r] = '\0';
    
    FILE* pek = fopen(p, "rb");
    if(!pek) exit(1);
    fseek(pek, 0, SEEK_END);
    long rozmiar = ftell(pek);
    if (rozmiar < 0)
    {
        fclose(pek);
        exit(1);
    }
    rewind(pek);
    char* tymaczsowy = malloc(rozmiar);
    if(!tymaczsowy) exit(1);
    size_t wczytano = fread(tymaczsowy, 1, rozmiar, pek);
    if(wczytano != rozmiar) exit(1);
    ustawZawartoscZmiennej(wynik, rozmiar, tymaczsowy);
    (*wynik)->rod = (*z1)->rod;
    free(tymaczsowy);
    fclose(pek);
    free(p);
    *wskaznikOdnosnikaPolecenia += 2 * sizeof(size_t);
}


void __declspec(dllexport) wpisz_zawartosc_w_pek_pek()
{
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;
    size_t r = (*z1)->rozmiar;
    char* p = malloc(r + 1);
    memcpy(p, zawartosc(*z1), r);
    p[r] = '\0';
    
    FILE* pek = fopen(p, "wb");
    if(!pek) exit(1);
    
    size_t wpisano = fwrite(zawartosc(*z2), 1, (*z2)->rozmiar, pek);
    if(wpisano == (*z2)->rozmiar)
    {
        unsigned char znak = 1;
        ustawZawartoscZmiennej(wynik, sizeof(znak), &znak);
    }
    else
    {
        unsigned char znak = 0;
        ustawZawartoscZmiennej(wynik, sizeof(znak), &znak);
    }
    fclose(pek);
    free(p);
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}


void __declspec(dllexport) dostosuj(SciagnijObszaryKon sok, SciagnijNazwyZmiennychKon snzk, SciagnijPoczetKon spk, SciagnijPoleceniaKon spok, SciagnijZarzadzanieZmiennymiKon szzk, SciagnijZarzadzanieStosemWyjscKon szswk) // dostosowywuje dll
{
    //spk(&wskaznikPocztu, &wskaznikWskaznikaOdnosnikaPolecenia, &wskaznikWskaznikaDlugosciPocztu, &wskaznikWskaznikaPojemnosciPocztu);
    sok(NULL, NULL, NULL, &wskaznikOzinow);
    snzk(&wskaznikNazwZmiennych, NULL, NULL);
    spk(NULL, &wskaznikPocztu, &wskaznikOdnosnikaPolecenia);
    spok(NULL, &wskaznikNazwPolecen, &wskaznikDlugosciWywodowPolecen, NULL, NULL);
    szzk(&dodajZawartoscDoZmiennej, &ustawZawartoscZmiennej, &utworzOdbicieZmiennej);
    szswk(&przelaczNaPoczet, &przelaczNaObszar, &zejdz);
    mp_get_memory_functions(NULL, NULL, &zwolnijPamiecZMpz);
}

// nazwy poleceń przenoszonych
__declspec(dllexport) char* nazwyPrzenoszonychPolecen[] = {
    "pobierz_zawartosc_z_peku_pek",
    "wpisz_zawartosc_w_pek_pek",
};

// ilość bajtów jaką wymaga każde polecenie (nie licząc własnego odnośnika)
__declspec(dllexport) size_t dlugosciWywodowPrzenoszonychPolecen[] = {
    2 * sizeof(size_t), // "pobierz_zawartosc_z_peku_pek",
    3 * sizeof(size_t), // "wpisz_zawartosc_w_pek_pek",
};

__declspec(dllexport) size_t liczbaPrzenoszonychPolecen = sizeof(nazwyPrzenoszonychPolecen)/sizeof(nazwyPrzenoszonychPolecen[0]); // ich liczba