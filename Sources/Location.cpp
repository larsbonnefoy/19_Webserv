#include "../Includes/Location.hpp"
#include <iostream>

Location::Location(void) : _Path("/"), _autoindex(false) {
    this->setAutorizedMethods(false, false, false);
}

Location::Location(const Location &other) 
    : _Path(other._Path), _autoindex(other._autoindex) {
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
}

Location::~Location(void) {
}

Location &Location::operator=(const Location &other) {
    this->_Path = other._Path;
    this->_autoindex = other._autoindex;
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
    return *this;
}

std::string Location::getPath(void) const {
    return (this->_Path);
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

void Location::setAutorizedMethods(bool get, bool post, bool del) {
    this->_autorizedMethods[GET] = get;
    this->_autorizedMethods[POST] = post;
    this->_autorizedMethods[DELETE] = del;
}
std::ostream &operator<<(std::ostream &out, const Location &loc) {
    
    out << "Path : " << loc.getPath() << std::endl;
    out << "Autoindex : " << loc.getAutoIndex() << std::endl;
    out << "GET : " << loc.getGetVal() << std::endl;
    out << "POST : " << loc.getPostVal() << std::endl;
    out << "DEL : " << loc.getDelVal() << std::endl;
    
    return (out);
}
