#include "../Includes/Location.hpp"
#include <iostream>

Location::Location(void) : _path("/"), _autoindex(false) {
    this->setAutorizedMethods(false, false, false);
}

Location::Location(const Location &other) 
    : _path(other._path), _autoindex(other._autoindex) {
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
}

Location::~Location(void) {
}

Location &Location::operator=(const Location &other) {
    this->_path = other._path;
    this->_autoindex = other._autoindex;
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
    return *this;
}

std::string Location::getPath(void) const {
    return (this->_path);
};

bool Location::getAutoIndex(void) const {
    return (this->_autoindex);
}

bool Location::getGetVal(void) const {
    return (this->_autorizedMethods[GET]);
}

bool Location::getPostVal(void) const {
    return (this->_autorizedMethods[POST]);
}

bool Location::getDelVal(void) const {
    return (this->_autorizedMethods[DELETE]);
}

void Location::setPath(std::string path) {
    this->_path = path;
}

void Location::setAutoIndex(bool val) {
    this->_autoindex = val;
}

void Location::setGet(bool val) {
    this->_autorizedMethods[GET] = val;
}

void Location::setPost(bool val) {
    this->_autorizedMethods[POST] = val;
}

void Location::setDel(bool val) {
    this->_autorizedMethods[DELETE] = val;
}

void Location::setAutorizedMethods(bool get, bool post, bool del) {
    this->_autorizedMethods[GET] = get;
    this->_autorizedMethods[POST] = post;
    this->_autorizedMethods[DELETE] = del;
}
std::ostream &operator<<(std::ostream &out, const Location &loc) {

    out << "    ->Location" << std::endl;
    out << "    Path : " << loc.getPath() << std::endl;
    out << "    Autoindex : " << loc.getAutoIndex() << std::endl;
    out << "    GET : " << loc.getGetVal() << std::endl;
    out << "    POST : " << loc.getPostVal() << std::endl;
    out << "    DEL : " << loc.getDelVal() << std::endl;
    
    return (out);
}
