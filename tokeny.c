#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LEN 100

typedef enum {
    TOKEN_LICZBA,
    TOKEN_IDENTYFIKATOR,
    TOKEN_OPERATOR,
    TOKEN_NAWIAS,
    TOKEN_BLAD,
    TOKEN_KONIEC
} TokenTyp;

typedef struct {
    TokenTyp typ;
    char wartosc[MAX_TOKEN_LEN];
} Token;

Token skaner(const char **wejscie)
{
    Token token;
    token.wartosc[0] = '\0';

    while (isspace(**wejscie)) (*wejscie)++;

    if (**wejscie == '\0') {
        token.typ = TOKEN_KONIEC;
        return token;
    }

    if (isdigit(**wejscie)) {
        token.typ = TOKEN_LICZBA;
        int i = 0;
        while (isdigit(**wejscie) && i < MAX_TOKEN_LEN - 1) {
            token.wartosc[i++] = *(*wejscie)++;
        }
        token.wartosc[i] = '\0';
        return token;
    }

    if (isalpha(**wejscie)) {
        token.typ = TOKEN_IDENTYFIKATOR;
        int i = 0;
        while ((isalnum(**wejscie) || **wejscie == '_') && i < MAX_TOKEN_LEN - 1) {
            token.wartosc[i++] = *(*wejscie)++;
        }
        token.wartosc[i] = '\0';
        return token;
    }

    if (**wejscie == '+' || **wejscie == '-' || **wejscie == '*' || **wejscie == '/') {
        token.typ = TOKEN_OPERATOR;
        token.wartosc[0] = *(*wejscie)++;
        token.wartosc[1] = '\0';
        return token;
    }

    if (**wejscie == '(' || **wejscie == ')') {
        token.typ = TOKEN_NAWIAS;
        token.wartosc[0] = *(*wejscie)++;
        token.wartosc[1] = '\0';
        return token;
    }

    token.typ = TOKEN_BLAD;
    token.wartosc[0] = *(*wejscie)++;
    token.wartosc[1] = '\0';
    return token;
}

void wypiszToken(Token token) {
    switch (token.typ) {
        case TOKEN_LICZBA: printf("(LICZBA, %s)\n", token.wartosc); break;
        case TOKEN_IDENTYFIKATOR: printf("(IDENTYFIKATOR, %s)\n", token.wartosc); break;
        case TOKEN_OPERATOR: printf("(OPERATOR, %s)\n", token.wartosc); break;
        case TOKEN_NAWIAS: printf("(NAWIAS, %s)\n", token.wartosc); break;
        case TOKEN_BLAD: printf("[BŁĄD] Nieznany znak: %s\n", token.wartosc); break;
        case TOKEN_KONIEC: break;
    }
}

int main() {
    char wyrazenie[256];

    fgets(wyrazenie, sizeof(wyrazenie), stdin);

    wyrazenie[strcspn(wyrazenie, "\n")] = '\0';

    const char *ptr = wyrazenie;
    Token token;

    do
    {
        token = skaner(&ptr);
        wypiszToken(token);
    }
    while (token.typ != TOKEN_KONIEC);

    return 0;
}
