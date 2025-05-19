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
size_t* wskaznikOdnosnikaPolecenia = NULL;


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
    printf("Wywolano dodaj\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
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

void __declspec(dllexport) dostosuj(SciagnijObszarPowszechnyKon sopk, SciagnijNazwyZmiennychKon snzk, SciagnijPoczetKon spk) // dostosowywuje dll
{
    //spk(&wskaznikPocztu, &wskaznikWskaznikaOdnosnikaPolecenia, &wskaznikWskaznikaDlugosciPocztu, &wskaznikWskaznikaPojemnosciPocztu);
    spk(&wskaznikPocztu, &wskaznikOdnosnikaPolecenia);
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