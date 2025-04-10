%{
#include "dzielone.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
void yyerror(const char *s);

Czastka* czastki = NULL;
size_t odnosnikCzastki;
size_t liczbaCzastek;
Rozgalezienie* drzewo = NULL;

%}

%union {
    Zmienna* zmienna;
    GalazPodwojna* galazPodwojna;
    Rozgalezienie* rozgalezienie;
}

%token <zmienna> ZMIENNA
%token JESLI POKI
%token DODAJ ODEJMIJ MNOZ DZIEL NADAJ
%token SREDNIK LNAWIAS PNAWIAS LSPIECIE PSPIECIE
%type <rozgalezienie> wyrazenia
%type <galazPodwojna> wyrazenie dzialanie

%right NADAJ
%left DODAJ ODEJMIJ
%left MNOZ DZIEL

%start calosc

%%

calosc:
    wyrazenia   { drzewo = $1; }
    ;

wyrazenia:
      /* nic */             { $$ = utworzRozgalezienie(); }
    | wyrazenia wyrazenie   { dodajGalaz($1, $2); $$ = $1; }
    ;

wyrazenie:
      dzialanie SREDNIK                                             { $$ = $1; }
    | JESLI LNAWIAS dzialanie PNAWIAS LSPIECIE wyrazenia PSPIECIE   { $$ = utworzWyrazenieKluczowe(JESLI, $3, $6) }
    | JESLI LNAWIAS dzialanie PNAWIAS wyrazenie                     { $$ = utworzWyrazenieKluczowe(JESLI, $3, $5) }
    | POKI LNAWIAS dzialanie PNAWIAS LSPIECIE wyrazenia PSPIECIE    { $$ = utworzWyrazenieKluczowe(POKI, $3, $6) }
    | POKI LNAWIAS dzialanie PNAWIAS wyrazenie                      { $$ = utworzWyrazenieKluczowe(POKI, $3, $5) }
    ;

dzialanie:
      dzialanie DODAJ dzialanie     { $$ = utworzDzialanie(DODAJ, $1, $3); }
    | dzialanie ODEJMIJ dzialanie   { $$ = utworzDzialanie(ODEJMIJ, $1, $3); }
    | dzialanie MNOZ dzialanie      { $$ = utworzDzialanie(MNOZ, $1, $3); }
    | dzialanie DZIEL dzialanie     { $$ = utworzDzialanie(DZIEL, $1, $3); }
    | LNAWIAS dzialanie PNAWIAS     { $$ = $2; }
    | dzialanie NADAJ dzialanie     { $$ = utworzDzialanie(NADAJ, $1, $3); }
    | ZMIENNA                       { $$ = utworzZmienna($1); }
    ;

%%

GalazPodwojna* utworzZmienna(Zmienna* z)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = ZMIENNA;
    galaz->wartosc = z;
    galaz->lewa = NULL;
    galaz->prawa = NULL;
    return galaz;
}

Rozgalezienie* utworzRozgalezienie()
{
    Rozgalezienie* wyrazenia = malloc(sizeof(Rozgalezienie));
    if (!wyrazenia)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    wyrazenia->rozmiar = 0;
    wyrazenia->pojemnosc = 4;
    wyrazenia->galezie = malloc(wyrazenia->pojemnosc * sizeof(GalazPodwojna*));
    if (!wyrazenia->galezie)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    return wyrazenia;
}

GalazPodwojna* utworzDzialanie(int dz, GalazPodwojna* lewy, GalazPodwojna* prawy)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = dz;
    galaz->wartosc = NULL;
    galaz->lewa = lewy;
    galaz->prawa = prawy;
    return galaz;
}

GalazPodwojna* utworzWyrazenieKluczowe(int dz, GalazPodwojna* lewy, Rozgalezienie* prawy)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = dz;
    galaz->wartosc = NULL;
    galaz->lewa = lewy;
    galaz->prawa = prawy;
    return galaz;
}

int yylex(void)
{
    if (odnosnikCzastki >= liczbaCzastek) return 0;
    Czastka czastka = czastki[odnosnikCzastki++];
    if(czastka.rodzaj == 0) // nieznany (nawiasy, klamry i średniki)
    {
        yylval.zmienna = NULL;
        switch (((char*)czastka.zawartosc)[0]) // nieznany to zawsze jeden bajt
        {
            case ';': return SREDNIK;
            case '(': return LNAWIAS;
            case ')': return PNAWIAS;
            case '{': return LSPIECIE;
            case '}': return PSPIECIE;
            default:
                fprintf(stderr, "Nieznany token\n");
                exit(EXIT_FAILURE);
        }
    }
    if(czastka.rodzaj == 1) // słowo kluczowe
    {
        yylval.zmienna = NULL;
        const int kluczowe[] = {JESLI, POKI};
        for(int i = 0; i < sizeof(kluczowe) / sizeof(int); i++)
        {
            if((char*)czastka.zawartosc == slowaKluczowe[i]) return kluczowe[i]; // porównywanie wskaźników a nie stringów
        }

        for (int i = 0; i < sizeof(kluczowe) / sizeof(int); i++) // jeśli nic się nie znajdzie porównujemy zawartość dla pewności
        {
            if(strcmp((char*)czastka.zawartosc, slowaKluczowe[i]) == 0) return kluczowe[i];
        }
        fprintf(stderr, "Nieznany token\n");
        exit(EXIT_FAILURE);
    }
    if(czastka.rodzaj == 3) // działanie
    {
        yylval.zmienna = NULL;
        const int dzialaniowe[] = {DODAJ, ODEJMIJ, MNOZ, DZIEL, NADAJ};

        for(int i = 0; i < sizeof(dzialaniowe) / sizeof(int); i++)
        {
            if((char*)czastka.zawartosc == dzialania[i]) return dzialaniowe[i]; // porównywanie wskaźników a nie stringów
        }

        for (int i = 0; i < sizeof(dzialaniowe) / sizeof(int); i++) // jeśli nic się nie znajdzie porównujemy zawartość dla pewności
        {
            if(strcmp((char*)czastka.zawartosc, dzialania[i]) == 0) return dzialaniowe[i];
        }
        fprintf(stderr, "Nieznany token\n");
        exit(EXIT_FAILURE);
    }
    if(czastka.rodzaj == 2) // zmienna
    {
        yylval.zmienna = (Zmienna*)czastka.zawartosc;
        return ZMIENNA;
    }
    fprintf(stderr, "Nieznany token\n");
    exit(EXIT_FAILURE);
}

void yyerror(const char *s)
{
    fprintf(stderr, "Blad robienia drzewa: %s\n", s);
}

Rozgalezienie* robDrzewo(Czastka* cz, size_t l)
{
    czastki = cz;
    odnosnikCzastki = 0;
    liczbaCzastek = l;
    yyparse();
    return drzewo;
}

void dodajGalaz(Rozgalezienie* rozgalezienie, GalazPodwojna* galaz)
{
    if(rozgalezienie->pojemnosc == rozgalezienie->rozmiar)
    {
        rozgalezienie->pojemnosc *= 2;
        rozgalezienie->galezie = realloc(rozgalezienie->galezie, rozgalezienie->pojemnosc * sizeof(GalazPodwojna*));
        if(!rozgalezienie->galezie)
        {
            fprintf(stderr, "Brak pamieci\n");
            exit(EXIT_FAILURE);
        }
    }
    rozgalezienie->galezie[rozgalezienie->rozmiar++] = galaz;
}

void zetnijGalaz(GalazPodwojna* galaz)
{
    if (galaz == NULL) return;
    zetnijGalaz(galaz->lewa);
    if(galaz->rodzaj == JESLI || galaz->rodzaj == POKI) zetnijRozgalezienie(galaz->prawa);
    else zetnijGalaz(galaz->prawa);
    free(galaz);
}

void zetnijRozgalezienie(Rozgalezienie* rozgalezienie)
{
    if (rozgalezienie == NULL) return;
    for(int i = 0; i < rozgalezienie->rozmiar; i++)
    {
        zetnijGalaz(rozgalezienie->galezie[i]);
    }
    free(rozgalezienie->galezie);
    free(rozgalezienie);
}