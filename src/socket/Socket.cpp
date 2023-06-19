#include "Socket.hpp"

// Constructors
Socket::Socket()
{
	std::cout << "\e[0;33mDefault Constructor called of Socket\e[0m" << std::endl;
}

Socket::Socket(const std::string &host, const std::string &port)
{
	std::cout << "\e[0;33mConstructor called of Socket\e[0m" << std::endl;
	(void) host;
	(void) port;
}

Socket::Socket(const Socket &copy)
{
	(void) copy;
	std::cout << "\e[0;33mCopy Constructor called of Socket\e[0m" << std::endl;
}


// Destructor
Socket::~Socket()
{
	std::cout << "\e[0;31mDestructor called of Socket\e[0m" << std::endl;
}


// Operators
Socket & Socket::operator=(const Socket &assign)
{
	(void) assign;
	return *this;
}

