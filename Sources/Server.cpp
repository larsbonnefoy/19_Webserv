#include "../Includes/Server.hpp"
#include <cstdint>
#include <string>
#include <iostream>
#include <map>

Server::Server(void) 
    : _ip("127.0.0.1"), _port(80), _serverName("localhost") {
}

Server::Server(std::string ip, uint32_t port, std::string serverName, std::map<int, std::string> &errors)
    : _ip(ip), _port(port), _serverName(serverName), _errors(errors) {
}

Server::Server(const Server &other) 
    : _ip(other._ip), _port(other._port), _serverName(other._serverName), _errors(other._errors) {
}

Server::~Server(void) {
}

Server &Server::operator=(const Server &other) {
    this->_serverName = other.getName();
    this->_ip = other.getIp();
    this->_port = other.getPort();
    this->_errors = other._errors;
    return *this;
}

void Server::setIp(std::string ip) {
    this->_ip = ip;
}

void Server::setPort(uint32_t port) {
    this->_port = port;
}
void Server::setName(std::string name) {
    this->_serverName = name;
}

void Server::setError(int errCode, std::string errPath) {
    this->_errors[errCode] = errPath;
}

std::string Server::getName(void) const {
    return (this->_serverName);
}

std::string Server::getIp(void) const {
    return (this->_ip);
}

std::uint32_t Server::getPort(void) const {
    return (this->_port);
}

std::ostream &operator<<(std::ostream &out, const Server &serv) {
    out << "Server Name : " << serv.getName() << std::endl;
    out << "IP : " << serv.getIp() << std::endl;
    out << "Port : " << serv.getPort() << std::endl;
    return (out);
}
