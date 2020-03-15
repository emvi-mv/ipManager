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

const bool CIPDB2::add(const CIp addr, const CNetmask netmask) {
    auto key = makeKey(addr, netmask);
    if (std::find(mRanges.begin(), mRanges.end(), key) != mRanges.end())
        return false;
    mRanges.emplace_front(key);
    mRanges.sort();
    return true;
}

const bool CIPDB2::del(const CIp addr, const CNetmask netmask) {
    auto key = makeKey(addr, netmask);
    if (std::find(mRanges.begin(), mRanges.end(), key) == mRanges.end())
        return false;
    mRanges.remove(key);
    return true;
}

std::string CIPDB2::dumpKey(const CRangeKey key) const {
    std::stringstream s;
    s << inet_ntoa(in_addr{htonl(static_cast<in_addr_t>(key & 0xffffffff))}) << "/" << (32 - (key >> 32));
    return s.str();
}

COptionalRangeKey CIPDB2::check(const CIp addr) {
    COptionalRangeKey found;
    auto foundIt = std::find_if(mRanges.begin(), mRanges.end(), [&] (const CRangeKey &range) {
        return (addr - (range & 0xFFFFFFFF)) < (static_cast<uint32_t>(1) << (range >> 32));
    });
    if (foundIt != mRanges.end())
        return {*foundIt};
    return {};
}

void CIPDB2::bench(const unsigned int count, const CAddresses addresses) {
    for (auto i = count; i--;)
        std::for_each(addresses.begin(), addresses.end(), [this] (auto addr) { check(addr); });
}

void CIPDB2::dump() {
    std::for_each(mRanges.begin(), mRanges.end(), [this](auto range) {
        std::cout << dumpKey(range) << std::endl;
    });
}

