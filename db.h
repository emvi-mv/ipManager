/*
 * db.h
 *
 *  Created on: 24 lut 2020
 *      Author: emvi
 */

#ifndef DB_H_
#define DB_H_

#include <cstdint>
#include <bitset>
#include <vector>
#include <optional>

using CIp = uint32_t;
using CNetmask = uint8_t;
using CRanges = std::bitset< 1l<<33 >;
using CRangeOffset = unsigned long;
using COptionalRangeOffset = std::optional<CRangeOffset>;
using CAddresses = std::vector<CIp>;

class CIPDB {
                /* wpisane prefiksy zawierają bitową maskę wszystkich możliwych prefiksów w adresacji IPv4
                 * kontener zawiera na początku flagi dla prefiksów o krótkich maskach
                 * /1 - 2^1 prefiksów
                 * /2 - 2^2 prefiksów
                 * /3 - 2^3 prefiksów
                 * ...
                 * /32 - 2^32 prefiksów
                 * łącznie potrzebnych jest 2^33-2 bitów */
    private:    CRanges mRanges;

                /* Obliczenie offsetu dla grupy prefiksów o podanej masce */
    private:    constexpr CRangeOffset prefixGroupOffset(const CNetmask mask) const { return (1l << mask) - 2; }

                /* Przekształcenie offsetu w kontenerze na maskę reprezentowanego prefiksu */
    private:    CNetmask offsetToNetmask(const CRangeOffset offset) const;

                /* Przekształcenie offsetu w kontenerze na adres IP reprezentowanego prefiksu */
    private:    CIp offsetToIp(const CRangeOffset offset) const;

                /* Przekształcenie offsetu w kontenerze na tekstowy zapis IP/maska */
    public:     std::string dumpKey(const CRangeOffset offset) const;

    public:     const bool add(const CIp addr, const CNetmask netmask);
    public:     const bool del(const CIp addr, const CNetmask netmask);
    public:     COptionalRangeOffset check(const CIp addr);
    public:     void dump();
    public:     void bench(const unsigned int count, const CAddresses addresses);
};

#endif /* DB_H_ */
