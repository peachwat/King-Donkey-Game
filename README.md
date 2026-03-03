# King-Donkey-Game

# King Donkey Game

Projekt klasycznej gry zręcznościowej typu platformówka, zainspirowany legendarnym tytułem **Donkey Kong**. Gra została napisana w języku **C++** przy użyciu biblioteki **SDL2**.

## O Projekcie
Gracz wciela się w postać, która musi pokonywać kolejne platformy, wspinać się po drabinach i unikać toczących się beczek oraz innych przeszkód, aby dotrzeć do celu. Projekt skupia się na implementacji mechanik fizyki (grawitacja, skoki), detekcji kolizji oraz systemie animacji sprite'ów.

### Główne cechy:
* **System Żyć**: Gracz posiada określoną liczbę szans na ukończenie poziomu.
* **Różnorodne Poziomy**: Gra zawiera predefiniowane układy plansz o rosnącym poziomie trudności.
* **Mechanika Ruchu**: Płynne bieganie, wspinaczka po drabinach oraz skakanie.
* **Przeszkody**: Dynamicznie poruszające się beczki oraz przeciwnicy (Antagonista).

## Screeny z gry
Oto jak prezentuje się rozgrywka:

| Poziom 1 | Akcja na planszy |
| :---: | :---: |
| ![Screen 1](Img/screenshot1.jfif) | ![Screen 2](Img/screenshot2.jfif) |

## 🛠️ Technologia i Biblioteki
Projekt został zbudowany z wykorzystaniem następujących narzędzi:
* **Język**: C++
* **Biblioteka graficzna**: [SDL2 (Simple DirectMedia Layer)](https://www.libsdl.org/) w wersji 2.0.10.
* **Środowisko**: Visual Studio (pliki `.sln` i `.vcxproj` w zestawie).
* **Zasoby**: Grafiki w formacie `.bmp` (system autorski ładowania powierzchni).

## Struktura katalogów
* `Game/main.cpp`: Główna logika gry, silnik renderujący i obsługa zdarzeń.
* `Game/bmp/`: Folder zawierający wszystkie tekstury (gracz, beczki, drabiny, platformy).
* `Game/SDL2-2.0.10/`: Skonfigurowana biblioteka SDL2.
* `Img/`: Zrzuty ekranu prezentujące działanie aplikacji.

## Jak uruchomić
1. Sklonuj repozytorium na swój dysk.
2. Otwórz plik `Game/szablon2vs19.sln` w programie **Visual Studio 2019** (lub nowszym).
3. Upewnij się, że plik `SDL2.dll` znajduje się w katalogu wyjściowym projektu.
4. Skompiluj i uruchom projekt (klawisz F5).

## Sterowanie
| Klawisz | Akcja |
| --- | --- |
| `STRZAŁKI` | Poruszanie się i wspinanie |
| `SPACJA` | Skok |
| `ESC` | Wyjście z gry |
| `N` | Nowa gra |
| `ENTER` | Start / Potwierdzenie |

---
**Autor:** Evelina Rylova
**Rok powstania:** 2024
