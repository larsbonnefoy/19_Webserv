#include "../Includes/Server.hpp"
#include "../Includes/Config.hpp"
#include "../Includes/ConfigParser.hpp"
#include "../Includes/Location.hpp"
#include <fstream>
#include <sstream>
#include <string>
#define LISTEN 0
#define SERVER_NAME 1
#define ERROR_PAGE 2
#define LOCATION 3
#define MAX_BODY_SIZE 4
#define SERVER_ROOT 5

#define LOC 0 
#define ROOT 1
#define ACCEPT 2
#define CGI_LOC 3
#define INDEX 4
#define AUTOINDEX 5
#define REDIRECT 6

/*
 * When seeing Server Block between brackets, isolates it and sends the whole 
 * block to addServer 
 */ 
void parseConfig(std::string configFile, Config &conf) {

    std::ifstream file(configFile);

    if (file.fail() || !endsWithConf(configFile)) {
        throw UnvalidInputFile();
    }

    std::string line;
    std::streampos nextPos = 0;
    bool createdServer = false;

    while (std::getline(file.seekg(nextPos), line)) {
        std::string infoBuffer;
        if (line.find("server") != std::string::npos) {
            nextPos = getInstructionBlock(file, infoBuffer);
            addServer(infoBuffer, conf);
            createdServer = true;
        }
        else {
            if (!isEmptyLine(line)) {
                throw UnvalidInstructionBlock();
            }
            nextPos += 1;
        }
    }
    if (createdServer == false) {
        throw NoServerCreated();
    }
}
 
/*
 * Returns True if file ends in .conf, false otherwise;
 */ 
bool endsWithConf(const std::string& filename) {
    if (filename.length() < 5) 
        return (false);
    return (filename.substr(filename.length() - 5) == ".conf");
}

/*
 * Returns str enclosed by two matching brackets 
 * /!\ First opening bracket has already been seen, have to add it manually
 *
 */
std::streampos  getInstructionBlock(std::ifstream &file, std::string &outputBuffer) {

    std::stack<char>    bracketStack;
    std::string         line;
    
    bracketStack.push('{');

    while (std::getline(file, line)) {
        if (line.find('{') != std::string::npos) {
            bracketStack.push('{');
        }
        else if (line.find('}') != std::string::npos) {
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
        outputBuffer += (line + '\n');
    }
    if (!bracketStack.empty()) {
        throw UnterminatedBlock();
    }
    return(file.tellg());
}

/*
 * Writes string into an outputBuffer
 * /!\ First opening bracket has already been seen, have to add it manually
 *
 */
void getLocationBlock(std::stringstream &ss, std::string &outputBuffer) {

    std::stack<char>    bracketStack;
    std::string         line;
    
    bracketStack.push('{');
    outputBuffer += '\n';

    while (std::getline(ss, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.find('{') != std::string::npos) {
            bracketStack.push('{');
        }
        else if (line.find('}') != std::string::npos) {
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
        outputBuffer += (line + '\n');
    }
    if (!bracketStack.empty()) {
        throw UnterminatedBlock();
    }
}

/*
 * Given location returns string of containing matching opening and closing brackets;
 * Returns position AFTER closing bracket if found or throws error if not found 
 */ 
int nextMatchingBracket(std::string input, std::string &outputBuffer, size_t startPos) {

    std::stack<char>    bracketStack;
    size_t              bracketStart = 0;

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
 * Writes the value of [directive] from [inputString] to [outputBuffer] starting
 * [startPos] in inputString.
 * Value HAS to be terminated with ';'
 * Returns ends position of value that has been found or -1 if no match has been found
 * /!\Jumps to next ; so if value is not ended by semicolumn BIG pb
 */ 
int findMatchingValue(std::string inputString, std::string directive, std::string &outputBuffer) {
    std::size_t valuePos = directive.length();
    std::size_t endPos = inputString.find(";", valuePos);
    if (endPos != std::string::npos) {
        outputBuffer = inputString.substr(valuePos, endPos - valuePos);
        if (inputString.length() != outputBuffer.length() + directive.length() + 1) {
            throw UnvalidDirective();
        }
        return (endPos + 1);
    }
    else {
        throw UnterminatedDirective();
    }
    return (-1);
}

/*
 * Checks if line contains a directive form directives 
 * If TRUE returns directiveID
 * If FALSE returns -1
 */ 
int matchDirective(std::string line, std::string *directives, size_t nbrDirectives) {
    
    for (size_t directiveID = 0; directiveID < nbrDirectives; directiveID++) {
        if (line.find(directives[directiveID]) == 0) {
            return (directiveID);
        }
    }
    return (-1);
}

/*
 * Find key words "listen", "server_name" and "error_page" and saves those information in the Server instance that is then copied into the config;
 * Should be able to make certain directives mandatory
 * Using Array of value to true or false?
 */
void    addServer(std::string infoBuffer, Config &conf) {

    Server serv;
    std::string directives[6] = {"listen", "server_name", "error_page", "location", "max_body_size", "root"};

    std::stringstream ss(infoBuffer);
    std::string line;

    while (std::getline(ss, line)) {
        std::string value = "";
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line == "" || line == "{" || line == "}") {
            continue ;
        }
        else { 
            int directiveID = matchDirective(line, directives, 6);

            switch (directiveID) {
                case LISTEN:
                    findMatchingValue(line, directives[directiveID], value);
                    addIpPort(value, serv);
                    break; 

                case SERVER_NAME:
                    findMatchingValue(line, directives[directiveID], value);
                    serv.setName(value);
                    break; 
                    
                case ERROR_PAGE:
                    findMatchingValue(line, directives[directiveID], value);
                    addErrorPages(value, serv);
                    break; 

                case LOCATION:
                    getLocationBlock(ss, line);
                    createLocation(line, serv);
                    break; 

                case MAX_BODY_SIZE:
                    findMatchingValue(line, directives[directiveID], value);
                    addMaxBodySize(value, serv);
                    break; 

                case SERVER_ROOT:
                    findMatchingValue(line, directives[directiveID], value);
                    if (value.empty() || value[0] != '/') {
                        throw RootIsNotAbs();
                    }
                    serv.setServerRoot(value);
                    break;

                default:
                    throw UnvalidDirective();
            }
        }
    }

    if (conf.getServers().count(serv.getPort()) == 1) {
        throw DuplicateValueError();
    }
    if (serv.getServerRoot() == "" || serv.getIp() == "") {
        throw MissingDirective();
    }
    if (serv.getLocations().size() == 0) {
        throw MissingDefaultLocation();
    }
    conf.getServers()[serv.getPort()] = serv; 
    std::cout << serv << std::endl;
}

/*
 * Set new maxbody size if defined. If not stays at default value;
 */ 
void addMaxBodySize(std::string value, Server &serv) {
    if (!isNumeric(value)) {
        throw UnvalidValue();
    }
    std::stringstream ss(value);     
    size_t convVal;
    ss >> convVal;
    if (ss.fail()) {
        throw  UnvalidValue();
    }
    serv.setMaxBodySize(convVal);
}

/* 
 * Adding all occurences of error_pages [nb] [Path] to the server;
 * IF nb already exists, takes the newer one
 */ 
void addErrorPages(std::string infoBuffer, Server &serv) {

    std::string errCode;
    std::string errPath;
    size_t convErrCode;

    errCode = infoBuffer.substr(0, 3);
    errPath = infoBuffer.substr(3, infoBuffer.length());
    
    if (!isNumeric(errCode))
        throw UnvalidErrCode();

    std::stringstream ss(errCode);
    ss >> convErrCode;

    serv.setError(convErrCode, errPath);
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

        size_t convPort;

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

/*
 * Finds Location Path. Location path is between "Location Tag" and brackets 
 * If no brackets are found after the location, throws an error.
 * If Location path is empty, throws an error.
 */ 
std::string getLocationPath(std::string infoBuffer, size_t startPos) {
    std::string path;
    size_t endPos = infoBuffer.find("{", startPos);
    if (endPos == std::string::npos) {
        throw UnterminatedBlock();
    }
    path = infoBuffer.substr(startPos, endPos - startPos);
    if (path.empty() || path[0] != '/') {
        throw UnvalidRoute();
    }
    return (path);
}

void createLocation(std::string inputBuffer, Server &serv) {

    Location    loc;
    std::string directives[7] = {"location", "root", "accept", "cgi", "index", "autoindex", "redirect"};
    std::stringstream ss(inputBuffer);
    std::string line;
    bool definedActionOnLoading = false;

    while (std::getline(ss, line)) {
        std::string value = "";
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line == "" || line == "{" || line == "}") {
            continue ;
        }
        else {
            int directiveID = matchDirective(line, directives, 7);

            switch(directiveID) {
                case LOC:
                    loc.setPath(getLocationPath(line, 8));
                    break;

                case ROOT:
                    findMatchingValue(line, directives[directiveID], value);
                    if (value.empty() || value[0] != '/') {
                        throw RootIsNotAbs();
                    }
                    loc.setRoot(value);
                    break;

                case ACCEPT:
                    findMatchingValue(line, directives[directiveID], value);
                    addMethods(value, loc); 
                    break;
                
                case CGI_LOC:
                    findMatchingValue(line, directives[directiveID], value);
                    setCGI(value, loc);
                    break;

                case INDEX:
                    if (definedActionOnLoading == false) {
                        findMatchingValue(line, directives[directiveID], value);
                        loc.setIndex(value);
                        definedActionOnLoading = true;
                    }
                    else  
                        throw ConflictingInstruction();
                    break;

                case AUTOINDEX:
                    if (definedActionOnLoading == false) {
                        findMatchingValue(line, directives[directiveID], value);
                        if (value == "true")
                            loc.setAutoIndex(TRUE);
                        else if (value == "false")
                            loc.setAutoIndex(FALSE);
                        else 
                            throw UnvalidValue();
                        definedActionOnLoading = true;
                    }
                    else
                        throw ConflictingInstruction();
                    break;

                case REDIRECT:
                    if (definedActionOnLoading == false) {
                        findMatchingValue(line, directives[directiveID], value);
                        setRedir(value, loc);
                        definedActionOnLoading = true;
                    }
                    else
                        throw ConflictingInstruction();
                    break;

                default:
                    throw UnvalidDirective();
            }
        }
    }
    serv.setLocation(loc);
}

/*
 * Adds cgi PATH to location 
 * /!\ having "CGI" in path, check is repo exists?
 */
void setCGI(std::string inputBuffer, Location &loc) {
    loc.setCGIPath(inputBuffer);
}

/*
 * Checks if redirection is valid (ie no autoIndex or index existing)
 * Adds [code] [path] redirect
 * If not valid throws error
 */ 
void setRedir(std::string inputBuffer, Location &loc) {

    std::string redirCode = inputBuffer.substr(0, 3);
    std::string redirPath = inputBuffer.substr(3, inputBuffer.length());
    
    size_t      convCode; 
    if (!isNumeric(redirCode))
        throw UnvalidValue();

    std::stringstream ss(redirCode);
    ss >> convCode;
    if (ss.fail()) {
        throw UnvalidValue();
    }
    loc.setRedirect(convCode, redirPath);
}

/*
 * If method is defined, set its value to 1, otherwise it will stay at init value
 * ATM: accepts multiple defintions of the same method
 */ 
void addMethods(std::string infoBuffer, Location &loc) {
    int     methodID;

    methodID = matchMethod(infoBuffer);
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

int matchMethod(std::string method) {

    size_t retVal = -1;
    std::string methods[3] = {"GET", "POST", "DELETE"};

    for (size_t methodID = 0; methodID < 3; methodID++) {
        if (method == methods[methodID]) {
            retVal = methodID;
        }
    } 
    return (retVal);
}

bool isEmptyLine(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!isspace(str[i])) {
            return (false);
        }
    }
    return (true);
}

bool isNumeric(const std::string &input) {
    for (size_t i = 0; i < input.size(); i++) {
        if (!std::isdigit(input[i]))
            return (false);
    }
    return (true);
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
    return ("[ConfigFileError] : Instruction Block not enclosed by brackets");
}

const char *UnvalidValue::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Value");
}

const char *UnvalidErrCode::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Error Code");
}

const char *UnvalidRoute::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Location Route. Is Empty or not abs");
}

const char *UnvalidServerRoute::what(void) const throw() {
    return ("[ConfigFileError] : Unvalid Server Route");
}

const char *ConflictingInstruction::what(void) const throw() {
    return ("[ConfigFileError] : Conflicting Instructions");
}

const char *DuplicateValueError::what(void) const throw() {
    return ("[ConfigFileError] : Duplicate Port");
}

const char *MissingDirective::what(void) const throw() {
    return ("[ConfigFileError] : Missing Configuration Directive");
}

const char *UnvalidInstructionBlock::what(void) const throw() {
    return ("[ConfigFileError] : Unknown Instruction Block ");
}

const char *UnvalidDirective::what(void) const throw() {
    return ("[ConfigFileError] :  Unvalid Configuration Directive");
}

const char *MissingDefaultLocation::what(void) const throw() {
    return ("[ConfigFileError] :  Missing Location Configuration");
}

const char *UnvalidInputFile::what(void) const throw() {
    return ("[ConfigFileError] :  Unvalid Input File");
}

const char *NoServerCreated::what(void) const throw() {
    return ("[ConfigFileError] :  No Server Was Created");
}

const char *RootIsNotAbs::what(void) const throw() {
    return ("[ConfigFileError] :  Root Path should be absolute");
}
