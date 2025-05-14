#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>

#include "powszechne.h"

char** wskaznikPocztu = NULL;
size_t** wskaznikWskaznikaOdnosnikaPolecenia = NULL;
size_t** wskaznikWskaznikaDlugosciPocztu = NULL;
size_t** wskaznikWskaznikaPojemnosciPocztu = NULL;


void __declspec(dllexport) rowne_dz()
{
    printf("Wywolano rowne\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) rozne_dz()
{
    printf("Wywolano rozne\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) wieksze_badz_rowne_dz()
{
    printf("Wywolano wieksze badz rowne\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) mniejsze_badz_rowne_dz()
{
    printf("Wywolano mniejsze badz rowne\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) lub_dz()
{
    printf("Wywolano lub\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) i_dz()
{
    printf("Wywolano i\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) ndodaj_dz()
{
    printf("Wywolano ndodaj\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) nodejmij_dz()
{
    printf("Wywolano nodejmij\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) nmnoz_dz()
{
    printf("Wywolano nmnoz\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) ndziel_dz()
{
    printf("Wywolano ndziel\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) nreszta_dz()
{
    printf("Wywolano nreszta\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) dodaj_dz()
{
    printf("Wywolano dodaj\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) odejmij_dz()
{
    printf("Wywolano odejmij\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) mnoz_dz()
{
    printf("Wywolano mnoz\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) dziel_dz()
{
    printf("Wywolano dzielenie\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) reszta_dz()
{
    printf("Wywolano reszta\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) nadaj_dz()
{
    printf("Wywolano nadaj\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) nie_dz()
{
    printf("Wywolano nie\n");
    *wskaznikPocztu += 2 * sizeof(size_t);
}

void __declspec(dllexport) wieksze_dz()
{
    printf("Wywolano wieksze\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) mniejsze_dz()
{
    printf("Wywolano mniejsze\n");
    *wskaznikPocztu += 3 * sizeof(size_t);
}

void __declspec(dllexport) jesli_klc()
{
    printf("Wywolano jesli\n");
    *wskaznikPocztu += 2 * sizeof(size_t);
}

void __declspec(dllexport) poki_klc()
{
    printf("Wywolano poki\n");
    *wskaznikPocztu += 2 * sizeof(size_t);
}

void __declspec(dllexport) skok_wjk()
{
    printf("Wywolano skok\n");
    *wskaznikPocztu += 1 * sizeof(size_t);
}

// kluczowa zawartość dll

void __declspec(dllexport) dostosuj(SciagnijZmienneKon szk, SciagnijNazwyZmiennychKon snzk, SciagnijPoczetKon spk) // dostosowywuje dll
{
    spk(&wskaznikPocztu, &wskaznikWskaznikaOdnosnikaPolecenia, &wskaznikWskaznikaDlugosciPocztu, &wskaznikWskaznikaPojemnosciPocztu);
}

__declspec(dllexport) char* nazwyPrzenoszonychPolecen[] = { // nazwy poleceń przenoszonych
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

__declspec(dllexport) size_t liczbaPrzenoszonychPolecen = sizeof(nazwyPrzenoszonychPolecen)/sizeof(nazwyPrzenoszonychPolecen[0]); // ich liczba