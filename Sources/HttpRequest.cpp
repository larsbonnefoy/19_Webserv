/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:04 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/18 12:44:59 by hdelmas          ###   ########.fr       */
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
	this->_payload = assign._payload;
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
		std::string tmp;
		for (size_t i = 0; i < line.size(); ++i)
			if (!std::isspace(line[i]))
				tmp += line[i];
		line = tmp;
		this->addToHeaderField(line);
		std::getline(requestStream, line);
	}
	if (requestStream.eof() == 1)
		return ;
		
	this->_hasBody = 1;
	while (requestStream.eof() != 1)	
	{
		char str[BUFF_SIZE]; requestStream.read(str, BUFF_SIZE);
		this->_body.append(str, requestStream.gcount());
	}
	this->_body.append("\0");
}

void	HttpRequest::_parseUrl(std::string url)
{
		std::stringstream urlStream(url);

		this->_payload = "";
		std::getline(urlStream, this->_uri, '?');
		std::getline(urlStream, this->_payload);
		//this->_payload = "?" + this->_payload;
}
void	HttpRequest::parseFirstLine(void)
{
		std::stringstream startStream(this->_startLine);
		std::string		  url;
		
		std::getline(startStream, this->_methode, ' ');	
		std::getline(startStream, url, ' ');	
		this->_parseUrl(url);
		std::getline(startStream, this->_protocol, '/');	
		std::getline(startStream, this->_version);	
}

// Getter
std::string	HttpRequest::getName(void)
{
	std::stringstream	host(this->getHeaderField()["Host"]);
	std::string			name;

	std::getline(host, name, ':');
	return (name);
}

std::string	HttpRequest::getVersion(void)
{
	return(this->_version);	
}

std::string	HttpRequest::getMethode(void)
{
	return(this->_methode);
}

std::string	HttpRequest::getPayload(void)
{
	return(this->_payload);
}

std::string	HttpRequest::getProtocol(void)
{
	return(this->_protocol);
}

bool		HttpRequest::gethasBody(void)
{
	return(this->_hasBody);
}
