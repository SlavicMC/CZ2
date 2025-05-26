#ifndef DZIELONE_H
#define DZIELONE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>

#include "powszechne.h"

extern char* slowaKluczowe[];
extern size_t liczbaSlowKluczowych;
extern char* dzialania[];
extern size_t liczbaDzialan;

extern int zmienniana;
extern int poleceniana;
extern int wykonaniana;
extern int kluczowe[];
extern int dzialaniowe[];

Rozgalezienie* robDrzewo(Czastka* cz, size_t l);
void zetnijGalaz(GalazPodwojna* galaz);

Rozgalezienie* utworzRozgalezienie();
Rozgalezienie* pojedynczeRozgalezienie(GalazPodwojna* galaz);
void dodajGalaz(Rozgalezienie* rozgalezienie, GalazPodwojna* galaz);
void zetnijRozgalezienie(Rozgalezienie* rozgalezienie);

const char* nazwaRodzaju(int rodzaj);
void wypiszGalaz(GalazPodwojna* galaz, int wciecie);
void wypiszDrzewo(Rozgalezienie* rozgalezienie, int wciecie);

GalazPodwojna* utworzDzialanie(int dz, GalazPodwojna* lewy, void* prawy);
GalazPodwojna* utworzWyrazenieKluczowe(int dz, GalazPodwojna* lewy, Rozgalezienie* prawy);
GalazPodwojna* utworzZmiennaJakoGalaz(size_t z);
GalazPodwojna* utworzPolecenieJakoGalaz(char* p);

#ifdef __cplusplus
}
#endif

#endif