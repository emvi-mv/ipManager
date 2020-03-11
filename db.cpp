/*
 * db.cpp
 *
 *  Created on: 24 lut 2020
 *      Author: emvi
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "db.h"

const bool CIPDB::add(const CIp addr, const CNetmask netmask) {
    auto result = mRanges.emplace(makeKey(addr, netmask));
    return std::get<bool>(result);
}

const bool CIPDB::del(const CIp addr, const CNetmask netmask) {
    return mRanges.erase(makeKey(addr, netmask));
}

/* Algorytm wyszukiwania
 * Z opisu zadania wynika, że należy wyszukiwać najbardziej dopasowaną - najdłuższą maskę.
 * Algorytm wyszukuje kolejne prefiksy zgodne z podanym adresem oraz maską
 * zaczynając od /32 aż do /0. Aby prefiksy w bazie i wyszukiwane dało się dopasować,
 * końcowe bity muszą być albo wyzerowane albo ustawione. W tym rozwiązaniu zastosowałem
 * ustawianie. Prefiksy w bazie i wyszukiwane zamieniane są na adresy broadcastowe
 * 10.0.0.0/24 -> 10.0.0.255/24
 * Według takiego algorytmu, pierwszy dopasowany prefiks jest najbardziej dokładnym.
 * Taka metoda wyszukiwania powinna być szybsza od sekwencyjnego porównywania z kolejnymi
 * prefiksami w bazie. W najbardziej pesymistycznym przypadku trzeba wykonać 32 iteracje.
 *
 * W praktyce może się okazać, że większość wyszukiwań będzie nietrafionych więc tych
 * najbardziej czasochłonnych o 32 iteracjach. Można te przypadki przyspieszyć ale zabrakło
 * mi czasu na zaimplementowanie takiej optymalizacji.
 * Należałoby wstępnie zweryfikować czy wyszukiwany adres w ogóle występuje wśród wszystkich
 * prefiksów w bazie. Pełna pula adresów zawiera ich 2^32. Można dodatkowo przechowywać
 * mapę wpisanych adresów IP. Przyjmując jeden bit na adres mapa zajętości zmieści się w 512MB
 * pamięci. Przy dodawaniu prefiksu wystarczyłoby ustawić wszystkie bity odpowiadające
 * dodawanemu zakresowi. Przy usuwaniu prefiksu trzeba by od nowa sprawdzić wszystkie bity
 * usuwanego zakresu i wyszukać, które z nich nadal są zgodne z jakimkolwiek innym prefiksem.
 * Usuwanie stało by się dużo bardziej czasochłonną operacją. Zyskiem byłoby o wiele szybsze
 * sprawdzenie czy poszukiwany IP występuje w bazie i czy warto rozpoczynać wspomniane wyżej
 * iteracje.
 */
COptionalRangeKey CIPDB::check(const CIp addr) {
    COptionalRangeKey found;
    for (int netmask = 32; netmask >= 0; --netmask) {
        auto key = makeKey(addr, netmask);
        if (mRanges.count(key)) {
            found.emplace(key);
            break;
        }
    }
    return std::move(found);
}

void CIPDB::dump() {
    std::for_each(mRanges.begin(), mRanges.end(), [this](auto range) {
        std::cout << dumpKey(range) << std::endl;
    });
}

void CIPDB::bench(const unsigned int count, const CAddresses addresses) {
    for (auto i = count; i--;)
        std::for_each(addresses.begin(), addresses.end(), [this] (auto addr) { check(addr); });
}

std::string CIPDB::dumpKey(const CRangeKey key) const {
    std::stringstream s;
    s << inet_ntoa(in_addr{htonl(static_cast<in_addr_t>(key & 0xffffffff))}) << "/" << (key >> 32);
    return s.str();
}
