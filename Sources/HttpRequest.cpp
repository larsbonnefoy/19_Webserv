/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:04 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/01 13:37:45 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/HttpRequest.hpp"

// Constructors
HttpRequest::HttpRequest()
{
}

HttpRequest::HttpRequest(std::string request)
{
	requestParser(request);
}


HttpRequest::HttpRequest(const HttpRequest &copy) : Http(copy)
{
	*this = copy;
}


// Destructor
HttpRequest::~HttpRequest()
{
}

// Operators
HttpRequest & HttpRequest::operator=(const HttpRequest &assign)
{
	this->_version = assign._version;
	this->_methode = assign._methode;
	this->_uri = assign._uri;
	this->_protocol = assign._protocol;
	this->_hasBody = assign._hasBody;
	return *this;
}

// Member Function

void	HttpRequest::requestParser(std::string request)
{
	std::stringstream	requestStream(request);

	this->_hasBody = 0;

	std::getline(requestStream, this->_startLine);
	this->parseFirstLine();
	
	std::string line;
	std::getline(requestStream, line);
	while(*line.begin() != 13 && requestStream.eof() == 0)
	{
		this->addToHeaderField(line);
		std::getline(requestStream, line);
	}
	if (requestStream.eof() == 1)
		return ;
		
	this->_hasBody = 1;
	for (std::string bodyLine; std::getline(requestStream, bodyLine);)
		this->_body.append(bodyLine);
}

void	HttpRequest::parseFirstLine(void)
{
		std::stringstream startStream(this->_startLine);

		std::getline(startStream, this->_methode, ' ');	
		std::getline(startStream, this->_uri, ' ');	
		std::getline(startStream, this->_protocol, '/');	
		std::getline(startStream, this->_version);	
}

// Getter

std::string	HttpRequest::getVersion(void)
{
	return(this->_version);	
}

std::string	HttpRequest::getMethode(void)
{
	return(this->_methode);
}

std::string	HttpRequest::getUri(void)
{
	return(this->_uri);
}

std::string	HttpRequest::getProtocol(void)
{
	return(this->_protocol);
}

bool		HttpRequest::gethasBody(void)
{
	return(this->_hasBody);
}
