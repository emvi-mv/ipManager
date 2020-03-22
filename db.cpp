/*
 * db.cpp
 *
 *  Created on: 24 lut 2020
 *      Author: emvi
 */

#include <algorithm>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "db.h"

CNetmask CIPDB::offsetToNetmask(const CRangeOffset offset) const {
    CRangeOffset tmp = offset+2;
    CNetmask mask = 0;
    while (tmp>>=1)
        ++mask;
    return mask;
}

CIp CIPDB::offsetToIp(const CRangeOffset offset) const {
    auto netmask = offsetToNetmask(offset);
    auto groupOffset = prefixGroupOffset(netmask);
    return (offset-groupOffset)<<(32-netmask);
}

const bool CIPDB::add(const CIp addr, const CNetmask netmask) {
    auto groupOffset = prefixGroupOffset(netmask);
    auto ipOffset = addr >> (32-netmask);
    auto offset = groupOffset + ipOffset;
    auto alreadySet = mRanges.test(offset);
    if (! alreadySet)
        mRanges.set(offset);
    return ! alreadySet;
}

const bool CIPDB::del(const CIp addr, const CNetmask netmask) {
    auto groupOffset = prefixGroupOffset(netmask);
    auto ipOffset = addr >> (32-netmask);
    auto offset = groupOffset + ipOffset;
    auto wasSet = mRanges.test(offset);
    if (wasSet)
        mRanges.reset(offset);
    return wasSet;
}

COptionalRangeOffset CIPDB::check(const CIp addr) {
    for (int netmask = 32; netmask > 0; --netmask) {
        auto offset = prefixGroupOffset(netmask) + (addr >> (32-netmask));
        if (mRanges.test(offset))
            return offset;
    }
    return {};
}

void CIPDB::dump() {
    for (CRangeOffset offset = 0; offset < mRanges.size(); ++offset)
        if (mRanges.test(offset))
            std::cout << dumpKey(offset) << std::endl;
}

void CIPDB::bench(const unsigned int count, const CAddresses addresses) {
    for (auto i = count; i--;)
        std::for_each(addresses.begin(), addresses.end(), [this] (auto addr) { check(addr); });
}

std::string CIPDB::dumpKey(const CRangeOffset offset) const {
    std::string s(inet_ntoa(in_addr{htonl(static_cast<in_addr_t>(offsetToIp(offset)))}));
    s += "/";
    s += std::to_string(offsetToNetmask(offset));
    return std::move(s);
}

