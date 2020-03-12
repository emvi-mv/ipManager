/*
 * cli.cpp
 *
 *  Created on: 27 lut 2020
 *      Author: emvi
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <arpa/inet.h>

#include "cli.h"
#include "string_utils.h"

extern DB database;

void CCli::run() {
    std::string line;
    while (std::getline(std::cin, line)) {
        /* wczytanie linijki z wiersza poleceń i podział na komendę i argumenty */
        std::istringstream commandLine(line);
        std::string command;
        Targuments arguments;
        commandLine >> command;
        while (commandLine.rdbuf()->in_avail()) {
            std::string argument;
            commandLine >> argument;
            arguments.push_back(argument);
        }
        runCommand(command, arguments);
    }
}

void CCli::runCommand(const std::string &command, const Targuments &arguments) {
    switch (fnv1a(command.c_str())) {
        case "add"_h:
            commandAdd(arguments);
            break;
        case "del"_h:
            commandDel(arguments);
            break;
        case "check"_h:
            commandCheck(arguments);
            break;
        case "dump"_h:
            commandDump(arguments);
            break;
        case "bench"_h:
            commandBench(arguments);
            break;
        default:
            std::cout << "Error: unknown command" << std::endl;
            break;
    }
}

void CCli::commandAdd(const Targuments &arguments) {
    if (! checkNumberOfArguments(arguments, 1))
        return;
    CIp ip;
    CNetmask netmask;
    if (! getArgumentPrefix(arguments.front(), ip, netmask))
        return;
    if (database.add(ip, netmask))
        std::cout << "OK" << std::endl;
    else
        std::cout << "Error: already added" << std::endl;
}

void CCli::commandDel(const Targuments &arguments) {
    if (! checkNumberOfArguments(arguments, 1))
        return;
    CIp ip;
    CNetmask netmask;
    if (! getArgumentPrefix(arguments.front(), ip, netmask))
        return;
    if (database.del(ip, netmask))
        std::cout << "OK" << std::endl;
    else
        std::cout << "Error: not exists" << std::endl;
}

void CCli::commandCheck(const Targuments &arguments) {
    if (! checkNumberOfArguments(arguments, 1))
        return;
    CIp ip;
    if (! getArgumentIp(arguments.front(), ip))
        return;
    auto record = database.check(ip);
    if (record.has_value())
        std::cout << database.dumpKey(*record) << std::endl;
    else
        std::cout << "Not found in database" << std::endl;
}

void CCli::commandDump(const Targuments &arguments) {
    database.dump();
}

void CCli::commandBench(const Targuments &arguments) {
    if (! checkNumberOfArguments(arguments, 2))
        return;
    int count;
    CAddresses addresses;
    if (! getArgumentNumber(arguments.front(), count)) {
        std::cout << "Error: bad number of iterations" << std::endl;
        return;
    }
    bool ipError = false;
    std::for_each(++(arguments.begin()), arguments.end(), [&] (auto argument) {
        CIp ip;
        if (! getArgumentIp(argument, ip)) {
            ipError = true;
            return;
        }
        addresses.emplace_back(std::move(ip));
    });
    if (ipError)
        return;
    auto start_time = std::chrono::high_resolution_clock::now();
    database.bench(count, addresses);
    auto working_time = std::chrono::high_resolution_clock::now() - start_time;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(working_time).count() << "ms" << std::endl;
}

bool CCli::checkNumberOfArguments(const Targuments &arguments, const unsigned int minimum) {
    if (arguments.size() < minimum) {
        std::cout << "Error: need at least " << minimum << " argument(s)" << std::endl;
        return false;
    }
    return true;
}

bool CCli::getArgumentIp(const std::string &argument, CIp &ip) {
    struct in_addr addr;
    if (inet_aton(argument.c_str(), &addr) == 0) {
        std::cout << "Error: bad IP" << std::endl;
        return false;
    }
    ip = htonl(addr.s_addr);
    return true;
}

bool CCli::getArgumentNumber(const std::string &argument, int &number) {
    if (argument.size() && argument.find_first_not_of("0123456789") == std::string::npos) {
        number = std::atoi(argument.c_str());
        return true;
    }
    return false;
}

bool CCli::getArgumentPrefix(const std::string &argument, CIp &ip, CNetmask &netmask) {
    std::istringstream argumentPrefix(argument);
    std::string addressString, netmaskString;
    int netmaskI;

    std::getline(argumentPrefix, addressString, '/');
    std::getline(argumentPrefix, netmaskString);
    if (! getArgumentIp(addressString, ip))
        return false;
    if (! getArgumentNumber(netmaskString, netmaskI)) {
        std::cout << "Error: bad netmask" << std::endl;
        return false;
    }
    netmask = netmaskI & 0xFF;
    return true;
}
