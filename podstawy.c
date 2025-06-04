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

void __declspec(dllexport) rowne_dz()
{
    //printf("Wywolano rowne\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    unsigned char wynik_bool = (mpz_cmp(z1_mpz, z2_mpz) == 0) ? 1 : 0;

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5; 
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) rozne_dz()
{
    //printf("Wywolano rozne\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    unsigned char wynik_bool = (mpz_cmp(z1_mpz, z2_mpz) != 0) ? 1 : 0;

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5; 
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) wieksze_badz_rowne_dz()
{
    //printf("Wywolano wieksze badz rowne\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    unsigned char wynik_bool = (mpz_cmp(z1_mpz, z2_mpz) >= 0) ? 1 : 0;

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5;
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) mniejsze_badz_rowne_dz()
{
    //printf("Wywolano wieksze badz rowne\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    unsigned char wynik_bool = (mpz_cmp(z1_mpz, z2_mpz) <= 0) ? 1 : 0;

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5;
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) lub_dz()
{
    //printf("Wywolano lub\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    unsigned char wynik_bool = (*(zawartosc(*z1))) || (*(zawartosc(*z2)));

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5; 
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) i_dz()
{
    //printf("Wywolano i\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    unsigned char wynik_bool = (*(zawartosc(*z1))) && (*(zawartosc(*z2)));

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5; 
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) ndodaj_dz()
{
    //printf("Wywolano ndodaj\n");

    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);

    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    /*char* str1 = mpz_get_str(NULL, 10, z1_mpz);
    char* str2 = mpz_get_str(NULL, 10, z2_mpz);
    printf("Z1: %s\nZ2: %s\n", str1, str2);
    zwolnijPamiecZMpz(str1, strlen(str1) + 1);
    zwolnijPamiecZMpz(str2, strlen(str2) + 1);*/

    mpz_add(z1_mpz, z1_mpz, z2_mpz);

    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, z1_mpz);
    ustawZawartoscZmiennej(z1, rozmiar, surowa);
    ustawZawartoscZmiennej(wynik, rozmiar, surowa);
    (*wynik)->rod = (*z1)->rod;
    //printf("Ustawiono zawartosc\n");

    zwolnijPamiecZMpz(surowa, rozmiar);
    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    /*mpz_t log;
    mpz_init(log);
    mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
    char* str = mpz_get_str(NULL, 10, log);
    printf("Pomyslnie zapisano wynik: %s\n", str);
    zwolnijPamiecZMpz(str, strlen(str) + 1);
    mpz_clear(log);*/
}

void __declspec(dllexport) nodejmij_dz()
{
    //printf("Wywolano nodejmij\n");

    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);

    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    /*char* str1 = mpz_get_str(NULL, 10, z1_mpz);
    char* str2 = mpz_get_str(NULL, 10, z2_mpz);
    printf("Z1: %s\nZ2: %s\n", str1, str2);
    zwolnijPamiecZMpz(str1, strlen(str1) + 1);
    zwolnijPamiecZMpz(str2, strlen(str2) + 1);*/

    mpz_sub(z1_mpz, z1_mpz, z2_mpz);

    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, z1_mpz);
    ustawZawartoscZmiennej(z1, rozmiar, surowa);
    ustawZawartoscZmiennej(wynik, rozmiar, surowa);
    (*wynik)->rod = (*z1)->rod;
    //printf("Ustawiono zawartosc\n");

    zwolnijPamiecZMpz(surowa, rozmiar);
    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    /*mpz_t log;
    mpz_init(log);
    mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
    char* str = mpz_get_str(NULL, 10, log);
    printf("Pomyslnie zapisano wynik: %s\n", str);
    zwolnijPamiecZMpz(str, strlen(str) + 1);
    mpz_clear(log);*/
}

void __declspec(dllexport) nmnoz_dz()
{
    //printf("Wywolano nmnoz\n");

    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);

    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    /*char* str1 = mpz_get_str(NULL, 10, z1_mpz);
    char* str2 = mpz_get_str(NULL, 10, z2_mpz);
    printf("Z1: %s\nZ2: %s\n", str1, str2);
    zwolnijPamiecZMpz(str1, strlen(str1) + 1);
    zwolnijPamiecZMpz(str2, strlen(str2) + 1);*/

    mpz_mul(z1_mpz, z1_mpz, z2_mpz);

    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, z1_mpz);
    ustawZawartoscZmiennej(z1, rozmiar, surowa);
    ustawZawartoscZmiennej(wynik, rozmiar, surowa);
    (*wynik)->rod = (*z1)->rod;
    //printf("Ustawiono zawartosc\n");

    zwolnijPamiecZMpz(surowa, rozmiar);
    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    /*mpz_t log;
    mpz_init(log);
    mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
    char* str = mpz_get_str(NULL, 10, log);
    printf("Pomyslnie zapisano wynik: %s\n", str);
    zwolnijPamiecZMpz(str, strlen(str) + 1);
    mpz_clear(log);*/
}

void __declspec(dllexport) ndziel_dz()
{
    //printf("Wywolano ndziel\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nreszta_dz()
{
    //printf("Wywolano nreszta\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) dodaj_dz()
{
    // Log (tymczasowy)
    //printf("Wywolano dodaj\n");
    // Pozyskujemy wskaźnik do wartości z pocztu i rzutujemy je na size_t
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    // Wyciągamy zmienne z ozinów
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;
    size_t rod1 = (*z1)->rod;
    size_t rod2 = (*z2)->rod;
    if(rod1 == 3 && rod2 == 3)
    {
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
        /*// Log (tymczasowy)
        char* str1 = mpz_get_str(NULL, 10, z1_mpz);
        char* str2 = mpz_get_str(NULL, 10, z2_mpz);
        char* str3 = mpz_get_str(NULL, 10, wynik_mpz);
        printf("Z1: %s\nZ2: %s\nWynik: %s\n", str1, str2, str3);
        zwolnijPamiecZMpz(str1, strlen(str1) + 1);
        zwolnijPamiecZMpz(str2, strlen(str2) + 1);
        zwolnijPamiecZMpz(str3, strlen(str3) + 1);*/
        // Ustawiamy zawartość wyniku
        size_t rozmiar;
        void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, wynik_mpz);
        ustawZawartoscZmiennej(wynik, rozmiar, surowa);
        (*wynik)->rod = (*z1)->rod;
        //printf("Ustawiono zawartosc\n");
        // Zwalnianie pamięci
        zwolnijPamiecZMpz(surowa, rozmiar);
        mpz_clear(wynik_mpz);
        mpz_clear(z1_mpz);
        mpz_clear(z2_mpz);
        /*// Log (dla upewnienia się że zapisano dane poprawnie, tymczasowy)
        mpz_t log;
        mpz_init(log);
        mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
        char* str = mpz_get_str(NULL, 10, log);
        printf("Pomyslnie zapisano wynik: %s\n", str);
        zwolnijPamiecZMpz(str, strlen(str) + 1);
        mpz_clear(log);*/
    }
    else if(rod1 == 4 && rod2 == 4)
    {
        ustawZawartoscZmiennej(wynik, (*z1)->rozmiar, zawartosc(*z1));
        dodajZawartoscDoZmiennej(wynik, (*z2)->rozmiar, zawartosc(*z2));
        (*wynik)->rod = rod1;
    }
    // Przesuwamy poczet o 3 * size_t
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) odejmij_dz()
{
    printf("Wywolano odejmij\n");

    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t wynik_mpz, z1_mpz, z2_mpz;
    mpz_init(wynik_mpz);
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    mpz_sub(wynik_mpz, z1_mpz, z2_mpz);

    char* str1 = mpz_get_str(NULL, 10, z1_mpz);
    char* str2 = mpz_get_str(NULL, 10, z2_mpz);
    char* str3 = mpz_get_str(NULL, 10, wynik_mpz);
    printf("Z1: %s\nZ2: %s\nWynik: %s\n", str1, str2, str3);
    zwolnijPamiecZMpz(str1, strlen(str1) + 1);
    zwolnijPamiecZMpz(str2, strlen(str2) + 1);
    zwolnijPamiecZMpz(str3, strlen(str3) + 1);

    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, wynik_mpz);
    ustawZawartoscZmiennej(wynik, rozmiar, surowa);
    (*wynik)->rod = (*z1)->rod;  
    printf("Ustawiono zawartosc\n");

    zwolnijPamiecZMpz(surowa, rozmiar);
    mpz_clear(wynik_mpz);
    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    mpz_t log;
    mpz_init(log);
    mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
    char* str = mpz_get_str(NULL, 10, log);
    printf("Pomyslnie zapisano wynik: %s\n", str);
    zwolnijPamiecZMpz(str, strlen(str) + 1);
    mpz_clear(log);
}

void __declspec(dllexport) mnoz_dz()
{
    //printf("Wywolano mnoz\n");
    //*wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    //printf("Wywolano mnoz\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t wynik_mpz, z1_mpz, z2_mpz;
    mpz_init(wynik_mpz);
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    mpz_mul(wynik_mpz, z1_mpz, z2_mpz);

    /*char* str1 = mpz_get_str(NULL, 10, z1_mpz);
    char* str2 = mpz_get_str(NULL, 10, z2_mpz);
    char* str3 = mpz_get_str(NULL, 10, wynik_mpz);
    printf("Z1: %s\nZ2: %s\nWynik: %s\n", str1, str2, str3);
    zwolnijPamiecZMpz(str1, strlen(str1) + 1);
    zwolnijPamiecZMpz(str2, strlen(str2) + 1);
    zwolnijPamiecZMpz(str3, strlen(str3) + 1);*/

    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, wynik_mpz);
    ustawZawartoscZmiennej(wynik, rozmiar, surowa);
    (*wynik)->rod = (*z1)->rod;
    //printf("Ustawiono zawartosc\n");

    zwolnijPamiecZMpz(surowa, rozmiar);
    mpz_clear(wynik_mpz);
    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    /*mpz_t log;
    mpz_init(log);
    mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
    char* str = mpz_get_str(NULL, 10, log);
    printf("Pomyslnie zapisano wynik: %s\n", str);
    zwolnijPamiecZMpz(str, strlen(str) + 1);
    mpz_clear(log);*/
}

void __declspec(dllexport) dziel_dz()
{
    //printf("Wywolano dzielenie\n");
    //*wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    printf("Wywolano dziel\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t wynik_mpz, z1_mpz, z2_mpz;
    mpz_init(wynik_mpz);
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    //if(mpz_cmp_ui(z2_mpz, 0) == 0) {
    //    niezbywalnyBlad("Dzielenie przez zero");
    //}
    mpz_tdiv_q(wynik_mpz, z1_mpz, z2_mpz);

    char* str1 = mpz_get_str(NULL, 10, z1_mpz);
    char* str2 = mpz_get_str(NULL, 10, z2_mpz);
    char* str3 = mpz_get_str(NULL, 10, wynik_mpz);
    printf("Z1: %s\nZ2: %s\nWynik: %s\n", str1, str2, str3);
    zwolnijPamiecZMpz(str1, strlen(str1) + 1);
    zwolnijPamiecZMpz(str2, strlen(str2) + 1);
    zwolnijPamiecZMpz(str3, strlen(str3) + 1);

    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, wynik_mpz);
    ustawZawartoscZmiennej(wynik, rozmiar, surowa);
    (*wynik)->rod = (*z1)->rod;
    printf("Ustawiono zawartosc\n");

    zwolnijPamiecZMpz(surowa, rozmiar);
    mpz_clear(wynik_mpz);
    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);

    mpz_t log;
    mpz_init(log);
    mpz_import(log, (*wynik)->rozmiar, 1, 1, 0, 0, zawartosc(*wynik));
    char* str = mpz_get_str(NULL, 10, log);
    printf("Pomyslnie zapisano wynik: %s\n", str);
    zwolnijPamiecZMpz(str, strlen(str) + 1);
    mpz_clear(log);
}

void __declspec(dllexport) reszta_dz()
{
    printf("Wywolano reszta\n");
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nadaj_dz()
{
    //printf("Wywolano nadaj\n");

    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);

    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    //printf("Nadano do %p\n", z1);

    ustawZawartoscZmiennej(z1, (*z2)->rozmiar, zawartosc(*z2));
    (*z1)->rod = (*z2)->rod;
    ustawZawartoscZmiennej(wynik, (*z2)->rozmiar, zawartosc(*z2));
    (*wynik)->rod = (*z2)->rod;
    
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) nie_dz()
{
    //printf("Wywolano nie\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;

    unsigned char wynik_bool = !(*(zawartosc(*z1)));

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5;
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    *wskaznikOdnosnikaPolecenia += 2 * sizeof(size_t);
}

void __declspec(dllexport) wieksze_dz()
{
    //printf("Wywolano wieksze\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    unsigned char wynik_bool = (mpz_cmp(z1_mpz, z2_mpz) > 0) ? 1 : 0;

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5;
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) mniejsze_dz()
{
    //printf("Wywolano mniejsze\n");
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;

    mpz_t z1_mpz, z2_mpz;
    mpz_init(z1_mpz);
    mpz_init(z2_mpz);

    mpz_import(z1_mpz, (*z1)->rozmiar, 1, 1, 0, 0, zawartosc(*z1));
    mpz_import(z2_mpz, (*z2)->rozmiar, 1, 1, 0, 0, zawartosc(*z2));

    unsigned char wynik_bool = (mpz_cmp(z1_mpz, z2_mpz) < 0) ? 1 : 0;

    ustawZawartoscZmiennej(wynik, sizeof(wynik_bool), &wynik_bool);
    (*wynik)->rod = 5;
    //printf("Ustawiono zawartosc: %s\n", (*(zawartosc(*wynik))) ? "tak" : "nie");

    mpz_clear(z1_mpz);
    mpz_clear(z2_mpz);

    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) kropka_dz()
{
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    
    Zmienna** wynik = oziny[wartosci[0]].wWZmiennej;
    Zmienna** z1 = oziny[wartosci[1]].wWZmiennej;
    //Zmienna** z2 = oziny[wartosci[2]].wWZmiennej;
    size_t oNZ2 = oziny[wartosci[2]].odnosnikNazwy;

    char** nazwyZmiennych = *wskaznikNazwZmiennych;
    //printf("Szukam: %s\n", nazwyZmiennych[oNZ2]);

    if((*z1)->rod == 1)
    {
        size_t r = (*z1)->rozmiar / sizeof(Ozin);
        Ozin* ozinyZ1 = (Ozin*)zawartosc(*z1);
        for(size_t i = 1; i < r; i++)
        {
            //printf("Znaleziona nazwa: %s\n", nazwyZmiennych[ozinyZ1[i].odnosnikNazwy]);
            if(ozinyZ1[i].odnosnikNazwy == oNZ2)
            {
                Zmienna** znaleziona = ozinyZ1[i].wWZmiennej;
                //printf("Znaleziono!\n");
                ustawZawartoscZmiennej(wynik, (*znaleziona)->rozmiar, zawartosc(*znaleziona));
                (*wynik)->rod = (*znaleziona)->rod;
                break;
            }
        }
    }
    *wskaznikOdnosnikaPolecenia += 3 * sizeof(size_t);
}

void __declspec(dllexport) wywolaj_dz()
{
    Ozin* oziny = *wskaznikOzinow;
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    size_t liczbaWartosci = wartosci[0] / sizeof(size_t);
    if(liczbaWartosci < 2) return;
    Zmienna** wynik; // = oziny[wartosci[1]].wWZmiennej;
    Zmienna** wywolywana = oziny[wartosci[2]].wWZmiennej;
    if((*wywolywana)->rod != 1) return;
    Ozin* ozinyWywolywanej = (Ozin*)zawartosc(*wywolywana);
    liczbaWartosci -= 2;
    if(liczbaWartosci > (*ozinyWywolywanej).odnosnikNazwy) return;
    Zmienna** odbicie = utworzOdbicieZmiennej(wywolywana);
    wynik = oziny[wartosci[1]].wWZmiennej = odbicie;
    Ozin* ozinyOdbicia = (Ozin*)zawartosc(*odbicie);
    char** nazwyZmiennych = *wskaznikNazwZmiennych;
    for(size_t i = 1; i <= liczbaWartosci; i++)
    {
        ozinyOdbicia[i].wWZmiennej = utworzOdbicieZmiennej(oziny[wartosci[i + 2]].wWZmiennej);
    }
    size_t r = (*odbicie)->rozmiar / sizeof(Ozin);
    for(size_t i = liczbaWartosci + 1; i < r; i++)
    {
        char* nazwa = nazwyZmiennych[ozinyOdbicia[i].odnosnikNazwy];
        if(strcmp(nazwa, "zwrot") == 0)
        {
            ozinyOdbicia[i].wWZmiennej = wynik;
            //printf("Zwrot %zu\n", i);
        }
        else if(*nazwa != '*') 
        {
            ozinyOdbicia[i].wWZmiennej = utworzOdbicieZmiennej(ozinyOdbicia[i].wWZmiennej);
            //printf("Kopia %zu\n", i);
        }
        //else printf("Zasiegniecie %zu\n", i);
    }
    *wskaznikOdnosnikaPolecenia += wartosci[0] + sizeof(size_t);
    przelaczNaObszar(odbicie);
    //printf("Wywolano!\n");
}

void __declspec(dllexport) opusc_wjk()
{
    zejdz();
}

void __declspec(dllexport) jesli_klc()
{
    //printf("Wywolano jesli\n");
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    Zmienna** z = (*wskaznikOzinow)[wartosci[0]].wWZmiennej;
    //printf("Wartosc skoku: %zu\n", wartosci[1]);
    if(!(*(zawartosc(*z)))) *wskaznikOdnosnikaPolecenia = wartosci[1];
    else *wskaznikOdnosnikaPolecenia += 2 * sizeof(size_t);
}

void __declspec(dllexport) poki_klc()
{
    //printf("Wywolano poki\n");
    size_t* wartosci = (size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia);
    Zmienna** z = (*wskaznikOzinow)[wartosci[0]].wWZmiennej;
    //printf("Wartosc skoku: %zu\n", wartosci[1]);
    if(!(*(zawartosc(*z)))) *wskaznikOdnosnikaPolecenia = wartosci[1];
    else *wskaznikOdnosnikaPolecenia += 2 * sizeof(size_t);
}

void __declspec(dllexport) skok_wjk()
{
    //printf("Wywolano skok\n");
    //printf("Skok na miejsce %zu\n", *((size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia)));
    *wskaznikOdnosnikaPolecenia = *((size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia));
    //*wskaznikOdnosnikaPolecenia += 1 * sizeof(size_t);
}

void __declspec(dllexport) wypisz_prs()
{
    //printf("Wywolano wypisanie\n");
    
    Zmienna** z = (*wskaznikOzinow)[((size_t*)(*wskaznikPocztu + *wskaznikOdnosnikaPolecenia))[0]].wWZmiennej;
    size_t rod = (*z)->rod;
    if(rod == 0) printf("Nic\n");
    else if(rod == 1)
    {
        printf("Obszar(");
        Ozin* oziny = (Ozin*)zawartosc(*z);
        size_t l = oziny->odnosnikNazwy;
        char** nazwyZmiennych = *wskaznikNazwZmiennych;
        if(l)
        {
            printf("%s", nazwyZmiennych[oziny[1].odnosnikNazwy]);
            for(size_t i = 2; i <= l; i++) printf(", %s", nazwyZmiennych[oziny[i].odnosnikNazwy]);
        }
        printf(")\n");
        if(oziny->wWZmiennej)
        {
            z = oziny->wWZmiennej;
            printf("{");
            char** nazwyPolecen = *wskaznikNazwPolecen;
            size_t* dlugosciWywodowPolecen = *wskaznikDlugosciWywodowPolecen;
            size_t r = (*z)->rozmiar;
            size_t i = sizeof(Zmienna**); // pomijamy pierwszy wskaźnik
            char* zaw = zawartosc(*z);
            size_t wywod = 0;
            while(i < r)
            {
                if(wywod)
                {
                    printf(" %zu",*((size_t*)(zaw + i)));
                    i += sizeof(size_t);
                    wywod -= sizeof(size_t);
                }
                else
                {
                    unsigned char znak = *(zaw + i++);
                    printf("\n   %s", nazwyPolecen[znak]);
                    wywod = dlugosciWywodowPolecen[znak];
                    if(wywod == SIZE_MAX)
                    {
                        wywod = *((size_t*)(zaw + i));
                        printf(" %zu", wywod);
                        i += sizeof(wywod);
                    }
                }
            }

            printf("\n}\n");
        }
    }
    else if(rod == 2)
    {
        printf("Poczet\n{\n");
        /*char** nazwyPolecen = *wskaznikNazwPolecen;
        size_t* dlugosciWywodowPolecen = *wskaznikDlugosciWywodowPolecen;
        char* zaw = zawartosc(*z);
        size_t dlugosc = (*z)->rozmiar;
        size_t i = sizeof(Zmienna**);
        while(i < dlugosc)
        {
            printf("\t%s", nazwyPolecen[*(zaw + i++)]);
            size_t* wartosci = (size_t*)(zaw + i);
            size_t l = dlugosciWywodowPolecen[*(zaw + i)] / sizeof(size_t);
            if(l == SIZE_MAX)
            {
                l = *wartosci;
                wartosci += 1;
                i += sizeof(size_t);
                printf(" %zu", l);
            }
            for(size_t j = 0; j < l; j++)
            {
                printf(" %zu", wartosci[j]);
                i += sizeof(size_t);
            }

            printf("\n");
        }*/

        printf("}\n");
    }
    else if(rod == 3)
    {
        mpz_t liczba;
        mpz_init(liczba);
        mpz_import(liczba, (*z)->rozmiar, 1, 1, 0, 0, zawartosc(*z));
        char* str = mpz_get_str(NULL, 10, liczba);
        printf("%s\n", str);
        zwolnijPamiecZMpz(str, strlen(str) + 1);
        //gmp_printf("%Zd\n", liczba);
        mpz_clear(liczba);
    }
    else if(rod == 4) printf("%.*s\n", (*z)->rozmiar, zawartosc(*z));
    else if(rod == 5)
    {
        if(*(zawartosc(*z))) printf("tak\n");
        else printf("nie\n");
    }
    *wskaznikOdnosnikaPolecenia += 1 * sizeof(size_t);
}

// kluczowa zawartość dll

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
    "kropka_dz",

    "wywolaj_dz",
    "opusc_wjk",

    "jesli_klc",
    "poki_klc",

    "skok_wjk",

    "wypisz_prs"
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
    3 * sizeof(size_t), // "kropka_dz",
    
    SIZE_MAX,           // "wywolaj_dz",
    0,                  // "opusc_wjk",

    2 * sizeof(size_t), // "jesli_klc",
    2 * sizeof(size_t), // "poki_klc",

    1 * sizeof(size_t), // "skok_wjk",

    1 * sizeof(size_t) // "wypisz_prs"
};

__declspec(dllexport) size_t liczbaPrzenoszonychPolecen = sizeof(nazwyPrzenoszonychPolecen)/sizeof(nazwyPrzenoszonychPolecen[0]); // ich liczba