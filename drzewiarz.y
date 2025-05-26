%{
#include "dzielone.h"
#include "powszechne.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int yylex(void);
void yyerror(const char *s);

Czastka* czastki = NULL;
size_t odnosnikCzastki;
size_t liczbaCzastek;
Rozgalezienie* drzewo = NULL;

#define WYKONAJ 101

%}

%union {
    size_t zmienna;
    char* polecenie;
    GalazPodwojna* galazPodwojna;
    Rozgalezienie* rozgalezienie;
}

%token <zmienna> ZMIENNA
%token <polecenie> POLECENIE
%token JESLI POKI
%token DODAJ ODEJMIJ MNOZ DZIEL RESZTA
%token NADAJ NDODAJ NODEJMIJ NMNOZ NDZIEL NRESZTA
%token NIE ROWNE ROZNE WIEKSZE MNIEJSZE WIEKSZE_BADZ_ROWNE MNIEJSZE_BADZ_ROWNE
%token I LUB
%token SREDNIK PRZECINEK LNAWIAS PNAWIAS LSPIECIE PSPIECIE
%type <rozgalezienie> wyrazenia dzialania
%type <galazPodwojna> wyrazenie dzialanie wywolanie

%right NADAJ NDODAJ NODEJMIJ NMNOZ NDZIEL NRESZTA
%left I LUB
%left ROWNE ROZNE WIEKSZE MNIEJSZE WIEKSZE_BADZ_ROWNE MNIEJSZE_BADZ_ROWNE
%left DODAJ ODEJMIJ
%left MNOZ DZIEL RESZTA
%left NIE

%start calosc

%%

calosc:
    wyrazenia   { drzewo = $1; }
    //| dzialania   { drzewo = $1; }
    ;

wyrazenia:
      /* nic */             { $$ = utworzRozgalezienie(); }
    | wyrazenia wyrazenie   { dodajGalaz($1, $2); $$ = $1; }
    ;

wyrazenie:
      dzialanie SREDNIK                                             { $$ = $1; }
    | POLECENIE                                                     { $$ = utworzPolecenieJakoGalaz($1); }
    | JESLI LNAWIAS dzialanie PNAWIAS LSPIECIE wyrazenia PSPIECIE   { $$ = utworzWyrazenieKluczowe(JESLI, $3, $6); }
    | JESLI LNAWIAS dzialanie PNAWIAS wyrazenie                     { $$ = utworzWyrazenieKluczowe(JESLI, $3, pojedynczeRozgalezienie($5)); }
    | POKI LNAWIAS dzialanie PNAWIAS LSPIECIE wyrazenia PSPIECIE    { $$ = utworzWyrazenieKluczowe(POKI, $3, $6); }
    | POKI LNAWIAS dzialanie PNAWIAS wyrazenie                      { $$ = utworzWyrazenieKluczowe(POKI, $3, pojedynczeRozgalezienie($5)); }
    ;

dzialania:
      dzialanie                         { $$ = utworzRozgalezienie(); dodajGalaz($$, $1); }
    | dzialania PRZECINEK dzialanie     { dodajGalaz($1, $3); $$ = $1; }
    ;

dzialanie:
      dzialanie NADAJ dzialanie                 { $$ = utworzDzialanie(NADAJ, $1, $3); }
    | dzialanie NDODAJ dzialanie                { $$ = utworzDzialanie(NDODAJ, $1, $3); }
    | dzialanie NODEJMIJ dzialanie              { $$ = utworzDzialanie(NODEJMIJ, $1, $3); }
    | dzialanie NMNOZ dzialanie                 { $$ = utworzDzialanie(NMNOZ, $1, $3); }
    | dzialanie NDZIEL dzialanie                { $$ = utworzDzialanie(NDZIEL, $1, $3); }
    | dzialanie NRESZTA dzialanie               { $$ = utworzDzialanie(NRESZTA, $1, $3); }
    | dzialanie DODAJ dzialanie                 { $$ = utworzDzialanie(DODAJ, $1, $3); }
    | dzialanie ODEJMIJ dzialanie               { $$ = utworzDzialanie(ODEJMIJ, $1, $3); }
    | dzialanie MNOZ dzialanie                  { $$ = utworzDzialanie(MNOZ, $1, $3); }
    | dzialanie DZIEL dzialanie                 { $$ = utworzDzialanie(DZIEL, $1, $3); }
    | dzialanie RESZTA dzialanie                { $$ = utworzDzialanie(RESZTA, $1, $3); }
    | NIE dzialanie                             { $$ = utworzDzialanie(NIE, $2, NULL); }
    | dzialanie ROWNE dzialanie                 { $$ = utworzDzialanie(ROWNE, $1, $3); }
    | dzialanie ROZNE dzialanie                 { $$ = utworzDzialanie(ROZNE, $1, $3); }
    | dzialanie WIEKSZE dzialanie               { $$ = utworzDzialanie(WIEKSZE, $1, $3); }
    | dzialanie MNIEJSZE dzialanie              { $$ = utworzDzialanie(MNIEJSZE, $1, $3); }
    | dzialanie WIEKSZE_BADZ_ROWNE dzialanie    { $$ = utworzDzialanie(WIEKSZE_BADZ_ROWNE, $1, $3); }
    | dzialanie MNIEJSZE_BADZ_ROWNE dzialanie   { $$ = utworzDzialanie(MNIEJSZE_BADZ_ROWNE, $1, $3); }
    | dzialanie I dzialanie                     { $$ = utworzDzialanie(I, $1, $3); }
    | dzialanie LUB dzialanie                   { $$ = utworzDzialanie(LUB, $1, $3); }
    | wywolanie                                 { $$ = $1; }
    | LNAWIAS dzialanie PNAWIAS                 { $$ = $2; }
    | ZMIENNA                                   { $$ = utworzZmiennaJakoGalaz($1); }
    ;

wywolanie:
      ZMIENNA LNAWIAS dzialania PNAWIAS     { $$ = utworzDzialanie(WYKONAJ, utworzZmiennaJakoGalaz($1), $3); }
    | ZMIENNA LNAWIAS PNAWIAS               { $$ = utworzDzialanie(WYKONAJ, utworzZmiennaJakoGalaz($1), utworzRozgalezienie()); }

%%

int zmienniana = ZMIENNA;
int poleceniana = POLECENIE;
int wykonaniana = WYKONAJ;
int kluczowe[] = { 0, JESLI, POKI};
size_t liczbaSlowKluczowych = sizeof(kluczowe)/sizeof(kluczowe[0]);
// char* dzialania[] = {"==", "!=", ">=", "<=", "||", "&&", "+=", "-=", "*=", "/=", "%=", "+", "-", "*", "/", "%", "=", "!", ">", "<"};
int dzialaniowe[] = {ROWNE, ROZNE, WIEKSZE_BADZ_ROWNE, MNIEJSZE_BADZ_ROWNE, LUB, I, NDODAJ, NODEJMIJ, NMNOZ, NDZIEL, NRESZTA, DODAJ, ODEJMIJ, MNOZ, DZIEL, RESZTA, NADAJ, NIE, WIEKSZE, MNIEJSZE};
size_t liczbaDzialan = sizeof(dzialaniowe)/sizeof(dzialaniowe[0]);

GalazPodwojna* utworzZmiennaJakoGalaz(size_t z)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = ZMIENNA;
    galaz->wartosc = (uintptr_t)z;
    galaz->lewa = NULL;
    galaz->prawa = NULL;
    return galaz;
}

GalazPodwojna* utworzPolecenieJakoGalaz(char* p)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = POLECENIE;
    galaz->wartosc = (uintptr_t)p;
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
    wyrazenia->pojemnosc = 2;
    wyrazenia->galezie = malloc(wyrazenia->pojemnosc * sizeof(GalazPodwojna*));
    if (!wyrazenia->galezie)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    return wyrazenia;
}

Rozgalezienie* pojedynczeRozgalezienie(GalazPodwojna* galaz)
{
    Rozgalezienie* r = utworzRozgalezienie();
    dodajGalaz(r, galaz);
    return r;
}


GalazPodwojna* utworzDzialanie(int dz, GalazPodwojna* lewy, void* prawy)
{
    GalazPodwojna* galaz = malloc(sizeof(GalazPodwojna));
    if(!galaz)
    {
        fprintf(stderr, "Brak pamieci\n");
        exit(EXIT_FAILURE);
    }
    galaz->rodzaj = dz;
    galaz->wartosc = 0;
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
    galaz->wartosc = 0;
    galaz->lewa = lewy;
    galaz->prawa = prawy;
    return galaz;
}

int yylex(void)
{
    //printf("yylex rusza!\n");
    if(odnosnikCzastki >= liczbaCzastek) return 0;
    Czastka czastka = czastki[odnosnikCzastki++];
    if(czastka.rodzaj == 0) // nieznany (nawiasy, klamry i średniki)
    {
        yylval.zmienna = 0;
        //printf("Wyslano nieznany: %c\n", ((char*)czastka.zawartosc)[0]);
        //getchar();
        switch (*((char*)czastka.zawartosc)) // nieznany to zawsze jeden bajt
        {
            case ';': return SREDNIK;
            case ',': return PRZECINEK;
            case '(': return LNAWIAS;
            case ')': return PNAWIAS;
            case '{': return LSPIECIE;
            case '}': return PSPIECIE;
            default:
                fprintf(stderr, "Nieznany token\n");
                getchar();
                exit(EXIT_FAILURE);
        }
    }
    else if(czastka.rodzaj == 1) // słowo kluczowe
    {
        yylval.zmienna = 0;
        for(int i = 1; i < sizeof(kluczowe) / sizeof(int); i++)
        {
            if((char*)czastka.zawartosc == slowaKluczowe[i]) 
            {
                //printf("Wyslano kluczowe: %s\n", slowaKluczowe[i]);
                //getchar();
                return kluczowe[i]; // porównywanie wskaźników a nie stringów
            }
        }

        for (int i = 0; i < sizeof(kluczowe) / sizeof(int); i++) // jeśli nic się nie znajdzie porównujemy zawartość dla pewności
        {
            if(strcmp((char*)czastka.zawartosc, slowaKluczowe[i]) == 0) return kluczowe[i];
        }
        fprintf(stderr, "Nieznany token\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    else if(czastka.rodzaj == 3) // działanie
    {
        yylval.zmienna = 0;

        for(int i = 0; i < sizeof(dzialaniowe) / sizeof(int); i++)
        {
            if((char*)czastka.zawartosc == dzialania[i]) 
            {
                //printf("Wyslano dzialanie: %s\n", dzialania[i]);
                //getchar();
                return dzialaniowe[i]; // porównywanie wskaźników a nie stringów
            }
        }

        for (int i = 0; i < sizeof(dzialaniowe) / sizeof(int); i++) // jeśli nic się nie znajdzie porównujemy zawartość dla pewności
        {
            if(strcmp((char*)czastka.zawartosc, dzialania[i]) == 0) return dzialaniowe[i];
        }
        fprintf(stderr, "Nieznany token\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    else if(czastka.rodzaj == 4) // surowe polecenie
    {
        yylval.polecenie = (char*)czastka.zawartosc;
        return POLECENIE;
    }
    else if(czastka.rodzaj == 2) // zmienna
    {
        yylval.zmienna = czastka.zawartosc;
        //printf("Wyslano zmienna: %zu\n", (size_t)czastka.zawartosc);
        //getchar();
        return ZMIENNA;
    }
    fprintf(stderr, "Nieznany token\n");
    getchar();
    exit(EXIT_FAILURE);
}

void yyerror(const char *s)
{
    fprintf(stderr, "Blad robienia drzewa: %s\n", s);
    getchar();
    exit(EXIT_FAILURE);
}

Rozgalezienie* robDrzewo(Czastka* cz, size_t l)
{
    czastki = cz;
    odnosnikCzastki = 0;
    liczbaCzastek = l;
    if(!czastki || !liczbaCzastek)
    {
        printf("Podano puste dane!\n");
        getchar();
        return NULL;
    }
    drzewo = NULL;
    printf("Rozpoczynamy?");
    getchar();
    int result = yyparse();
    if (result != 0)
    {
        fprintf(stderr, "Błąd podczas parsowania (kod: %d)\n", result);
        getchar();
        return NULL;
    }
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
    if(galaz == NULL) return;
    zetnijGalaz(galaz->lewa);
    if(galaz->rodzaj == JESLI || galaz->rodzaj == POKI || galaz->rodzaj == WYKONAJ) zetnijRozgalezienie(galaz->prawa);
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

const char* nazwaRodzaju(int rodzaj)
{
    if(rodzaj == ZMIENNA) return "ZMIENNA";
    if(rodzaj == WYKONAJ) return "WYKONANIE";
    if(rodzaj == POLECENIE) return "POLECENIE";
    for(size_t i = 0; i < sizeof(dzialaniowe)/sizeof(dzialaniowe[0]); i++)
    {
        if(dzialaniowe[i] == rodzaj) return dzialania[i];
    }
    for(size_t i = 0; i < sizeof(kluczowe)/sizeof(kluczowe[0]); i++)
    {
        if(kluczowe[i] == rodzaj) return slowaKluczowe[i];
    }
    return "???";
}

void wypiszGalaz(GalazPodwojna* galaz, int wciecie)
{
    if(!galaz) return;

    for(int i = 0; i < wciecie; i++) printf("  ");

    printf("Rodzaj: %s", nazwaRodzaju(galaz->rodzaj));

    if(galaz->rodzaj == ZMIENNA)
    {
        printf(", Zmienna #%zu\n", galaz->wartosc);
    }
    else if(galaz->rodzaj == WYKONAJ)
    {
        printf(", Zmienna #%zu\n", ((GalazPodwojna*)galaz->lewa)->wartosc);
    }
    else if(galaz->rodzaj == POLECENIE)
    {
        printf("\n");
    }
    else if(galaz->rodzaj == JESLI || galaz->rodzaj == POKI)
    {
        printf(" {\n");
        for (int i = 0; i < wciecie + 1; i++) printf("  ");
        printf("Warunek:\n");
        wypiszGalaz(galaz->lewa, wciecie + 2);
        for (int i = 0; i < wciecie + 1; i++) printf("  ");
        printf("Spiecie:\n");
        wypiszDrzewo((Rozgalezienie*)galaz->prawa, wciecie + 2);
        for (int i = 0; i < wciecie; i++) printf("  ");
        printf("}\n");
    }
    else
    {
        printf("\n");
        wypiszGalaz(galaz->lewa, wciecie + 1);
        wypiszGalaz(galaz->prawa, wciecie + 1);
    }
}

void wypiszDrzewo(Rozgalezienie* rozgalezienie, int wciecie)
{
    if(!rozgalezienie) return;

    for(size_t i = 0; i < rozgalezienie->rozmiar; i++)
    {
        wypiszGalaz(rozgalezienie->galezie[i], wciecie);
    }
}