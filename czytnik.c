#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#include "powszechne.h"
#include "dzielone.h"


Zmienna** zmienne = NULL;               // zmienne
size_t liczbaZmiennych = 0;             // liczba używanych zmiennych
size_t pojemnoscZmiennych = 0;          // przypisane zmiennym miejsce

char** nazwyZmiennych = NULL;
size_t liczbaNazwZmiennych = 0;
size_t pojemnoscNazwZmiennych = 0;

FILE* zrodlo;
const char hasloCzp[] = { 0x50, 0x49, 0x45, 0x52, 0x57, 0x53, 0x5A, 0x59 }; // "PIERWSZY" zapisane szesnastkowo
char* poczatek = NULL;
char* koniec = NULL;

FILE* pekCzytelnegoPocztu;
int dlugoscTrzymakaNazwTymczasowych;
char* trzymakNazwTymaczasowych = NULL;

//size_t liczbaOzinow;
//size_t pojemnoscOzinow;
size_t* wskaznikLiczbyOzinow = NULL;
size_t* wskaznikPojemnosciOzinow = NULL;
Ozin* oziny;
//size_t dlugoscPocztu = 0;
size_t* wskaznikOdnosnikaPolecenia;
size_t* wskaznikDlugosciPocztu = NULL;
size_t* wskaznikPojemnosciPocztu = NULL;
char* poczet;

char* slowaKluczowe[] = {"###", "jesli", "poki"};
char* nazwyPolecenSlowKluczowych[] = {"###", "jesli_klc", "poki_klc"};
char* dzialania[] = {"==", "!=", ">=", "<=", "||", "&&", "+=", "-=", "*=", "/=", "%=", "+", "-", "*", "/", "%", "=", "!", ">", "<"}; // działania idą w kolejności malejącej długości (najpierw sprawdzamy najdłuższe)
char* nazwyPolecenDzialan[] = {"rowne_dz", "rozne_dz", "wieksze_badz_rowne_dz", "mniejsze_badz_rowne_dz", "lub_dz", "i_dz", "ndodaj_dz", "nodejmij_dz", "nmnoz_dz", "ndziel_dz", "nreszta_dz", "dodaj_dz", "odejmij_dz", "mnoz_dz", "dziel_dz", "reszta_dz", "nadaj_dz", "nie_dz", "wieksze_dz", "mniejsze_dz"};

Polecenie* polecenia = NULL;
char** nazwyPolecen = NULL;
size_t liczbaPolecen = 0;
size_t pojemnoscPolecen = 8;

HMODULE* dllDoZwolnienia = NULL;
unsigned int liczbaDllDoZwolnienia = 0;

uintptr_t nastepnaPotegaDwojki(uintptr_t x)
{
    if (x == 0) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
#if UINTPTR_MAX > 0xFFFFFFFF
    x |= x >> 32;
#endif
    return x + 1;
}

char* zawartosc(Zmienna* zmienna)
{
    return (char*)zmienna + sizeof(Zmienna);
}

void niezbywalnyBlad(const char* wiadomosc)
{
    fprintf(stderr, "[NIEZBYWALNY BLAD] %s\n", wiadomosc);
    for(size_t i = 1; i < liczbaZmiennych; i++)
    {
        if(zmienne[i]) free(zmienne[i]);
    }
    if(zmienne) free(zmienne);
    for(size_t i = 1; i < liczbaNazwZmiennych; i++)
    {
        if(nazwyZmiennych[i]) free(nazwyZmiennych[i]);
    }
    if(nazwyZmiennych) free(nazwyZmiennych);
    for(int i = 0; i < liczbaDllDoZwolnienia; i++)
    {
        if(dllDoZwolnienia[i]) FreeLibrary(dllDoZwolnienia[i]);
    }
    if(dllDoZwolnienia) free(dllDoZwolnienia);
    if(poczatek) free(poczatek);
    if(polecenia) free(polecenia);
    if(trzymakNazwTymaczasowych) free(trzymakNazwTymaczasowych);
    if(zrodlo) fclose(zrodlo);
    if(pekCzytelnegoPocztu) fclose(pekCzytelnegoPocztu);
    getchar();
    exit(EXIT_FAILURE);
}

size_t odnosnikWTablicyChar(const char* szukany, const char* tablica[])
{
    for (size_t i = 0; tablica[i] != NULL; i++) {
        if (strcmp(szukany, tablica[i]) == 0) {
            return 1; // Znaleziono
        }
    }
    return 0; // Nie znaleziono
}

// $ - obszary główne
// ! - zmienne wczytywane
// = - zmienne tymczasowe (do obliczeń)
size_t dodajNazweZmiennej(char* nazwa, size_t dlugosc)
{
    if(!nazwa) return 0;
    if(dlugosc == 0) dlugosc = strlen(nazwa);
    if(liczbaNazwZmiennych >= pojemnoscNazwZmiennych)
    {
        pojemnoscNazwZmiennych *= 2;
        nazwyZmiennych = (char**)realloc(nazwyZmiennych, pojemnoscNazwZmiennych * sizeof(char*));
        if(!nazwyZmiennych) niezbywalnyBlad("Brak pamieci");
    }
    char* pamiec = (char*)malloc(dlugosc + 1);
    if(!pamiec) niezbywalnyBlad("Brak pamieci");
    memcpy(pamiec, nazwa, dlugosc);
    pamiec[dlugosc] = '\0';
    nazwyZmiennych[liczbaNazwZmiennych] = pamiec;
    return liczbaNazwZmiennych++;
}

size_t znajdzLubDodajNazweZmiennej(char* nazwa, size_t dlugosc)
{
    if(nazwa == NULL) return 0;
    if(dlugosc == 0) dlugosc = strlen(nazwa);
    for(size_t i = 1; i < liczbaNazwZmiennych; i++)
    {
        if(memcmp(nazwa, nazwyZmiennych[i], dlugosc) == 0 && nazwyZmiennych[i][dlugosc] == '\0') return i;
    }
    if(liczbaNazwZmiennych >= pojemnoscNazwZmiennych)
    {
        pojemnoscNazwZmiennych *= 2;
        nazwyZmiennych = (char**)realloc(nazwyZmiennych, pojemnoscNazwZmiennych * sizeof(char*));
        if(nazwyZmiennych == NULL) niezbywalnyBlad("Brak pamieci");
    }
    char* pamiec = (char*)malloc(dlugosc + 1);
    if(!pamiec) niezbywalnyBlad("Brak pamieci");
    strncpy(pamiec, nazwa, dlugosc);
    pamiec[dlugosc] = '\0';
    nazwyZmiennych[liczbaNazwZmiennych] = pamiec;
    return liczbaNazwZmiennych++;
}

size_t utworzZmienna(size_t rozmiar, size_t pojemnosc, size_t rod, void* zawartosc)
{
    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zawartosc, rozmiar);
    if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(!zmienne) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    return liczbaZmiennych++;
}

size_t utworzZmiennaZCechami(size_t rozmiar, size_t pojemnosc, size_t rod, size_t cechy, char* zawartosc, size_t* zawartoscCech)
{
    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc + cechy * sizeof(size_t));
    if(zmienna == NULL) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = cechy;
    char *w = (char*)zmienna + sizeof(Zmienna);
    memcpy(w, zawartosc, rozmiar);
    memcpy(w + pojemnosc, zawartoscCech, cechy * sizeof(size_t));
    if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(zmienne == NULL) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    return liczbaZmiennych++;
}

size_t znajdzLubUtworzZmiennaWOzinach(char* nazwa, size_t rozmiar, size_t pojemnosc, size_t rod, void* zaw)
{
    size_t liczbaOzinow = *wskaznikLiczbyOzinow;
    for(size_t i = 0; i < liczbaOzinow; i++)
    {
        if(oziny[i].odnosnik == 0) continue;
        Zmienna* z = zmienne[oziny[i].odnosnik];
        if(z->rod == rod && z->rozmiar == rozmiar && memcmp(zaw, zawartosc(z), rozmiar) == 0) return i;
    }

    if(rozmiar > pojemnosc) pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + pojemnosc);
    if(!zmienna) niezbywalnyBlad("Brak pamieci");
    zmienna->rozmiar = rozmiar;
    zmienna->pojemnosc = pojemnosc;
    zmienna->rod = rod;
    zmienna->cechy = 0;
    memcpy((char*)zmienna + sizeof(Zmienna), zaw, rozmiar);
    if(liczbaZmiennych >= pojemnoscZmiennych)
    {
        pojemnoscZmiennych *= 2;
        zmienne = (Zmienna**)realloc(zmienne, pojemnoscZmiennych * sizeof(Zmienna*));
        if(!zmienne) niezbywalnyBlad("Brak pamieci");
    }
    zmienne[liczbaZmiennych] = zmienna;
    if(wskaznikPojemnosciOzinow)
    {
        if(liczbaOzinow >= *wskaznikPojemnosciOzinow)
        {
            *wskaznikPojemnosciOzinow = nastepnaPotegaDwojki(liczbaOzinow + 1);
            oziny = (Ozin*)realloc(oziny, *wskaznikPojemnosciOzinow * sizeof(Ozin));
            if(!oziny) niezbywalnyBlad("Brak pamieci");
        }
        oziny[liczbaOzinow].odnosnik = liczbaZmiennych++;
        oziny[liczbaOzinow].nazwa = znajdzLubDodajNazweZmiennej(nazwa, 0);
        //printf("Utworzono zmienna: %zu (%s)\n", liczbaOzinow, nazwa);
        return (*wskaznikLiczbyOzinow)++;
    }
    printf("ZWROCONO ODNOSNIK ZMIENNEJ, NIE OZINA\n");
    return liczbaZmiennych++;
}


void sciagnijZmienne(Zmienna*** wskaznikZmiennych, size_t* wskaznikLiczbyZmiennych, size_t* wskaznikPojemnosciZmiennych)
{
    if(wskaznikZmiennych) wskaznikZmiennych = &zmienne;
    if(wskaznikLiczbyZmiennych) wskaznikLiczbyZmiennych = &liczbaZmiennych;
    if(wskaznikPojemnosciZmiennych) wskaznikPojemnosciZmiennych = &pojemnoscZmiennych;
}

void sciagnijNazwyZmiennych(char*** wskaznikNazwZmiennych, size_t* wskaznikLiczbyNazwZmiennych, size_t* wskaznikPojemnosciNazwZmiennych)
{
    if(wskaznikNazwZmiennych) wskaznikNazwZmiennych = &nazwyZmiennych;
    if(wskaznikLiczbyNazwZmiennych) wskaznikLiczbyNazwZmiennych = &liczbaNazwZmiennych;
    if(wskaznikPojemnosciNazwZmiennych) wskaznikPojemnosciNazwZmiennych = &pojemnoscNazwZmiennych;
}

void sciagnijPoczet(char*** wWPocztu, size_t*** wWWOdnosnikaPolecenia, size_t*** wWWDlugosciPocztu, size_t*** wWWPojemnosciPocztu)
{
    if(wWPocztu) *wWPocztu = &poczet;
    if(wWWOdnosnikaPolecenia) *wWWOdnosnikaPolecenia = &wskaznikOdnosnikaPolecenia;
    if(wWWDlugosciPocztu) *wWWDlugosciPocztu = &wskaznikDlugosciPocztu;
    if(wWWPojemnosciPocztu) *wWWPojemnosciPocztu = &wskaznikPojemnosciPocztu;
}

size_t walkujGalazPodwojna(GalazPodwojna* galaz, size_t liczbaZmiennychTymczasowych);
void walkujRozgalezienie(Rozgalezienie* rozgalezienie, size_t liczbaZmiennychTymczasowych);

size_t walkujGalazPodwojna(GalazPodwojna* galaz, size_t liczbaZmiennychTymczasowych)
{
    if(!galaz) return 0;
    if(galaz->rodzaj == zmienniana)
    {
        return galaz->wartosc;
    }
    for(size_t i = 1; i < liczbaSlowKluczowych; i++) // pierwsze słowo kluczowe pomijamy bo jest ono przetwarzane nieco inaczej
    {
        if(kluczowe[i] == galaz->rodzaj)
        {
            printf("Rozpoczeto wpisywanie slowa kluczowego: %s (liczba zmiennych tymczasowych: %zu)", nazwyPolecenSlowKluczowych[i], liczbaZmiennychTymczasowych);
            getchar();
            if(!galaz->lewa || !galaz->prawa) niezbywalnyBlad("Blad wczytywania slowa kluczowego (brak galezi)");
            size_t poczatekPocztuSlowaKluczowego = *wskaznikDlugosciPocztu; // zapisujemy gdzie zaczynamy bo wałkowanie lewej gałęzi może i najpewniej go przesunie

            size_t z1 = walkujGalazPodwojna((GalazPodwojna*)(galaz->lewa), liczbaZmiennychTymczasowych);

            size_t dlugoscPocztu = *wskaznikDlugosciPocztu;
            size_t pojemnoscPocztu = *wskaznikPojemnosciPocztu;
            size_t wymaganaDlugosc = dlugoscPocztu + 1 + sizeof(size_t)*2;
            if(pojemnoscPocztu < wymaganaDlugosc)
            {
                pojemnoscPocztu = nastepnaPotegaDwojki(wymaganaDlugosc);
                poczet = (char*)realloc(poczet, pojemnoscPocztu);
                if(poczet == NULL) niezbywalnyBlad("Brak pamieci");
            }

            fprintf(pekCzytelnegoPocztu, "%s %zu\n", nazwyPolecenSlowKluczowych[i], z1);
            unsigned char odnosnikPolecenia = 0; // 0 to nieprawidłowe polecenie (w tablicy ma wartość NULL)
            for(unsigned char j = 1; j < liczbaPolecen; j++)
            {
                if(strcmp(nazwyPolecen[j], nazwyPolecenSlowKluczowych[i]) == 0) odnosnikPolecenia = j;
            }
            if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia obslugujacego slowo kluczowe");
            memcpy(poczet + dlugoscPocztu, &odnosnikPolecenia, 1);
            dlugoscPocztu++;
            memcpy(poczet + dlugoscPocztu, &z1, sizeof(size_t));
            dlugoscPocztu += sizeof(size_t);
            size_t miejsceSkoku = dlugoscPocztu; // zapisujemy gdzie wpiszemy wartość skoku kiedy ją znajdziemy
            dlugoscPocztu += sizeof(size_t); // a na razie pomijamy tą wartość

            // odswieżamy wskaźniki przed wałkowaniem rozgałęzienia
            *wskaznikDlugosciPocztu = dlugoscPocztu;
            *wskaznikPojemnosciPocztu = pojemnoscPocztu;
            
            walkujRozgalezienie((Rozgalezienie*)(galaz->prawa), liczbaZmiennychTymczasowych);

            // odświeżamy zmienne po wałkowaniu
            dlugoscPocztu = *wskaznikDlugosciPocztu;
            pojemnoscPocztu = *wskaznikPojemnosciPocztu;

            printf("Wpisano slowo kluczowe %s (%s)", nazwyPolecen[odnosnikPolecenia], nazwyPolecenSlowKluczowych[i]);
            if(i == 2) // jeśli to polecenie 'póki' musi mieć skok
            {
                // poszerzamy pamięć jeśli to konieczne
                size_t wymaganaDlugosc = dlugoscPocztu + 1 + sizeof(size_t);
                if(pojemnoscPocztu < wymaganaDlugosc)
                {
                    pojemnoscPocztu = nastepnaPotegaDwojki(wymaganaDlugosc);
                    poczet = (char*)realloc(poczet, pojemnoscPocztu);
                    if(poczet == NULL) niezbywalnyBlad("Brak pamieci");
                }

                // teraz szukamy polecenia skoku i dorzucamy je do pocztu
                odnosnikPolecenia = 0;
                for(unsigned char j = 1; j < liczbaPolecen; j++)
                {
                    if(strcmp(nazwyPolecen[j], "skok_wjk") == 0) odnosnikPolecenia = j;
                }
                if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia obslugujacego wyjatkowe");
                memcpy(poczet + dlugoscPocztu, &odnosnikPolecenia, 1);
                dlugoscPocztu++;
                fprintf(pekCzytelnegoPocztu, "skok_wjk %zu\n", poczatekPocztuSlowaKluczowego);
                memcpy(poczet + dlugoscPocztu, &poczatekPocztuSlowaKluczowego, sizeof(size_t));
                dlugoscPocztu += sizeof(size_t);

                // teraz wiedząc gdzie jest koniec wpisujemy wartość skoku
                memcpy(poczet + miejsceSkoku, &dlugoscPocztu, sizeof(size_t));

                *wskaznikDlugosciPocztu = dlugoscPocztu;
                *wskaznikPojemnosciPocztu = pojemnoscPocztu;
                printf("\nWpisano wyjatkowe %s (%s)", nazwyPolecen[odnosnikPolecenia], nazwyPolecenSlowKluczowych[i]);
            }
            getchar();
            return 0;
        }
    }
    for(size_t i = 0; i < liczbaDzialan; i++)
    {
        if(dzialaniowe[i] == galaz->rodzaj)
        {
            printf("Rozpoczeto wpisywanie dzialania: %s (liczba zmiennych tymczasowych: %zu)", nazwyPolecenDzialan[i], liczbaZmiennychTymczasowych);
            getchar();
            snprintf(trzymakNazwTymaczasowych, dlugoscTrzymakaNazwTymczasowych, "=t%zu", liczbaZmiennychTymczasowych);
            size_t nazwa = znajdzLubDodajNazweZmiennej(trzymakNazwTymaczasowych, 0);
            size_t wynik = znajdzLubUtworzZmiennaWOzinach(nazwyZmiennych[nazwa], sizeof(size_t), 0, 0, &nazwa);
            size_t z1 = walkujGalazPodwojna((GalazPodwojna*)(galaz->lewa), liczbaZmiennychTymczasowych + 1);
            size_t z2 = walkujGalazPodwojna((GalazPodwojna*)(galaz->prawa), liczbaZmiennychTymczasowych + 2);

            size_t dlugoscPocztu = *wskaznikDlugosciPocztu;
            size_t pojemnoscPocztu = *wskaznikPojemnosciPocztu;
            size_t wymaganaDlugosc = dlugoscPocztu + 1 + sizeof(size_t)*3; // wydłużamy poczet o odnośnik polecenia i 3 argumenty
            if(pojemnoscPocztu < wymaganaDlugosc)
            {
                pojemnoscPocztu = nastepnaPotegaDwojki(wymaganaDlugosc);
                poczet = (char*)realloc(poczet, pojemnoscPocztu);
                if(poczet == NULL) niezbywalnyBlad("Brak pamieci");
            }

            fprintf(pekCzytelnegoPocztu, "%s %zu", nazwyPolecenDzialan[i], wynik); // wpisujemy nazwę polecenia i odnośnik zmiennej wynikowej
            unsigned char odnosnikPolecenia = 0; // 0 to nieprawidłowe polecenie (w tablicy ma wartość NULL)
            for(unsigned char j = 1; j < liczbaPolecen; j++)
            {
                if(strcmp(nazwyPolecen[j], nazwyPolecenDzialan[i]) == 0) odnosnikPolecenia = j;
            }
            if(!odnosnikPolecenia) niezbywalnyBlad("Nie znaleziono polecenia obslugujacego dzialanie");
            memcpy(poczet + dlugoscPocztu, &odnosnikPolecenia, 1);
            dlugoscPocztu++;
            memcpy(poczet + dlugoscPocztu, &wynik, sizeof(size_t));
            dlugoscPocztu += sizeof(size_t);
            if(galaz->lewa) // jeśli mamy lewą stronę dopisujemy ją
            {
                fprintf(pekCzytelnegoPocztu, " %zu", z1);
                memcpy(poczet + dlugoscPocztu, &z1, sizeof(size_t));
                dlugoscPocztu += sizeof(size_t);
            }
            if(galaz->prawa) // jeśli mamy prawą stronę dopisujemy ją
            {
                fprintf(pekCzytelnegoPocztu, " %zu", z2);
                memcpy(poczet + dlugoscPocztu, &z2, sizeof(size_t));
                dlugoscPocztu += sizeof(size_t);
            }
            fprintf(pekCzytelnegoPocztu, "\n");
            *wskaznikDlugosciPocztu = dlugoscPocztu;
            *wskaznikPojemnosciPocztu = pojemnoscPocztu;
            printf("Wpisano dzialanie %s (%s)", nazwyPolecen[odnosnikPolecenia], nazwyPolecenDzialan[i]);
            getchar();
            return wynik;
        }
    }
}

void walkujRozgalezienie(Rozgalezienie* rozgalezienie, size_t liczbaZmiennychTymczasowych)
{
    if(!rozgalezienie) return;
    for(size_t i = 0; i < rozgalezienie->rozmiar; i++)
    {
        walkujGalazPodwojna(rozgalezienie->galezie[i], liczbaZmiennychTymczasowych);
    }
}


void przelaczNaObszar(Zmienna* obszar, size_t* wskaznikPojemnosci)
{
    char* dane = zawartosc(obszar);

    //memcpy(&liczbaOzinow, dane, sizeof(size_t));
    //pojemnoscOzinow = liczbaOzinow;

    wskaznikLiczbyOzinow = (size_t*)dane;
    if(wskaznikPojemnosci != NULL) *wskaznikPojemnosci = *wskaznikLiczbyOzinow;
    //memcpy(&dlugoscPocztu, dane + sizeof(size_t), sizeof(size_t));
    wskaznikPojemnosciPocztu = (size_t*)(dane + sizeof(size_t));
    oziny = (Ozin*)(dane + sizeof(size_t) * 2);
    poczet = dane + sizeof(size_t) * 2 + *wskaznikLiczbyOzinow * sizeof(Ozin);
    for(size_t i = 0; i < *wskaznikLiczbyOzinow; i++)
    {
        printf("%s: %zu\n", nazwyZmiennych[oziny[i].nazwa], oziny[i].odnosnik);
    }
    printf("Pierwszy znak pocztu: %d (%s)\n", poczet[0], nazwyPolecen[poczet[0]]);
}

// zaklepuje pamięć i tworzy podstawowe zmienne rodowe
void przygotowaniePodstawowe()
{
    const int liczbaZmiennychPodstawowych = 5;

    pojemnoscZmiennych = nastepnaPotegaDwojki(liczbaZmiennychPodstawowych + 1);
    zmienne = (Zmienna**)malloc(pojemnoscZmiennych*sizeof(Zmienna*));
    zmienne[0] = NULL;
    liczbaZmiennych = 1;

    pojemnoscNazwZmiennych = pojemnoscZmiennych;
    nazwyZmiennych = (char**)malloc(pojemnoscNazwZmiennych*sizeof(char*));
    nazwyZmiennych[0] = NULL;
    liczbaNazwZmiennych = 1;

    // Tworzenie zmiennej obszaru
    size_t pustyObszar[2] = {0};
    utworzZmienna(sizeof(pustyObszar), sizeof(pustyObszar), 1, (char*)pustyObszar);
    dodajNazweZmiennej("obs", 3);

    // Tworzenie zmiennej liczby
    char pustaLiczba = 0;
    utworzZmienna(1, 1, 2, &pustaLiczba);
    dodajNazweZmiennej("lic", 3);

    // Tworzenie zmiennej pisma
    char pustePismo[1];
    utworzZmienna(0, 1, 3, pustePismo);
    dodajNazweZmiennej("pis", 3);

    // Tworzenie zmiennej tak-lub-nie (boolean)
    char pustyTln = 0;
    utworzZmienna(1, 1, 4, &pustyTln);
    dodajNazweZmiennej("tln", 3);
}

void przygotowanieDlaCzp()
{
    const int r = sizeof(hasloCzp)/sizeof(char);
    unsigned char bajty[r];
    fread(bajty, 1, r, zrodlo);
    if(memcmp(bajty, hasloCzp, r) != 0)
    {
        niezbywalnyBlad("Pek .czp uszkodzony lub niepoprawny");
    }
    przygotowaniePodstawowe();
    uintptr_t rozmiar = 2 * sizeof(uintptr_t) + 1;
    uintptr_t pojemnosc = nastepnaPotegaDwojki(rozmiar);
    Zmienna* obszarDomyslny = malloc(sizeof(Zmienna) + pojemnosc);
    if(obszarDomyslny == NULL) niezbywalnyBlad("Brak pamieci");
    obszarDomyslny->cechy = 0;
    obszarDomyslny->rod = 1;
    obszarDomyslny->rozmiar = rozmiar;
    obszarDomyslny->pojemnosc = pojemnosc;
    uintptr_t* w = (uintptr_t*)((char*)obszarDomyslny + sizeof(Zmienna));
    *w = 0;
    w = (uintptr_t*)((char*)w + sizeof(uintptr_t));
    *w = 1;
    *((char*)w + sizeof(uintptr_t)) = 2;
    zmienne[liczbaZmiennych] = obszarDomyslny;
}

size_t przygotowanieDlaCz()
{
    przygotowaniePodstawowe();
    size_t w1 = 0;
    size_t w2 = 2;
    char p1 = 2;
    char p2 = 3;
    char zawartosc[sizeof(size_t) * 2 + 2];

    memcpy(zawartosc, &w1, sizeof(size_t));
    memcpy(zawartosc + sizeof(size_t), &w2, sizeof(size_t));
    memcpy(zawartosc + sizeof(size_t) * 2, &p1, sizeof(char));
    memcpy(zawartosc + sizeof(size_t) * 2 + sizeof(char), &p2, sizeof(char));
    size_t odnosnikZmiennej = utworzZmienna(sizeof(size_t) * 2 + 2, 0, 1, zawartosc);
    size_t odnosnikNazwy = dodajNazweZmiennej("$domyslny", 9);
    if(odnosnikZmiennej == odnosnikNazwy) return odnosnikZmiennej;
    else niezbywalnyBlad("Pomieszanie odnosnikow");
    return 0;
}

Czastka wczytujNastepne(char** wskaznik);
size_t wczytujJakoNazwa(char** wskaznik);

size_t wczytujJakoObszarGlowny(char** wskaznik)
{
    printf("Rozpoczeto wczytywanie obszaru glownego\n");
    char* polozenie = *wskaznik;
    printf("Zaladowany tekst:\n%s\n", polozenie);
    
    size_t liczbaOzinowObszaru = 1;
    size_t pojemnoscOzinowObszaru = 1;
    Ozin* ozinyObszaru = (Ozin*)malloc(sizeof(Ozin) * pojemnoscOzinowObszaru);
    ozinyObszaru[0].odnosnik = 0;
    ozinyObszaru[0].nazwa = 0;

    Ozin* stareOziny = oziny;
    size_t* staryWskaznikLiczbyOzinow = wskaznikLiczbyOzinow;
    size_t* staryWskaznikPojemnosciOzinow = wskaznikPojemnosciOzinow;

    oziny = ozinyObszaru;
    wskaznikLiczbyOzinow = &liczbaOzinowObszaru;
    wskaznikPojemnosciOzinow = &pojemnoscOzinowObszaru;

    Czastka* czastkiObszaru = (Czastka*)malloc(sizeof(Czastka));
    size_t pojemnoscCzastekObszaru = 1;
    size_t rozmiarCzastekObszaru = 0;

    while(TRUE)
    {
        while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
        if(polozenie >= koniec)
        {
            *wskaznik = polozenie;
            break;
        }
        Czastka c = wczytujNastepne(&polozenie);
        if(c.rodzaj == 1 && (char*)c.zawartosc == slowaKluczowe[0]) break; // kończymy jeśli napotkamy ###
        if(rozmiarCzastekObszaru >= pojemnoscCzastekObszaru)
        {
            pojemnoscCzastekObszaru *= 2;
            czastkiObszaru = realloc(czastkiObszaru, sizeof(Czastka) * pojemnoscCzastekObszaru);
            if(czastkiObszaru == NULL) niezbywalnyBlad("Brak pamieci");
        }
        czastkiObszaru[rozmiarCzastekObszaru++] = c;
    }

    printf("Wczytano czastki:\n");
    for(size_t i = 0; i < rozmiarCzastekObszaru; i++)
    {
        if(czastkiObszaru[i].rodzaj == 2) printf("Czastka %zu: %hhu - %zu\n", i, czastkiObszaru[i].rodzaj, (size_t)czastkiObszaru[i].zawartosc);
        else if(czastkiObszaru[i].rodzaj == 0) printf("Czastka %zu: %hhu - %c\n", i, czastkiObszaru[i].rodzaj, *((char*)czastkiObszaru[i].zawartosc));
        else printf("Czastka %zu: %hhu - %s\n", i, czastkiObszaru[i].rodzaj, (char*)czastkiObszaru[i].zawartosc);
    }

    getchar();

    printf("Rozpoczeto robienie drzewa\n");
    Rozgalezienie* drzewo = robDrzewo(czastkiObszaru, rozmiarCzastekObszaru);
    printf("Drzewo:\n");
    wypiszDrzewo(drzewo, 0);
    getchar();

    //pekCzytelnegoPocztu = fopen("C:\\Users\\_USER_\\Desktop\\CZ2\\poczet.txt", "w");
    pekCzytelnegoPocztu = fopen("poczet.txt", "w+");
    if(pekCzytelnegoPocztu == NULL)
    {
        perror("fopen (poczet.txt)");
        niezbywalnyBlad("Nie mozna otworzyc peku pocztu (w)");
    }
    /*fclose(pekCzytelnegoPocztu);
    pekCzytelnegoPocztu = fopen("C:\\Users\\_USER_\\Desktop\\CZ2\\poczet.txt", "a");
    if(pekCzytelnegoPocztu == NULL)
    {
        perror("fopen (poczet.txt)");
        niezbywalnyBlad("Nie mozna otworzyc peku pocztu (a)");
    }*/

    char* staryPoczet = poczet;
    size_t* staryWskaznikDlugosciPocztu = wskaznikDlugosciPocztu;

    size_t dlugoscPocztu = 0;
    size_t pojemnoscPocztu = 16;
    wskaznikDlugosciPocztu = &dlugoscPocztu;
    wskaznikPojemnosciPocztu = &pojemnoscPocztu;
    poczet = malloc(pojemnoscPocztu);

    walkujRozgalezienie(drzewo, 1);
    fclose(pekCzytelnegoPocztu);
    printf("Zakonczono walkowanie\n");

    ozinyObszaru = oziny;
    oziny = stareOziny;
    wskaznikLiczbyOzinow = staryWskaznikLiczbyOzinow;
    wskaznikPojemnosciOzinow = staryWskaznikPojemnosciOzinow;

    size_t r = 2*sizeof(size_t) + liczbaOzinowObszaru * sizeof(Ozin) + dlugoscPocztu;
    char* pamiec = malloc(r + 1);
    memcpy(pamiec, &liczbaOzinowObszaru, sizeof(size_t));
    memcpy(pamiec + sizeof(size_t), &dlugoscPocztu, sizeof(size_t));
    memcpy(pamiec + 2 * sizeof(size_t), ozinyObszaru, liczbaOzinowObszaru * sizeof(Ozin));
    memcpy(pamiec + 2 * sizeof(size_t) + liczbaOzinowObszaru * sizeof(Ozin), poczet, dlugoscPocztu);
    char odnosnikPoleceniaKonczacego = 4;
    memcpy(pamiec + r, &odnosnikPoleceniaKonczacego, 1);
    dlugoscPocztu++;
    size_t wynik = znajdzLubUtworzZmiennaWOzinach("$glowny", r + 1, 0, 1, pamiec);

    free(pamiec);
    free(ozinyObszaru);
    free(poczet);
    free(czastkiObszaru);
    wskaznikDlugosciPocztu = staryWskaznikDlugosciPocztu;
    poczet = staryPoczet;
    //printf("Przywrocono stary poczet\n");

    zetnijRozgalezienie(drzewo);
    //printf("Scieto drzewo\n");

    //printf("%d\n", wskaznikLiczbyOzinow == &liczbaOzinowObszaru);
    /*for(size_t i = 0; i < liczbaOzinowObszaru; i++)
    {
        printf("%s: %zu\n", nazwyZmiennych[oziny[i].nazwa], oziny[i].odnosnik);
    }*/

    //printf("Uwolniono oziny\n");
    //printf("Uwolniono czastki\n");

    return wynik;
}

size_t wczytujJakoObszar(char** wskaznik) // nie pojawi się w pierwszym teście
{
    char* polozenie = *wskaznik;
    
    // Sprawdzanie czy zaczyna się na "obszar"
    if(6 > koniec - polozenie) return 0;
    if(memcmp("obszar", polozenie, 6) != 0) return 0;
    if(isalpha((unsigned char)polozenie[6])) return 0;
    polozenie += 6;

    // Wczytywanie zbioru wartości przenoszonych (argumentów)
    size_t liczbaZmiennychObszaru = 1;
    size_t pojemnoscZmiennychObszaru = 1;
    Ozin* zmienneObszaru = (Ozin*)malloc(sizeof(Ozin) * pojemnoscZmiennychObszaru);
    zmienneObszaru[0].odnosnik = 0;
    zmienneObszaru[0].nazwa = 0;
    while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
    if(polozenie[0] == '(')
    {
        polozenie++;
        while(TRUE)
        {
            while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
            if(polozenie[0] == ')') 
            {
                polozenie++;
                break;
            }
            size_t odnosnik = wczytujJakoNazwa(&polozenie);
            if(odnosnik == 0) niezbywalnyBlad("Blad skladni: nieprawidlowe przekazanie nazw wartosci przenoszonych obszaru");

            if(liczbaZmiennychObszaru >= pojemnoscZmiennychObszaru)
            {
                pojemnoscZmiennychObszaru *= 2;
                zmienneObszaru = (Ozin*)realloc(zmienneObszaru, sizeof(Ozin)* pojemnoscZmiennychObszaru);
                if(zmienneObszaru == NULL) niezbywalnyBlad("Brak pamieci");
            }
            zmienneObszaru[liczbaZmiennychObszaru].odnosnik = odnosnik;
            //zmienneObszaru[liczbaZmiennychObszaru++].nazwa = zawartosc(zmienne[odnosnik]);

            while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;

            if(polozenie[0] == ',') 
            {
                polozenie++;
            }
            else if(polozenie[0] == ')') 
            {
                polozenie++;
                break;
            }
            else niezbywalnyBlad("Blad skladni: nieprawidlowe przekazanie nazw wartosci przenoszonych obszaru");
        }
    }

    // Wczytywanie spiecia
    // if(polozenie[0] != '{' && polozenie - poczatek != 0) return NULL;
    if(polozenie[0] == '{')
    {
        polozenie++;
        Czastka* czastkiObszaru = (Czastka*)malloc(sizeof(Czastka));
        size_t pojemnoscCzastekObszaru = 1;
        size_t rozmiarCzastekObszaru = 0;
        while(TRUE)
        {
            while(polozenie < koniec && isspace((unsigned char)polozenie[0])) polozenie++;
            if(polozenie >= koniec || polozenie[0] == '}')
            {
                if(polozenie[0] == '}') polozenie += 1;
                *wskaznik = polozenie;
                break;
            }
            Czastka c = wczytujNastepne(&polozenie);
            if(pojemnoscCzastekObszaru == rozmiarCzastekObszaru)
            {
                pojemnoscCzastekObszaru *= 2;
                czastkiObszaru = realloc(czastkiObszaru, sizeof(Czastka) * pojemnoscCzastekObszaru);
                if(czastkiObszaru == NULL) niezbywalnyBlad("Brak pamieci");
            }
            czastkiObszaru[rozmiarCzastekObszaru++] = c;
        }

        Rozgalezienie* drzewo = robDrzewo(czastkiObszaru, rozmiarCzastekObszaru);
    }
    

    /*Zmienna* zmienna = (Zmienna*)malloc(sizeof(Zmienna) + 2*sizeof(uintptr_t));
    zmienna->rod = 1;
    zmienna->rozmiar = 2*sizeof(uintptr_t);
    zmienna->pojemnosc = 2*sizeof(uintptr_t);
    zmienna->cechy = 0;
    memset((char*)zmienna + sizeof(Zmienna), 0, 2 * sizeof(uintptr_t));
    return zmienna;*/
    return 0;
}

size_t wczytujJakoLiczbe(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(!isdigit((unsigned char)polozenie[0])) return 0;
    char* pierwotne = polozenie;
    while(polozenie < koniec && isdigit((unsigned char)polozenie[0])) polozenie++;

    size_t dlugosc = polozenie - pierwotne;
    //printf("Wczytujemy liczbe: ");
    //fwrite(pierwotne, 1, dlugosc, stdout);
    //printf("\n");

    char* liczba = malloc(dlugosc + 1);
    if(!liczba) niezbywalnyBlad("Brak pamieci");

    memcpy(liczba, pierwotne, dlugosc);
    liczba[dlugosc] = '\0';
    //printf("Zapisano liczbe: %s. Zaraz wszystko sie posypie X~X\n", liczba);

    mpz_t liczba_mpz;
    mpz_init(liczba_mpz);
    //printf("Jeszcze zyjemy!\n");

    if(mpz_set_str(liczba_mpz, liczba, 10) != 0)
    {
        mpz_clear(liczba_mpz);
        free(liczba);
        niezbywalnyBlad("Przeksztalcanie liczby nie powiodlo sie");
    }
    size_t rozmiar;
    void* surowa = mpz_export(NULL, &rozmiar, 1, 1, 0, 0, liczba_mpz);
    if(!surowa && mpz_cmp_ui(liczba_mpz, 0) != 0)
    {
        mpz_clear(liczba_mpz);
        free(liczba);
        niezbywalnyBlad("Blad eksportu liczby");
    }
    //size_t wynik = utworzZmienna(rozmiar, 1, 2, surowa);
    size_t wynik = znajdzLubUtworzZmiennaWOzinach("!lic", rozmiar, 1, 2, surowa);
    //printf("Utworzono zmienna na miejscu %zu\n", wynik);
    //printf("Przed mpz_clear\n");
    mpz_clear(liczba_mpz);
    //printf("Przed free(liczba)\n");
    free(liczba);
    //printf("Przed przesunieciem wskaznika\n");
    *wskaznik += dlugosc;
    //printf("Liczba wczytana! ^-^\n");
    return wynik;
}

size_t wczytujJakoPismo(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(polozenie[0] != '"') return 0;
    char* pierwotne = ++polozenie;
    while(polozenie[0] != '"') polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    //size_t wynik = utworzZmienna(dlugosc, 1, 3, pierwotne);
    size_t wynik = znajdzLubUtworzZmiennaWOzinach("!pis", dlugosc, 1, 3, pierwotne);
    *wskaznik += dlugosc + 2;
    return wynik;
}

size_t wczytujJakoTln(char** wskaznik)
{
    char* polozenie = *wskaznik;
    size_t pozostale = koniec - polozenie;
    if(pozostale < 3) return 0;
    if(memcmp(polozenie, "nie", 3) == 0 && (pozostale == 3 || !isalpha(polozenie[3])))
    {
        *wskaznik += 3;
        unsigned char wartosc = 0;
        //return utworzZmienna(sizeof(wartosc), 1, 4, &wartosc);
        return znajdzLubUtworzZmiennaWOzinach("!tln", sizeof(wartosc), 1, 4, &wartosc);
    }
    if(memcmp(polozenie, "tak", 3) == 0 && (pozostale == 3 || !isalpha(polozenie[3])))
    {
        *wskaznik += 3;
        unsigned char wartosc = 1;
        //return utworzZmienna(sizeof(wartosc), 1, 4, &wartosc);
        return znajdzLubUtworzZmiennaWOzinach("!tln", sizeof(wartosc), 1, 4, &wartosc);
    }
    return 0;
}

size_t wczytujJakoNazwa(char** wskaznik)
{
    char* polozenie = *wskaznik;
    if(!isalpha((unsigned char)polozenie[0])) return 0;
    char* pierwotne = polozenie;
    while(isalpha((unsigned char)polozenie[0])) polozenie++;
    uintptr_t dlugosc = polozenie - pierwotne;
    size_t nazwa = znajdzLubDodajNazweZmiennej(pierwotne, dlugosc);
    *wskaznik += dlugosc;
    //return utworzZmienna(sizeof(size_t), 0, 0, &(nazwyZmiennych[nazwa])); // utworzenie zmiennej odnosnik do nazwy (a nie nazwę)
    return znajdzLubUtworzZmiennaWOzinach(nazwyZmiennych[nazwa], sizeof(size_t), 0, 0, &nazwa); // utworzenie zmiennej odnosnik do nazwy (a nie nazwę)
}

char* wczytujKluczowe(char** wskaznik)
{
    char* polozenie = *wskaznik;
    for(int i = 0; i < sizeof(slowaKluczowe)/sizeof(slowaKluczowe[0]); i++)
    {
        size_t dlugosc = strlen(slowaKluczowe[i]);
        if(dlugosc > koniec - polozenie) continue;
        if(memcmp(slowaKluczowe[i], polozenie, dlugosc) != 0) continue;
        if(isalpha((unsigned char)polozenie[dlugosc])) continue;
        *wskaznik += dlugosc;
        return slowaKluczowe[i];
    }
    return NULL;
}

char* wczytujDzialania(char** wskaznik)
{
    char* polozenie = *wskaznik;
    for(int i = 0; i < sizeof(dzialania)/sizeof(dzialania[0]); i++)
    {
        size_t dlugosc = strlen(dzialania[i]);
        if(dlugosc > koniec - polozenie) continue;
        if(memcmp(dzialania[i], polozenie, dlugosc) != 0) continue;
        *wskaznik += dlugosc;
        return dzialania[i];
    }
    return NULL;
}

size_t (*konajkiDoZmiennych[])(char**) = {wczytujJakoObszar, wczytujJakoLiczbe, wczytujJakoPismo, wczytujJakoTln, wczytujJakoNazwa};

size_t wczytujZmienne(char** wskaznik)
{
    size_t wynik;
    for(int i = 0; i < sizeof(konajkiDoZmiennych) / sizeof(konajkiDoZmiennych[0]); i++)
    {
        wynik = konajkiDoZmiennych[i](wskaznik);
        if(wynik) return wynik;
    }
    return 0;
}

Czastka wczytujNastepne(char** wskaznik)
{
    uintptr_t wynik;
    wynik = (uintptr_t)wczytujKluczowe(wskaznik);
    if(wynik)
    {
        Czastka c = {1, wynik};
        printf("Wczytano kluczowe: %s\n", (char*)wynik);
        return c;
    }
    wynik = (uintptr_t)wczytujDzialania(wskaznik);
    if(wynik)
    {
        Czastka c = {3, wynik};
        printf("Wczytano dzialanie: %s\n", (char*)wynik);
        return c;
    }
    wynik = wczytujZmienne(wskaznik);
    if(wynik)
    {
        Czastka c = {2, wynik};
        printf("Wczytano zmienna: %zu (nazwa: %s)\n", (size_t)wynik, nazwyZmiennych[oziny[(size_t)wynik].nazwa]);
        return c;
    }
    printf("Wczytano nieznany: %c\n", **wskaznik);
    Czastka c = {0, (uintptr_t)((*wskaznik)++)};
    return c;
}

void wzywanie(char* nazwa)
{
    HMODULE uchwytDll = LoadLibrary(nazwa);
    if(!uchwytDll) niezbywalnyBlad("Nie udalo sie wczytac polecen");
    printf("Pomyslnie pozyskano uchwyt\n");

    //char*** wskaznikNazwPrzenoszonychPolecen = (char***)GetProcAddress(uchwytDll, "nazwyPrzenoszonychPolecen");
    char** nazwyPrzenoszonychPolecen = (char**)GetProcAddress(uchwytDll, "nazwyPrzenoszonychPolecen");
    size_t* wskaznikLiczbyPrzenoszonychPolecen = (size_t*)GetProcAddress(uchwytDll, "liczbaPrzenoszonychPolecen");

    if(nazwyPrzenoszonychPolecen && wskaznikLiczbyPrzenoszonychPolecen) // jeśli oba zosatły wczytane przenosimy polecenia (ich obecność nie jest obowiązkowa)
    {
        printf("Pomyslnie pozyskano nazwy, rozpoczynam wczytywanie\n");
        //char** nazwyPrzenoszonychPolecen = *wskaznikNazwPrzenoszonychPolecen;
        size_t liczbaPrzenoszonychPolecen = *wskaznikLiczbyPrzenoszonychPolecen;

        for(size_t i = 0; i < liczbaPrzenoszonychPolecen; i++)
        {
            Polecenie p = (Polecenie)GetProcAddress(uchwytDll, nazwyPrzenoszonychPolecen[i]);
            if(p)
            {
                if(liczbaPolecen >= pojemnoscPolecen)
                {
                    pojemnoscPolecen *= 2;
                    polecenia = (Polecenie*)realloc(polecenia, sizeof(Polecenie) * pojemnoscPolecen);
                    nazwyPolecen = (char**)realloc(nazwyPolecen, sizeof(char*) * pojemnoscPolecen);
                    if(!polecenia || !nazwyPolecen) niezbywalnyBlad("Brak pamieci");
                }
                polecenia[liczbaPolecen] = p;
                printf("Pomyslnie pozyskano polecenie %s\n", nazwyPrzenoszonychPolecen[i]);
                nazwyPolecen[liczbaPolecen++] = nazwyPrzenoszonychPolecen[i];
            }
        }
    }

    printf("Koniec wczytywania polecen\n");

    DostosujKon dostosuj = (DostosujKon)GetProcAddress(uchwytDll, "dostosuj"); // jeśli taka funkcja jest to ją wywołujemy
    if(dostosuj) dostosuj(sciagnijZmienne, sciagnijNazwyZmiennych, sciagnijPoczet);
    if(dostosuj) printf("Wywolano dostosowywanie\n");

    dllDoZwolnienia = (HMODULE*)realloc(dllDoZwolnienia, sizeof(HMODULE) * (liczbaDllDoZwolnienia + 1));
    dllDoZwolnienia[liczbaDllDoZwolnienia++] = uchwytDll;
    printf("Koniec wczytywania\n");
}

void wezwij()
{

}

void tlumacz()
{
    printf("Rozpoczeto tlumaczenie\n");
    fseek(zrodlo, 0, SEEK_END);
    size_t rozmiar = ftell(zrodlo);
    rewind(zrodlo);

    poczatek = (char*)malloc(rozmiar + 1);
    if(!poczatek) niezbywalnyBlad("Brak pamieci");
    fread(poczatek, 1, rozmiar, zrodlo);
    poczatek[rozmiar] = '\0';
    char* obecny = poczatek;
    koniec = poczatek + rozmiar;

    dlugoscTrzymakaNazwTymczasowych = snprintf(NULL, 0, "!t%zu", SIZE_MAX) + 1;
    trzymakNazwTymaczasowych = malloc(dlugoscTrzymakaNazwTymczasowych);
    if(!trzymakNazwTymaczasowych) niezbywalnyBlad("Brak pamieci");

    size_t odonosnikObszaruGlownego = wczytujJakoObszarGlowny(&obecny);
    printf("Zwrocono obszar glowny o odnosniku %zu\n", odonosnikObszaruGlownego);

    free(trzymakNazwTymaczasowych);
    trzymakNazwTymaczasowych = NULL;
}

void czytaj()
{
    przelaczNaObszar(zmienne[23], NULL);
    *wskaznikOdnosnikaPolecenia = 0;
    printf("Przelaczano na obszar glowny\n");
    //niezbywalnyBlad("Koniec testu");
}

void zakoncz()
{
    for(size_t i = 1; i < liczbaZmiennych; i++)
    {
        if(zmienne[i]) free(zmienne[i]);
    }
    if(zmienne) free(zmienne);
    for(size_t i = 1; i < liczbaNazwZmiennych; i++)
    {
        if(nazwyZmiennych[i]) free(nazwyZmiennych[i]);
    }
    if(nazwyZmiennych) free(nazwyZmiennych);
    for(int i = 0; i < liczbaDllDoZwolnienia; i++)
    {
        if(dllDoZwolnienia[i]) FreeLibrary(dllDoZwolnienia[i]);
    }
    if(dllDoZwolnienia) free(dllDoZwolnienia);
    if(poczatek) free(poczatek);
    if(polecenia) free(polecenia);
    if(trzymakNazwTymaczasowych) free(trzymakNazwTymaczasowych);
    if(zrodlo) fclose(zrodlo);
    if(pekCzytelnegoPocztu) fclose(pekCzytelnegoPocztu);
    printf("Pomyslnie zakonczono dzialanie\n");
    getchar();
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        niezbywalnyBlad("Nie podano zrodla");
    }
    zrodlo = fopen(argv[1], "rb");
    if(zrodlo == NULL)
    {
        niezbywalnyBlad("Nie mozna otworzyc zrodla");
    }

    size_t dlugosc = strlen(argv[1]);

    size_t obszarPrzygotowawczy = 0;
    if(dlugosc > 4 && strcmp(argv[1] + dlugosc - 4, ".czp") == 0) przygotowanieDlaCzp();
    else if(dlugosc > 3 && strcmp(argv[1] + dlugosc - 3, ".cz") == 0) obszarPrzygotowawczy = przygotowanieDlaCz();
    else niezbywalnyBlad("Wymagany pek .czp lub .cz");
    printf("Zapisano obszar przygotowawczy na miejscu %zu\n", obszarPrzygotowawczy);

    char sciezka[MAX_PATH];
    strncpy(sciezka, argv[1], sizeof(sciezka));
    sciezka[sizeof(sciezka) - 1] = '\0';
    char dysk[_MAX_DRIVE], kat[_MAX_DIR];
    _splitpath(sciezka, dysk, kat, NULL, NULL);
    char folder[MAX_PATH];
    snprintf(folder, sizeof(folder), "%s%s", dysk, kat);
    SetCurrentDirectoryA(folder);

    polecenia = (Polecenie*)malloc(sizeof(Polecenie) * pojemnoscPolecen);
    if(!polecenia) niezbywalnyBlad("Brak pamieci");
    polecenia[0] = NULL;
    polecenia[1] = wezwij;
    polecenia[2] = tlumacz;
    polecenia[3] = czytaj;
    polecenia[4] = zakoncz;

    nazwyPolecen = (char**)malloc(sizeof(char*) * pojemnoscPolecen);
    nazwyPolecen[0] = NULL;
    nazwyPolecen[1] = "wezwij_dom";
    nazwyPolecen[2] = "tlumacz_dom";
    nazwyPolecen[3] = "czytaj_dom";
    nazwyPolecen[4] = "zakoncz_dom";

    liczbaPolecen = 5;

    size_t odnosnikaPolecenia = 0;
    wskaznikOdnosnikaPolecenia = &odnosnikaPolecenia;

    printf("Wzywanie podstawy.dll\n");
    wzywanie("podstawy.dll");

    przelaczNaObszar(zmienne[obszarPrzygotowawczy], NULL);

    printf("Rozpoczeto wykonywanie\n");
    while(TRUE)
    {
        printf("Zaraz wykonamy polecenie %s (%d)", nazwyPolecen[poczet[*wskaznikOdnosnikaPolecenia]], poczet[*wskaznikOdnosnikaPolecenia]);
        getchar();
        polecenia[poczet[(*wskaznikOdnosnikaPolecenia)++]]();
        printf("Powrot do petli w main\n");
        getchar();
    }

    getchar();
    return 0;
}