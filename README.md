# Dokumentacja języka programowania CZ

---

## Autorzy

- Maciej Cieżak - mciezak@student.agh.edu.pl
- Seweryn Fronc - sfronc@student.agh.edu.pl

---

## Założenia programu

### Ogólne cele

Stworzenie prostego polskiego języka programowania z bytecodem

### Rodzaj translatora

**Interpreter**

### Planowany wynik działania programu

**Kompilacja kodu CZ na CZP i/lub interpretacja CZP**

### Język implementacji

**C**

### Realizacja

- **Skaner - własny**
- **Parser - Yacc**

---

## Tabela tokenów

### Domyślne rodzaje danych

| Ród           | Opis                     |
|---------------|--------------------------|
| `obs`         | obszar (funkcja/obiekt)  |
| `lic`         | liczba całkowita         |
| `pis`         | ciąg znaków              |
| `tln`         | boolean                  |
| `nck`         | liczba niecałkowita      |
| `cig`         | ciąg zmiennych (wektor)  |

Należy zaznaczyć, że:
- ród to odnośnik na bezobszarową zmienną tworzoną na początku która ma przypięte metody zarządzające rodem
- ze względu na dynamiczny charakter języka parser nie rozpoznaje rodów zmiennych
- takie podejście umożliwia proste tworzenie własnych rodzajów danych poprzez utworzenie zmiennej rodowej i zdefiniowanie jej funkcji


### Działania

| Token                 | Opis                    | Przykład |
|-----------------------|-------------------------|----------|
| `DODAJ`               | dodawanie               | `+`      |
| `ODEJMIJ`             | odejmowanie             | `-`      |
| `MNOZ`                | mnożenie                | `*`      |
| `DZIEL`               | dzielenie               | `/`      |
| `RESZTA`              | reszta z dzielenia      | `%`      |
| `POTEGA`              | potęgowanie             | `**`     |
| `ZWIEKSZ`             | inkrementacja           | `++`     |
| `ZMNIEJSZ`            | dekrementacja           | `--`     |
| `ROWNE`               | równe                   | `==`     |
| `ROZNE`               | nierówne                | `!=`     |
| `MNIEJSZE`            | mniejsze                | `<`      |
| `MNIEJSZE_BADZ_ROWNE` | mniejsze bądź równe     | `<=`     |
| `WIEKSZE`             | większe                 | `>`      |
| `WIEKSZE_BADZ_ROWNE`  | większe bądź równe      | `>=`     |
| `I`                   | operator logiczny "AND" | `&&`     |
| `LUB`                 | operator logiczny "OR"  | `\|\|`   |
| `NIE`                 | operator logiczny "NOT" | `!`      |
| `NADAJ`               | przypisanie             | `=`      |
| `NDODAJ`              | przypisanie dodawania   | `+=`     |
| `NODEJMIJ`            | przypisanie odejmowania | `-=`     |
| `NMNOZ`               | przypisanie mnożenia    | `*=`     |
| `NDZIEL`              | przypisanie dzielenia   | `/=`     |
| `NRESZTA`             | przypisanie reszty      | `%=`     |

Możliwe że język będzie miał możliwość dodawania własnych działań


### Inne znaki

| Token           | Opis                          | Przykład |
|-----------------|-------------------------------|----------|
| `LNAWIAS`       | nawias otwierający            | `(`      |
| `PNAWIAS`       | nawias zamykający             | `)`      |
| `LLUBIANKA`     | nawias kwadratowy otwierający | `[`      |
| `PLUBIANKA`     | nawias kwadratowy zamykający  | `]`      |
| `LSPIECIE`      | nawias klamrowy otwierający   | `{`      |
| `PSPIECIE`      | nawias klamrowy zamykający    | `}`      |
| `SREDNIK`       | średnik                       | `;`      |


### Słowa kluczowe

| Token          | Opis                              | Przykład       |
|----------------|-----------------------------------|----------------|
| `JESLI`        | wyrażenie jeśli                   | `jesli`        |
| `IJESLI`       | wyrażenie w przeciwnym razie jeśli| `inaczej jesli`|
| `INACZEJ`      | wyrażenie w przeciwnym razie      | `inaczej`      |
| `POKI`         | wyrażenie póki                    | `poki`         |
| `POTY`         | wyrażenie póty (razem z póki)     | `poty`         |


### Inne tokeny

| Token        | Opis                    | Przykład                 |
|--------------|-------------------------|--------------------------|
| `###`        | koniec obszaru głównego | `###`                    |

---

## Gramatyka

[Gramatyka](./drzewiarz.y)

## Przykładowy kod źródłowy

[Przykład](./przyklad.cz)