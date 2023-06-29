/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:04 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/29 16:48:35 by hdelmas          ###   ########.fr       */
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


HttpRequest::HttpRequest(const HttpRequest &copy)
{
	(void) copy;
}


// Destructor
HttpRequest::~HttpRequest()
{
}


// Operators
HttpRequest & HttpRequest::operator=(const HttpRequest &assign)
{
	(void) assign;
	return *this;
}

// Member Function

void	HttpRequest::requestParser(std::string request)
{
	std::stringstream	requestStream(request);

	this->_hasBody = 0;

	std::getline(requestStream, this->_startLine);
	ws_log(this->_startLine);
	this->parseFirstLine();
	std::string line;
	std::getline(requestStream, line);
	while(*line.begin() != 13 && requestStream.eof() == 0)
	{
		this->addToHeaderField(line);
		// ws_log(line);
		std::getline(requestStream, line);
		// ws_log((int)line.c_str()[0]);
	}

	if (requestStream.eof() == 1)
		return ;
	this->_hasBody = 1;
	for (std::string bodyLine; std::getline(requestStream, bodyLine);)
		this->_body.append(bodyLine);
	ws_log(this->_body);
}

void	HttpRequest::parseFirstLine(void)
{
		std::stringstream startStream(this->_startLine);

		std::getline(startStream, this->_methode, ' ');	
		std::getline(startStream, this->_uri, ' ');	
		std::getline(startStream, this->_protocol, '/');	
		std::getline(startStream, this->_version);	
}