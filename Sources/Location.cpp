#include "../Includes/Location.hpp"
#include <iostream>
#include <sys/types.h>

Location::Location(void) :_path(""), _root(""), _index(""), _autoindex(UNDEFINED) {
    this->setAutorizedMethods(false, false, false);
    this->_redirect.first = 0;
    this->_redirect.second = "";
}

Location::Location(const Location &other) 
    : _path(other._path), _root(other._root), _index(other._index), 
    _autoindex(other._autoindex), _redirect(other._redirect) {
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
}

Location::~Location(void) {
}

Location &Location::operator=(const Location &other) {
    this->_path = other._path;
    this->_root = other._root;
    this->_index = other._index;
    this->_autoindex = other._autoindex;
    this->_redirect = other._redirect;
    this->setAutorizedMethods(other.getGetVal(), other.getPostVal(), other.getDelVal());
    return *this;
}

std::string Location::getPath(void) const {
    return (this->_path);
};

std::string Location::getRoot(void) const {
    return (this->_root);
};

std::string Location::getIndex(void) const {
    return (this->_index);
};

size_t Location::getAutoIndex(void) const {
    return (this->_autoindex);
}

std::pair<size_t, std::string> Location::getRedirect(void) const {
    return (this->_redirect);
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

void Location::setRoot(std::string root) {
    this->_root = root;
}

void Location::setIndex(std::string index) {
    this->_index = index;
}

void Location::setAutoIndex(size_t val) {
    this->_autoindex = val;
}

void Location::setRedirect(size_t code, std::string path) {
    this->_redirect.first = code;
    this->_redirect.second = path;
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
    out << "    Root : " << loc.getRoot() << std::endl;
    out << "    Index : {" << loc.getIndex() << "}";
    out << "    Autoindex : {" << loc.getAutoIndex() << "}";
    out << "    Redir : {[" << loc.getRedirect().first << "] " << loc.getRedirect().second << "}" << std::endl;  
    out << "    GET : {" << loc.getGetVal() << "}";
    out << "    POST : {" << loc.getPostVal() << "}";
    out << "    DEL : {" << loc.getDelVal() << "}" << std::endl;
    
    return (out);
}
