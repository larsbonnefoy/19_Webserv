/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:27 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/29 11:55:05 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Http.hpp"
#include <utility>
/*
StaticInit::StaticInit(void) {

    STATUS_CODE_PHRASE[100] = "Continue";
    STATUS_CODE_PHRASE[101] = "Switching Protocols";
    STATUS_CODE_PHRASE[200] = "OK";
    STATUS_CODE_PHRASE[201] = "Created";
    STATUS_CODE_PHRASE[202] = "Accepted";
    STATUS_CODE_PHRASE[203] = "Non-Authoritative Information";
    STATUS_CODE_PHRASE[204] = "No Content";
    STATUS_CODE_PHRASE[205] = "Reset Content";
    STATUS_CODE_PHRASE[206] = "Partial Content";
    STATUS_CODE_PHRASE[300] = "Multiple Choices";
    STATUS_CODE_PHRASE[301] = "Moved Permanently"; 
    STATUS_CODE_PHRASE[302] = "Found";
    STATUS_CODE_PHRASE[303] = "See Other";
    STATUS_CODE_PHRASE[304] = "Not Modified";
    STATUS_CODE_PHRASE[305] = "Use Proxy";
    STATUS_CODE_PHRASE[307] = "Temporary Redirect";
    STATUS_CODE_PHRASE[400] = "Bad Request";
    STATUS_CODE_PHRASE[401] = "Unauthorized";
    STATUS_CODE_PHRASE[402] = "Payment Required";
    STATUS_CODE_PHRASE[403] = "Forbidden";
    STATUS_CODE_PHRASE[404] = "Not Found";
    STATUS_CODE_PHRASE[405] = "Method Not Allowed";
    STATUS_CODE_PHRASE[406] = "Not Acceptable";
    STATUS_CODE_PHRASE[407] = "Proxy Authentication Required";
    STATUS_CODE_PHRASE[408] = "Request Timeout";
    STATUS_CODE_PHRASE[409] = "Conflict";
    STATUS_CODE_PHRASE[410] = "Gone";
    STATUS_CODE_PHRASE[411] = "Length Required";
    STATUS_CODE_PHRASE[412] = "Precondition Failed";
    STATUS_CODE_PHRASE[413] = "Request Entity Too Large";
    STATUS_CODE_PHRASE[414] = "Request-URI Too Long";
    STATUS_CODE_PHRASE[415] = "Unsupported Media Type";
    STATUS_CODE_PHRASE[416] = "Requested Range Not Satisfiable";
    STATUS_CODE_PHRASE[417] = "Expectation Failed";
    STATUS_CODE_PHRASE[500] = "Internal Server Error";
    STATUS_CODE_PHRASE[501] = "Not Implemented";
    STATUS_CODE_PHRASE[502] = "Bad Gateway";
    STATUS_CODE_PHRASE[503] = "Service Unavailable";
    STATUS_CODE_PHRASE[504] = "Gateway Timeout";
    STATUS_CODE_PHRASE[505] = "HTTP Version Not Supported";

    MIME_TYPES[".html"] = "text/html";
    MIME_TYPES[".txt"] = "text/plain";
}
*/

// Constructors
Http::Http(void)
{
}

Http::Http(const Http &copy) 
    : _startLine(copy._startLine), _body(copy._body), _headerField(copy._headerField) {
}


// Destructor
Http::~Http()
{
}

// Operators
Http & Http::operator=(const Http &assign)
{
    this->_startLine = assign._startLine;
    this->_body = assign._body;
    this->_headerField = assign._headerField;
	return *this;
}

// Setters
void	Http::setStartLine(std::string version)
{
	this->_startLine = version;
}

void	Http::setBody(std::string body)
{
	this->_body = body;
}

void	Http::setHeaderField(std::map<std::string, std::string> headerField)
{
	this->_headerField = headerField;
}

void	Http::addToHeaderField(std::string headerToAdd)
{
	std::pair<std::string, std::string> pair;
	std::stringstream					headerStream(headerToAdd);
	
	
	std::string::iterator checkIt = std::find(headerToAdd.begin(), headerToAdd.end(), ':');
	if (checkIt == headerToAdd.end())
		throw std::exception(); //TODO

	std::getline(headerStream, pair.first, ':');
	std::getline(headerStream, pair.second);

	this->_headerField.insert(pair);
}

void	Http::addToHeaderField(std::string header, std::string value) {
    this->_headerField[header] = value;
}

// Getters
std::string							Http::getStartLine(void) const
{
	return (this->_startLine);
}

std::string							Http::getBody(void) const
{
	return (this->_body);
}

std::map<std::string, std::string>	Http::getHeaderField(void) const
{
	return (this->_headerField);
}

std::string Http::headerToStr(void) {
    std::string headerStr;
    
    for (std::map<std::string, std::string>::iterator it = _headerField.begin();
            it != _headerField.end(); it++) {
        headerStr += (it->first + ": " + it->second + "\r\n");
    }
    return (headerStr);
}
