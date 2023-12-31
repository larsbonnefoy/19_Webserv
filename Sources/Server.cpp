#include "../Includes/Server.hpp"
#include "../Includes/ConfigParser.hpp"
#include <cstddef>
#include <exception>
#include <vector>
#include <sys/stat.h>

Server::Server(void) 
    :_root(""), _ip(""), _port(0), _maxBodySize(10000), _serverName("") {
}

Server::Server(const Server &other) 
    : _root(other._root), _ip(other._ip), _port(other._port), _maxBodySize(other.getMaxBodySize()), 
    _serverName(other._serverName), _errors(other._errors), _locations(other._locations) {
}

Server::~Server(void) {
}

Server &Server::operator=(const Server &other) {
    this->_root = other.getServerRoot();
    this->_serverName = other.getName();
    this->_ip = other.getIp();
    this->_port = other.getPort();
    this->_maxBodySize = other.getMaxBodySize();
    this->_errors = other._errors;
    this->_locations = other._locations;
    return *this;
}

void Server::setServerRoot(std::string rootPath) {
    struct stat dir_stat;
    if (stat(rootPath.c_str(), &dir_stat) == 0) {
        if (S_ISDIR(dir_stat.st_mode)) {
            this->_root = rootPath;
        }
        else {
            throw UnvalidServerRoute();
        } 
    }
    else { 
        throw UnvalidServerRoute();
    }
}

void Server::setIp(std::string ip) {
    this->_ip = ip;
}

void Server::setPort(size_t port) {
    this->_port = port;
}

void Server::setMaxBodySize(size_t size) {
    this->_maxBodySize = size;
}

void Server::setName(std::string name) {
    this->_serverName = name;
}

void Server::setError(size_t errCode, std::string errPath) {
    if (_errors.count(errCode) == 1) {
        throw DuplicateValueError();
    }
    this->_errors[errCode] = errPath;
}

std::string Server::getServerRoot(void) const {
    return (this->_root);
}

std::string Server::getIp(void) const {
    return (this->_ip);
}

std::size_t Server::getPort(void) const {
    return (this->_port);
}

std::size_t Server::getMaxBodySize(void) const {
    return (this->_maxBodySize);
}

void Server::setLocation(Location &loc) {
    this->_locations.push_back(loc);
}

std::string Server::getName(void) const {
    return (this->_serverName);
}

std::map<size_t, std::string> Server::getErrors(void) const {
    return (this->_errors);
}

std::vector<Location> Server::getLocations(void) const {
    return (this->_locations);
}

std::ostream &operator<<(std::ostream &out, const Server &serv) {
    
    out << "Server RootPath : " << serv.getServerRoot() << std::endl;
    out << "Server Name : " << serv.getName() << std::endl;
    out << "IP : " << serv.getIp() << std::endl;
    out << "Port : " << serv.getPort() << std::endl;
    out << "Max Body Size : " << serv.getMaxBodySize() << std::endl;
    
    std::map<size_t, std::string> errors = serv.getErrors();
    for (std::map<size_t, std::string>::iterator it = errors.begin(); 
            it != errors.end(); it++) {
        out << "Error : [" << it->first << "]" << " : " << it->second << std::endl; 
    }
     
    std::vector<Location> loc = serv.getLocations();
    for (size_t i = 0; i < loc.size(); i++) {
        out << loc[i] << std::endl;
    }
    return (out);
}
