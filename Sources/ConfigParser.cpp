#include "../Includes/Server.hpp"
#include "../Includes/Config.hpp"
#include "../Includes/ConfigParser.hpp"
#include "../Includes/Location.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <stack>

/* TO DO 
 * Check possible parsing errors 
 *      -> non ending ;
 *      -> non valid ip address xxx.xxx.xxx.xxx
 *
 * Change return type of nextMatchingBracket func
 * 
 * Reserve WORD (directives) to make parsing more effective? 
 *
 * Limit client body size 
 * Be able to not set server_name;
 * First host:port has to be default one 
 *
 * Location (->Route??)
 *  ->Default file to answer if request is a directory
 *  ->Exec CGI on certain file extension
 *  ->Make route able to accept uploaded files and configure where they should be saved
 *
 */ 

int32_t     nextMatchingBracket(std::string input, std::string &outputBuffer, uint16_t startPos = 0);
int32_t     findMatchingValue(std::string inputString, std::string directive,std::string &outputBuffer, uint16_t startPosition = 0);
std::string nextMatchingCharBuffer(std::ifstream &file, char openingChar, char closingChar);

void        addServer(std::string infoBuffer, Config &conf);
void        addIpPort(std::string values, Server &serv);
void        addErrorPages(std::string infoBuffer, Server &serv);
void        addLocation(std::string infoBuffer, Server &serv);

void        createLocation(std::string inputBuffer, Server &serv);
void        addMethods(std::string infoBuffer, Location &loc);
int32_t     matchMethod(std::string method);

bool        isNumeric(const std::string &input);

/*
 * When seeing a key word (server,.. tbc) jump to the next closing bracket and
 * start configuring server obj that will be added in Config object
 */ 
Config *parseConfig(std::string configFile) {
    Config *configRes = new Config();

    std::ifstream file(configFile.c_str()); 
    std::string line;

    while (std::getline(file, line, '{')) {

        if (line.find("server") != std::string::npos) {
            std::string serverInfoBuffer =  nextMatchingCharBuffer(file, '{', '}');
            addServer(serverInfoBuffer, *configRes);
        }
    }
    return (configRes);
}

/*
 * Returns str enclosed by two matching brackets 
 * /!\ First opening bracket has already been seen, have to add it manually
 *
 */
std::string nextMatchingCharBuffer(std::ifstream &file, char openingChar, char closingChar) {

    std::stack<char>    bracketStack;
    std::string         buffer;
    std::string         line;
    
    bracketStack.push(openingChar);

    while (std::getline(file, line)) {
        if (line.find(openingChar) != std::string::npos) {
            bracketStack.push(openingChar);
        }
        else if (line.find(closingChar) != std::string::npos) {
            if (!bracketStack.empty()) {
                bracketStack.pop(); 
                if (bracketStack.empty()) {
                    break;
                }
            }
            else {
                throw UnterminatedBlock();
            }
        }
        buffer += (line);
    }
    if (!bracketStack.empty()) {
        throw UnterminatedBlock();
    }
    return (buffer);
}

/*
 * Given location returns string of containing matching opening and closing brackets;
 * Returns position AFTER closing bracket if found or throws error if not found 
 */ 
int32_t nextMatchingBracket(std::string input, std::string &outputBuffer, uint16_t startPos) {

    std::stack<char>    bracketStack;
    uint32_t            bracketStart = 0;

    for (size_t i = startPos; i < input.length(); i++) {
        if (input[i] == '{') {
            if (bracketStart == 0) {
                bracketStart = i;
            }
            bracketStack.push('{');
        }
        else if (input[i] == '}') {
            if (bracketStack.empty()) {
                throw UnterminatedBlock();
            }
            bracketStack.pop();
            if (bracketStack.empty()) {
                outputBuffer = input.substr(bracketStart + 1, i - bracketStart - 1);
                return (i + 1);
            }
        }
    }
    throw UnterminatedBlock();
}

/*
 * Find key words "listen", "server_name" and "error_page" and saves those information in the Server instance that is then copied into the config;
 * Find a way to add multiple errors and locations i guess
 */
void    addServer(std::string infoBuffer, Config &conf) {

    Server serv;
    infoBuffer.erase(remove_if(infoBuffer.begin(), infoBuffer.end(), isspace), infoBuffer.end());

    std::string directives[4] = {"listen", "server_name", "error_page", "location"};
    
    for (uint32_t directiveID = 0; directiveID < 4; directiveID++) {
        std::string value;
        switch (directiveID) {
            case 0:
                findMatchingValue(infoBuffer, directives[directiveID], value);
                addIpPort(value, serv);
                break; 
            case 1:
                findMatchingValue(infoBuffer, directives[directiveID], value);
                serv.setName(value);
                break; 
            case 2:
                addErrorPages(infoBuffer, serv);
                break; 
            case 3:
                addLocation(infoBuffer, serv);
                break; 
        }
    }
    conf.getServers().push_back(serv);
}

void addLocation(std::string infoBuffer, Server &serv) {

    uint16_t    endPos = 0;
    size_t      searchFrom;
    std::string outputBuffer;
    
    while (true) {
        searchFrom = infoBuffer.find("location", endPos);
        if (searchFrom == std::string::npos) {
            break;
        }
        endPos = nextMatchingBracket(infoBuffer, outputBuffer, searchFrom);
        createLocation(outputBuffer, serv);
    }
}

void createLocation(std::string inputBuffer, Server &serv) {
    Location    loc;
    std::string directives[3] = {"root", "accept", "autoindex"};
    
    for (uint32_t directiveID = 0; directiveID < 3; directiveID++) {
        std::string value;
        switch (directiveID) {
            case 0:
                findMatchingValue(inputBuffer, directives[directiveID], value);
                loc.setPath(value);
                break; 
            case 1:
                addMethods(inputBuffer, loc);
                break; 
            case 2:
                findMatchingValue(inputBuffer, directives[directiveID], value);
                if (value == "true")
                    loc.setAutoIndex(true);
                else if (value == "false")
                    loc.setAutoIndex(false);
                else 
                    throw UnvalidValue();
                break; 
        }
    }
    serv.setLocation(loc);
}

/*
 * ATM: accepts multiple defintions of the same method
 */ 
void addMethods(std::string infoBuffer, Location &loc) {
    int32_t     searchPos = 0;
    std::string outputBuffer;
    int32_t     methodID;

    while (true) {
        searchPos = findMatchingValue(infoBuffer, "accept", outputBuffer, searchPos);
        if (searchPos == -1)
            break;
        methodID = matchMethod(outputBuffer);
        switch (methodID) {
            case GET:
                loc.setGet(true);
                break;
            case POST:
                loc.setPost(true);
                break;
            case DELETE:
                loc.setDel(true);
                break;
            default:
                throw UnvalidValue();
        }
    }    
}

int32_t matchMethod(std::string method) {

    uint32_t retVal = -1;
    std::string methods[3] = {"GET", "POST", "DELETE"};

    for (size_t methodID = 0; methodID < 3; methodID++) {
        if (method == methods[methodID]) {
            retVal = methodID;
        }
    } 
    return (retVal);
}

/* 
 * Adding all occurences of error_pages [nb] [Path] to the server;
 */ 
void addErrorPages(std::string infoBuffer, Server &serv) {

    std::string outputBuffer;
    int32_t searchPos = 0;
    std::string errCode;
    std::string errPath;
    uint32_t convErrCode;

    while (true) {
        searchPos = findMatchingValue(infoBuffer, "error_page", outputBuffer, searchPos);
        if (searchPos == -1)
            break ;

        errCode = outputBuffer.substr(0, 3);
        errPath = outputBuffer.substr(3, outputBuffer.length());
        
        if (!isNumeric(errCode))
            throw UnvalidErrCode();

        std::stringstream ss(errCode);
        ss >> convErrCode;

        serv.setError(convErrCode, errPath);
    }
}

void addIpPort(std::string values, Server &serv) {
     
    std::size_t sep = values.find(":");
    if (sep != std::string::npos) {
        std::string ip = values.substr(0, sep);
        serv.setIp(ip);
        
        std::string port = values.substr(sep + 1, values.length());
        std::stringstream ss(port);
        if (!isNumeric(port)) {
            throw UnvalidPort();
        }

        uint32_t convPort;

        ss >> convPort;
        if (ss.fail()) {
            throw ConfigFileError();
        }
        else {
            serv.setPort(convPort);
        }
    }
    else {
        throw UnvalidListenDirective();
    }
}

bool isNumeric(const std::string &input) {
    for (size_t i = 0; i < input.size(); i++) {
        if (!std::isdigit(input[i]))
            return (false);
    }
    return (true);
}

/*
 * Writes the value of [directive] from [inputString] to [outputBuffer] starting
 * [startPos] in inputString.
 * Value HAS to be terminated with ';'
 * Returns ends position of value that has been found or -1 if no match has been found
 * /!\Jumps to next ; so if value is not ended by semicolumn BIG pb
 */ 
int32_t findMatchingValue(std::string inputString, std::string directive, std::string &outputBuffer, uint16_t startPos) {
    std::size_t directivePos =  inputString.find(directive, startPos);

    if (directivePos != std::string::npos) {
        std::size_t valuePos = directivePos + directive.length();
        std::size_t endPos = inputString.find(";", valuePos);
        if (endPos != std::string::npos) {
            outputBuffer = inputString.substr(valuePos, endPos - valuePos);
            return (endPos);
        }
        else {
            throw UnterminatedDirective();
        }
    }
    return (-1);
}

/*--------------------------Exceptions---------------------------------------*/

const char *ConfigFileError::what(void) const throw() {
    return ("[ConfigFileError] : Error while parsing Config File");
}

const char *UnvalidListenDirective::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Listen Directive");
}

const char *UnvalidPort::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Port");
}

const char *UnterminatedDirective::what(void) const throw() {
    return ("[ConfigFileError] : Directive not terminated by semicolumn");
}

const char *UnterminatedBlock::what(void) const throw() {
    return ("[ConfigFileError] : Instruction Block not enclosed by brackets}");
}

const char *UnvalidValue::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Value");
}

const char *UnvalidErrCode::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Error Code");
}
