#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <exception>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <exception>
#include <ios>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <stack>
#include <netinet/in.h>
#include "../Includes/Config.hpp"

void parseConfig(std::string configFile, Config &conf);

int             nextMatchingBracket(std::string input, std::string &outputBuffer, size_t startPos = 0);
int             findMatchingValue(std::string inputString, std::string directive,std::string &outputBuffer, size_t startPosition = 0);
std::streampos  getInstructionBlock(std::ifstream &file, std::string &outputBuffer);

void        addServer(std::string infoBuffer, Config &conf);
void        addMaxBodySize(std::string value, Server &serv);
void        addIpPort(std::string values, Server &serv);
void        addErrorPages(std::string infoBuffer, Server &serv);
void        addLocation(std::string infoBuffer, Server &serv);

std::string getLocationPath(std::string infoBuffer, size_t startPos);
void        createLocation(std::string inputBuffer, Server &serv);
void        addMethods(std::string infoBuffer, Location &loc);
void        setRedir(std::string inputBuffer, Location &loc);
int         matchMethod(std::string method);
void        setCGI(std::string inputBuffer, Location &loc);

bool        isNumeric(const std::string &input);
bool        isEmptyLine(std::string str);


class ConfigFileError : public std::exception {
    public:
        const char *what(void) const throw();
};

class UnvalidValue : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnvalidListenDirective : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnvalidPort : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnterminatedDirective : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnterminatedBlock : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnvalidErrCode : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnvalidRoute : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnvalidServerRoute : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class ConflictingInstruction : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class DuplicateValueError : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class MissingDirective : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnvalidInstructionBlock : public std::exception { 
    public:
        const char *what(void) const throw();
}; 

class UnvalidDirective : public std::exception { 
    public:
        const char *what(void) const throw();
}; 
#endif
