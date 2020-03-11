/*
 * db.h
 *
 *  Created on: 24 lut 2020
 *      Author: emvi
 */

#ifndef DB_H_
#define DB_H_

#include <cstdint>
#include <set>
#include <optional>
#include <vector>

using CIp = uint32_t;
using CNetmask = uint8_t;
using CRangeKey = uint64_t;
using COptionalRangeKey = std::optional<CRangeKey>;
using CRanges = std::set<CRangeKey>;
using CAddresses = std::vector<CIp>;

class CIPDB {
        /* Prefiksy zapisane są w kontenerze typu set gdzie cała informacja zawarta jest w kluczu.
         * Program ma działać na architekturze 64-bitowej, więc wystarczy na 32 bity adresu IP
         * i kilka bitów maski.
         * W młodszej części klucza znajdują się 32 bity adresu więc nie potrzeba ich przesuwać.
         * W kolejnym- starszym bajcie znajduje się rozmiar maski.
         */

    private:    CRanges mRanges;    /* wpisane prefiksy */

    private:    constexpr CRangeKey networkAddress(const CNetmask netmask) const { return ((static_cast<CRangeKey>(1) << (32-netmask)) - 1); }
    private:    const CRangeKey makeKey(const CIp addr, const CNetmask netmask) const { return addr | networkAddress(netmask) | (static_cast<CRangeKey>(netmask) << 32); }
    public:     std::string dumpKey(const CRangeKey key) const;
    public:     const bool add(const CIp addr, const CNetmask netmask);
    public:     const bool del(const CIp addr, const CNetmask netmask);
    public:     COptionalRangeKey check(const CIp addr);
    public:     void dump();
    public:     void bench(const unsigned int count, const CAddresses addresses);
};


#endif /* DB_H_ */
