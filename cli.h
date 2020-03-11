/*
 * cli.h
 *
 *  Created on: 27 lut 2020
 *      Author: emvi
 */

#ifndef CLI_H_
#define CLI_H_

#include <list>

#include "db.h"

class CCli {
    using Targuments = std::list<std::string>;

    public:     void run();

    private:    void runCommand(const std::string &command, const Targuments &arguments);
    private:    void commandAdd(const Targuments &arguments);
    private:    void commandDel(const Targuments &arguments);
    private:    void commandCheck(const Targuments &arguments);
    private:    void commandDump(const Targuments &arguments);
    private:    void commandBench(const Targuments &arguments);

    private:    bool checkNumberOfArguments(const Targuments &arguments, const unsigned int minimum);
    private:    bool getArgumentIp(const std::string &argument, CIp &ip);
    private:    bool getArgumentNumber(const std::string &argument, int &number);
    private:    bool getArgumentPrefix(const std::string &argument, CIp &ip, CNetmask &netmask);
};

#endif /* CLI_H_ */
