#include "../Includes/Http.hpp"

// Constructors
Http::Http()
{
	std::cout << "\e[0;33mDefault Constructor called of Http\e[0m" << std::endl;
}

Http::Http(const Http &copy)
{
	(void) copy;
	std::cout << "\e[0;33mCopy Constructor called of Http\e[0m" << std::endl;
}


// Destructor
Http::~Http()
{
	std::cout << "\e[0;31mDestructor called of Http\e[0m" << std::endl;
}


// Operators
Http & Http::operator=(const Http &assign)
{
	(void) assign;
	return *this;
}

// Setters
void	Http::setVersion(std::string version)
{
	this->_version = version;
}

void	Http::setBody(std::string body)
{
	this->_body = body;
}

void	Http::setHeaderField(std::map<std::string, std::string> headerField)
{
	this->_headerField = headerField;
}

// Getters
std::string							Http::GetVersion(void) const
{
	return (this->_version);
}

std::string							Http::GetBody(void) const
{
	return (this->_body);
}

std::map<std::string, std::string>	Http::GetHeaderField(void) const
{
	return (this->_headerField);
}