/*
 * db.h
 *
 *  Created on: 24 lut 2020
 *      Author: emvi
 */

#ifndef DB_H_
#define DB_H_

#include <cstdint>
#include <optional>
#include <forward_list>

using CIp = uint32_t;
using CNetmask = uint8_t;
using CRangeKey = uint64_t;
using COptionalRangeKey = std::optional<CRangeKey>;
using CRanges = std::forward_list<CRangeKey>;
using CAddresses = std::forward_list<CIp>;

class CIPDB2 {
        /* Prefiksy zapisane są w postaci liczby gdzie zawarta jest cała informacja.
         * Program ma działać na architekturze 64-bitowej, więc wystarczy na 32 bity adresu IP
         * i kilka bitów maski.
         * W młodszej części klucza znajdują się 32 bity adresu więc nie potrzeba ich przesuwać.
         * W kolejnym- starszym bajcie znajduje się rozmiar maski przeznaczonej na hosty objęte
         * prefiksem.
         */
    private:    CRanges mRanges;    /* wpisane prefiksy */

    private:    constexpr CRangeKey networkAddress(const CNetmask negativeNetmask) const {
                    return ((static_cast<CRangeKey>(1) << negativeNetmask) - 1);
                }

    private:    const CRangeKey makeKey(const CIp addr, const CNetmask netmask) const  {
                    auto negativeNetmask = 32-netmask;
                    return (addr & ~networkAddress(negativeNetmask)) | (static_cast<CRangeKey>(negativeNetmask) << 32);
                }

    public:     const bool add(const CIp addr, const CNetmask netmask);
    public:     const bool del(const CIp addr, const CNetmask netmask);
    public:     std::string dumpKey(const CRangeKey key) const;
    public:     COptionalRangeKey check(const CIp addr);
    public:     void bench(const unsigned int count, const CAddresses addresses);
    public:     void dump();
};

using DB = CIPDB2;

#endif /* DB_H_ */
