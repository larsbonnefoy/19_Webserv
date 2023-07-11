/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:27 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/04 11:56:17 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Http.hpp"
std::map<size_t, std::string>       StaticInit::STATUS_CODE_PHRASE;
std::map<std::string, std::string>  StaticInit::MIME_TYPES;
std::string                         StaticInit::DEF_FILE;
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

void	Http::setUri(std::string uri)
{
	this->_uri = uri;
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
	if (checkIt == headerToAdd.end()) {
		throw std::exception(); //TODO
    }
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

std::string	Http::getUri(void)
{
	return(this->_uri);
}

std::string Http::headerToStr(void) {
    std::string headerStr;
    
    for (std::map<std::string, std::string>::iterator it = _headerField.begin();
            it != _headerField.end(); it++) {
        headerStr += (it->first + ": " + it->second + "\r\n");
    }
    return (headerStr);
}

std::string Http::valToString(size_t num) {
    std::stringstream ss;
    ss << num;
    return (ss.str());
}
