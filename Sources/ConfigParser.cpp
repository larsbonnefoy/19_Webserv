#include "../Includes/Server.hpp"
#include "../Includes/Config.hpp"
#include "../Includes/ConfigParser.hpp"
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

int32_t     findMatchingValue(std::string inputString, std::string directive,std::string &outputBuffer, uint32_t startPosition = 0);
void        addServer(std::string infoBuffer, Config &conf);
std::string nextMatchingCharBuffer(std::ifstream &file, char openingChar, char closingChar);
void        addIpPort(std::string values, Server &serv);
void        addErrorPages(std::string infoBuffer, Server &serv);

/*
 * When seeing a key word (server,.. tbc) jump to the next closing bracket and
 * start configuring server obj that will be added in Config object
 */ 
Config *parseConfig(std::string configFile) {
    //creates a new empty object in first
    Config *configRes = new Config();

    std::ifstream file(configFile); 
    std::string line;

    while (std::getline(file, line, '{')) {
        //line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

        //jump to next matching bracket and use this text to config server
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
                std::cout << "Unvalid Closing Char" << std::endl;
            }
        }
        buffer += (line + '\n');
    }
    return (buffer);
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
                break; 
        }
    }
    conf.getServers().push_back(serv);
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

        std::stringstream ss(errCode);
        ss >> convErrCode;
        if (ss.fail()) {
            std::cout << "Unvalid Err Code" << std::endl;
        }
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
        uint32_t convPort;
        ss >> convPort;
        if (ss.fail()) {
            std::cout << "Unvalid Port" << std::endl;
        }
        else {
            serv.setPort(convPort);
        }
    }
    else {
        std::cout << "Unvalid Listen directive" << std::endl;
    }
}

/*
 * Writes the value of [directive] from [inputString] to [outputBuffer] starting
 * [startPos] in inputString.
 * Value HAS to be terminated with ';'
 * Returns ends position of value that has been found or -1 if no match has been found
 * /!\Jumps to next ; so if value is not ended by semicolumn BIG pb
 */ 
int32_t findMatchingValue(std::string inputString, std::string directive, std::string &outputBuffer, uint32_t startPos) {
    std::size_t directivePos =  inputString.find(directive, startPos);

    if (directivePos != std::string::npos) {
        std::size_t valuePos = directivePos + directive.length();
        std::size_t endPos = inputString.find(";", valuePos);
        if (endPos != std::string::npos) {
            outputBuffer = inputString.substr(valuePos, endPos - valuePos);
            return (endPos);
        }
        else {
            std::cout << "B: Directive not ended by semicolumn" << std::endl;
        }
    }
    return (-1);
}
