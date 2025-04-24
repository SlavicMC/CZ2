# Autorzy:
Maciej Cieżak, Seweryn Fronc

# Pliki repozytorium:
- `czytnik.c` - główny program języka, podaje się mu plik w formie kodu źródłowego (.cz) lub kodu wykonywalnego (.czp)
- `drzewiarz.y` - kod parsera uruchamiany przez funkcję `robDrzewo()`, korzystać z tego ma `czytnik.c`
- `dzielone.h` - deklaracje przekazywane między `czytnik.c` a drzewiarz.y

Pozostałe pliki to nieistotne notatki i pomoce.

# Opis działania:
`czytnik.c` otrzymuje plik z kodem i w zależności od formatu rozróżnia co z nim zrobić. Na starcie przygotowuje podstawowe zmienne oraz na bazie podstawowej zmiennej `obszar` tworzy oraz wykonuje kod który w zależności od formatu przetłumaczy i przeczyta podany kod źródłowy (.cz) lub przeczyta kod wykonywalny (.czp). Zmienne są wczytywane za pomocą indywidualnych funkcji 

# Rodzaje zmiennych:
- `obszar` - (obs) połączenie klasy, funkcji i obiektu w jedno. Może przechowywać zmienne i kod wykonywalny. Cały kod źródłowy wczytywany jest jako duży obszar i funkcja wczytująca obszary odpowiedzialna jest za przetłumaczenie go (przy użyciu `drzewiarz.y`). Kluczowa zmienna języka;
- `liczba` - (lic) liczba całkowita o dowolnej precyzji. Używa biblioteki GMP;
- `pismo` - (pis) zmienna tekstowa;
- `tak-lub-nie` - (tln) boolean. W założeniu będzie mógł przechowywać 8 wartości na bajt (pozyskiwane jak z tablicy);

Wszystkie zmienne w języku korzystają z tej samej struktury C i są wektorami.
