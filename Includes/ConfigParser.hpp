#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <exception>
#include <iostream>
#include <fstream>
#include "../Includes/Config.hpp"

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

Config *parseConfig(std::string configFile);

#endif
