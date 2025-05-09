#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>

#include "powszechne.h"

void __declspec(dllexport) dodaj_dz()
{
    printf("Wywolano dodawanie\n");
}

void __declspec(dllexport) odejmij_dz()
{
    printf("Wywolano odejmowanie\n");
}

void __declspec(dllexport) mnoz_dz()
{
    printf("Wywolano mnozenie\n");
}

void __declspec(dllexport) dziel_dz()
{
    printf("Wywolano dzielenie\n");
}
void __declspec(dllexport) nadaj_dz()
{
    printf("Wywolano nadawanie\n");
}


// kluczowa zawartość dll

void __declspec(dllexport) dostosuj(SciagnijZmienneKon szk, SciagnijNazwyZmiennychKon snzk) // dostosowywuje dll
{
    
}

__declspec(dllexport) char* nazwyPrzenoszonychPolecen[] = { // nazwy poleceń przenoszonych
    "dodaj_dz",
    "odejmij_dz",
    "mnoz_dz",
    "dziel_dz",
    "nadaj_dz"
};

__declspec(dllexport) size_t liczbaPrzenoszonychPolecen = sizeof(nazwyPrzenoszonychPolecen)/sizeof(nazwyPrzenoszonychPolecen[0]); // ich liczba