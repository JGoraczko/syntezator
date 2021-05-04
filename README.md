# Projekt SCZR 2021L - Syntezator

## Autorzy
Jakub Gorączko i Łukasz Reszka

## Treść projektu
Celem projektu jest implementacja prostego **syntezatora**. Dźwięki dostarczane są do syntezatora na podstawie wejścia **MIDI**, lub czytane z pliku. 
Za pomocą specjalnej aplikacji użytkownik określa parametry przetwarzania wejściowego sygnału – może np.: wybrać rodzaj fali (trójkątna, prostokątna itp.) i zastosować filtr (np.: dolnoprzepustowy). 
Na końcu przetworzony sygnał jest przesyłany na wyjście audio karty dźwiękowej.

Syntezator zostanie zrealizowany w języku **C++** dla systemu **Ubuntu 20.04 LTS**. Do jego
implementacji zostanie prawdopodobnie użyta **ALSA Library API** (zapewnia interfejs do
sterowników ALSA).

## Schemat działania
Na implementowany syntezator składają się 4 procesy:
- Proces A pobiera informacje w formacie MIDI i generuje dźwięk na ich podstawie.
- Proces B przetwarza próbki nakładając filtr.
- Proces C przesyła na wyjście audio karty dźwiękowej.
- Proces D jest nadzorcą i obsługuje interfejs użytkownika.

Przesyłane bloki próbek będą miały znaczniki czasowe. Odpowiednie czasy przetwarzania
będą zapisywane do plików co pewien okres przez oddzielne wątki procesów B i C.

## Planowane doświadczenia
Celem doświadczeń przeprowadzanych nad zaimplementowanym syntetyzatorem będzie
pomiar latencji w realizowanym systemie czasu rzeczywistego w zależności od:
- Metod komunikacji między procesami – bufor w pamięci współdzielonej czy kolejki
komunikatów,
- Sposobu szeregowania – zmiana priorytetów procesów,
- Wielkości przesyłanych bloków danych,
- Okresu zapisu danych diagnostycznych.

Wszystkie zebrane wyniki zostaną udokumentowane na odpowiednich wykresach, na
podstawie których wyciągniemy wnioski i spostrzeżenia.
