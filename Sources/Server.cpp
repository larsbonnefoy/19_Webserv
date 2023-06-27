#include "../Includes/Server.hpp"
#include <cstddef>
#include <exception>
#include <vector>

Server::Server(void) 
    : _ip("127.0.0.1"), _port(80), _maxBodySize(1048576),_serverName("localhost") {
}

Server::Server(const Server &other) 
    : _ip(other._ip), _port(other._port), _maxBodySize(other.getMaxBodySize()), 
    _serverName(other._serverName), _errors(other._errors), _locations(other._locations) {
}

Server::~Server(void) {
}

Server &Server::operator=(const Server &other) {
    this->_serverName = other.getName();
    this->_ip = other.getIp();
    this->_port = other.getPort();
    this->_maxBodySize = other.getMaxBodySize();
    this->_errors = other._errors;
    this->_locations = other._locations;
    return *this;
}

void Server::setIp(std::string ip) {
    this->_ip = ip;
}

void Server::setPort(uint32_t port) {
    this->_port = port;
}

void Server::setMaxBodySize(uint32_t size) {
    this->_maxBodySize = size;
}

void Server::setName(std::string name) {
    this->_serverName = name;
}

void Server::setError(uint32_t errCode, std::string errPath) {
    if (_errors.count(errCode) == 1) {
        throw std::exception();
    }
    this->_errors[errCode] = errPath;
}

std::string Server::getIp(void) const {
    return (this->_ip);
}

std::uint32_t Server::getPort(void) const {
    return (this->_port);
}

std::uint32_t Server::getMaxBodySize(void) const {
    return (this->_maxBodySize);
}

void Server::setLocation(Location &loc) {
    this->_locations.push_back(loc);
}

std::string Server::getName(void) const {
    return (this->_serverName);
}
std::map<uint32_t, std::string> Server::getErrors(void) const {
    return (this->_errors);
}

std::vector<Location> Server::getLocations(void) const {
    return (this->_locations);
}

std::ostream &operator<<(std::ostream &out, const Server &serv) {

    out << "Server Name : " << serv.getName() << std::endl;
    out << "IP : " << serv.getIp() << std::endl;
    out << "Port : " << serv.getPort() << std::endl;
    out << "Max Body Size : " << serv.getMaxBodySize() << std::endl;
    
    std::map<uint32_t, std::string> errors = serv.getErrors();
    for (std::map<uint32_t, std::string>::iterator it = errors.begin(); 
            it != errors.end(); it++) {
        out << "Error : [" << it->first << "]" << " : " << it->second << std::endl; 
    }
     
    std::vector<Location> loc = serv.getLocations();
    for (size_t i = 0; i < loc.size(); i++) {
        out << loc[i] << std::endl;
    }
    return (out);
}
