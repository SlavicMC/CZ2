#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>

#include "powszechne.h"

Ozin** wskaznikOzinow = NULL;

char** wskaznikPocztu = NULL;
size_t* wskaznikOdnosnikaPolecenia = NULL;

void (*ustawZawartoscZmiennej)(Zmienna**, size_t, void*) = NULL;

void (*zwolnijPamiecZMpz)(void *, size_t);

void __declspec(dllexport) rowne_dz()
{
    printf("Wywolano rowne\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) rozne_dz()
{
    printf("Wywolano rozne\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) wieksze_badz_rowne_dz()
{
    printf("Wywolano wieksze badz rowne\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) mniejsze_badz_rowne_dz()
{
    printf("Wywolano mniejsze badz rowne\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) lub_dz()
{
    printf("Wywolano lub\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) i_dz()
{
    printf("Wywolano i\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) ndodaj_dz()
{
    printf("Wywolano ndodaj\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nodejmij_dz()
{
    printf("Wywolano nodejmij\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nmnoz_dz()
{
    printf("Wywolano nmnoz\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) ndziel_dz()
{
    printf("Wywolano ndziel\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nreszta_dz()
{
    printf("Wywolano nreszta\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) dodaj_dz()
{
    // Log (tymczasowy)
    printf("Wywolano dodaj\n");
    // Pozyskujemy wskaźnik do wartości z pocztu i rzutujemy je na size_t
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    // Wyciągamy zmienne z ozinów
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;
    // Przekształcamy je na mpz do obliczeń
    mpz_t wynik_mpz, z1_mpz, z2_mpz;
    mpz_init(wynik_mpz);
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);
    // Wpisujemy wartości liczb ze zmiennych
    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));
    // Wykonujemy działanie
    mpz_add(wynik_mpz, z1_mpz, z2_mpz);
    // Log (tymczasowy)
    char* str1 = mpz_get_str(NULL, 10, z1_mpz);
    char* str2 = mpz_get_str(NULL, 10, z2_mpz);
    char* str3 = mpz_get_str(NULL, 10, wynik_mpz);
    printf("Z1: %s\nZ2: %s\nWynik: %s\n", str1, str2, str3);
    zwolnijPamiecZMpz(str1, strlen(str1) + 1);
    zwolnijPamiecZMpz(str2, strlen(str2) + 1);
    zwolnijPamiecZMpz(str3, strlen(str3) + 1);
    // Ustawiamy zawartość wyniku
    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, wynik_mpz);
    ustawZawartoscZmiennej(wynik, rozmiar, surowa);
    printf("Ustawiono zawartosc\n");
    // Zwalnianie pamięci
    zwolnijPamiecZMpz(surowa, rozmiar);
    mpz_clear(wynik_mpz);
    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);
    // Przesuwamy poczet o 3 * size_t
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
    // Log (dla upewnienia się że zapisano dane poprawnie, tymczasowy)
    mpz_t log;
    mpz_init(log);
    mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
    char* str = mpz_get_str(NULL, 10, log);
    printf("Pomyslnie zapisano wynik: %s\n", str);
    zwolnijPamiecZMpz(str, strlen(str) + 1);
    mpz_clear(log);
}

void __declspec(dllexport) odejmij_dz()
{
    printf("Wywolano odejmij\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) mnoz_dz()
{
    printf("Wywolano mnoz\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) dziel_dz()
{
    printf("Wywolano dzielenie\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) reszta_dz()
{
    printf("Wywolano reszta\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nadaj_dz()
{
    printf("Wywolano nadaj\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nie_dz()
{
    printf("Wywolano nie\n");
    *wskaznikOdnosnikaPolecenia += 2 * sizeof(size_t);
}

void __declspec(dllexport) wieksze_dz()
{
    printf("Wywolano wieksze\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) mniejsze_dz()
{
    printf("Wywolano mniejsze\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) jesli_klc()
{
    printf("Wywolano jesli\n");
    *wskaznikOdnosnikaPolecenia += 2 * sizeof(size_t);
}

void __declspec(dllexport) poki_klc()
{
    printf("Wywolano poki\n");
    *wskaznikOdnosnikaPolecenia += 2 * sizeof(size_t);
}

void __declspec(dllexport) skok_wjk()
{
    printf("Wywolano skok\n");
    *wskaznikOdnosnikaPolecenia += 1 * sizeof(size_t);
}

// kluczowa zawartość dll

void __declspec(dllexport) dostosuj(SciagnijObszaryKon sok, SciagnijNazwyZmiennychKon snzk, SciagnijPoczetKon spk, SciagnijZarzadzanieZmiennymiKon szzk) // dostosowywuje dll
{
    //spk(&wskaznikPocztu, &wskaznikWskaznikaOdnosnikaPolecenia, &wskaznikWskaznikaDlugosciPocztu, &wskaznikWskaznikaPojemnosciPocztu);
    sok(NULL, NULL, NULL, &wskaznikOzinow);
    spk(NULL, &wskaznikPocztu, &wskaznikOdnosnikaPolecenia);
    szzk(NULL, &ustawZawartoscZmiennej);
    mp_get_memory_functions(NULL, NULL, &zwolnijPamiecZMpz);
}

// nazwy poleceń przenoszonych
__declspec(dllexport) char* nazwyPrzenoszonychPolecen[] = {
    "rowne_dz",
    "rozne_dz",
    "wieksze_badz_rowne_dz",
    "mniejsze_badz_rowne_dz",
    "lub_dz",
    "i_dz",
    "ndodaj_dz",
    "nodejmij_dz",
    "nmnoz_dz",
    "ndziel_dz",
    "nreszta_dz",
    "dodaj_dz",
    "odejmij_dz",
    "mnoz_dz",
    "dziel_dz",
    "reszta_dz",
    "nadaj_dz",
    "nie_dz",
    "wieksze_dz",
    "mniejsze_dz",

    "jesli_klc",
    "poki_klc",

    "skok_wjk"
};

// ilość bajtów jaką wymaga każde polecenie (nie licząc własnego odnośnika)
__declspec(dllexport) size_t dlugosciWywodowPrzenoszonychPolecen[] = {
    3 * sizeof(size_t), // "rowne_dz",
    3 * sizeof(size_t), // "rozne_dz",
    3 * sizeof(size_t), // "wieksze_badz_rowne_dz",
    3 * sizeof(size_t), // "mniejsze_badz_rowne_dz",
    3 * sizeof(size_t), // "lub_dz",
    3 * sizeof(size_t), // "i_dz",
    3 * sizeof(size_t), // "ndodaj_dz",
    3 * sizeof(size_t), // "nodejmij_dz",
    3 * sizeof(size_t), // "nmnoz_dz",
    3 * sizeof(size_t), // "ndziel_dz",
    3 * sizeof(size_t), // "nreszta_dz",
    3 * sizeof(size_t), // "dodaj_dz",
    3 * sizeof(size_t), // "odejmij_dz",
    3 * sizeof(size_t), // "mnoz_dz",
    3 * sizeof(size_t), // "dziel_dz",
    3 * sizeof(size_t), // "reszta_dz",
    3 * sizeof(size_t), // "nadaj_dz",
    2 * sizeof(size_t), // "nie_dz",
    3 * sizeof(size_t), // "wieksze_dz",
    3 * sizeof(size_t), // "mniejsze_dz",

    2 * sizeof(size_t), // "jesli_klc",
    2 * sizeof(size_t), // "poki_klc",

    1 * sizeof(size_t), // "skok_wjk"
};

__declspec(dllexport) size_t liczbaPrzenoszonychPolecen = sizeof(nazwyPrzenoszonychPolecen)/sizeof(nazwyPrzenoszonychPolecen[0]); // ich liczba