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

// Getters
std::string							Http::GetStartLine(void) const
{
	return (this->_startLine);
}

std::string							Http::GetBody(void) const
{
	return (this->_body);
}

std::map<std::string, std::string>	Http::GetHeaderField(void) const
{
	return (this->_headerField);
}