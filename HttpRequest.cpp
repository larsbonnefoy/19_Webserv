#include "HttpRequest.hpp"

// Constructors
HttpRequest::HttpRequest()
{
	std::cout << "\e[0;33mDefault Constructor called of HttpRequest\e[0m" << std::endl;
}

HttpRequest::HttpRequest(const HttpRequest &copy)
{
	(void) copy;
	std::cout << "\e[0;33mCopy Constructor called of HttpRequest\e[0m" << std::endl;
}


// Destructor
HttpRequest::~HttpRequest()
{
	std::cout << "\e[0;31mDestructor called of HttpRequest\e[0m" << std::endl;
}


// Operators
HttpRequest & HttpRequest::operator=(const HttpRequest &assign)
{
	(void) assign;
	return *this;
}

