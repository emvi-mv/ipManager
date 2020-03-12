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

#include "db.h"
#include "cli.h"

DB database;

int main(int argc, char** argv) {
    CCli cli;
    cli.run();
}
