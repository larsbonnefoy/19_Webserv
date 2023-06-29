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

// Constructors
Http::Http()
{
}

Http::Http(const Http &copy)
{
	(void) copy;
}


// Destructor
Http::~Http()
{
}

// Operators
Http & Http::operator=(const Http &assign)
{
	(void) assign;
	return *this;
}

// Setters
void	Http::setStartLine(std::string version)
{
	this->_startLine   = version;
}

void	Http::setBody(std::string body)
{
	this->_body = body;
}

void	Http::setHeaderField(std::map<std::string, std::string> headerField)
{
	this->_headerField = headerField;
}

void	Http::addToHeaderField(std::string header, std::string value) {
    this->_headerField[header] = value;
}

void	Http::addToHeaderField(std::string headerToAdd)
{
	std::pair<std::string, std::string> pair;
	std::stringstream					headerStream(headerToAdd);
	
	
	std::string::iterator checkIt = std::find(headerToAdd.begin(), headerToAdd.end(), ":");
	if (checkIt == headerToAdd.end())
		throw std::exception(); //TODO

	std::getline(headerStream, pair.first, ':');
	std::getline(headerStream, pair.second);

	this->_headerField.insert(pair);
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
