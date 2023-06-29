/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:04 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/29 14:04:13 by hdelmas          ###   ########.fr       */
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



	std::string line;
	std::getline(requestStream, line);
	while(line != "" && requestStream.eof() == 0)
	{
		this->addToHeaderField(line);
		std::getline(requestStream, line);
	}

	if (requestStream.eof() == 1)
		return ;
	this->_hasBody = 1;
	//
}
