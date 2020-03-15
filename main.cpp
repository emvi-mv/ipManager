/*
 * main.cpp
 *
 *  Created on: 24 lut 2020
 *      Author: emvi
 */

/* Przypuszczam, że podczas rozmowy optymalizacją która miałaby przyspieszyć algorytm
 * było zgromadzenie danych w drzewie Trie.
 * Musiałem niestety w tym celu użyć google-a. Nie przypominam sobie żebym kiedykolwiek
 * wcześniej słyszał o drzewach Trie.
 * Czy ten trop jest właściwy?
 */

/* Po zmianie sposobu zapisu danych na (potęga 2 określająca liczbę adresów):(pierwszy adres)
 * do wyszukania wystarczy jedno użycie std::find_if z odpowiednio przygotowanym warunkiem trafienia
 * find_if znajduje pierwszy w szeregu element spełniający warunek, więc aby znalazł
 * najbardziej dopasowany, prefiksy muszą być posortowane od najniższej potęgi określającej
 * liczbę adresów do najwyższej.
 * Na moim komputerze przyspieszenie jest bliskie 10-ciokrotnemu względem poprzedniego
 * rozwiązania. Faktycznie wymyślenie takiego rozwiązania wydaje się w miarę proste.
 * Następnym etapem może być sprawdzenie czy użycie innego kontenera nie da szybszego efektu.
 */

/*
 * Użycie vectora przyspiesza wyszukiwanie jeszcze (o ile dobrze pamiętam) x2-x3.
 * Nieco lepszy efekt daje forward_list dlatego zostawiłem ten kontener.
 */

#include "db.h"
#include "cli.h"

DB database;

int main(int argc, char** argv) {
    CCli cli;
    cli.run();
}
