#include "Socket.hpp"

// Constructors
Socket::Socket()
{
	std::cout << "\e[0;33mDefault Constructor called of Socket\e[0m" << std::endl;
	int option;

	option = 1;
	this->socketAddressLen = sizeof(this->socketAddress);
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == -1)
		throw std::exception();
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) == -1)
		throw std::exception();
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_addr.s_addr = INADDR_ANY;
	this->socketAddress.sin_port = htons(DEFAULTPORT);
	if (bind(this->serverSocket, (struct sockaddr *)&this->socketAddress, (socklen_t)this->socketAddressLen) == -1)
		throw std::exception();
	if (listen(this->serverSocket, 1) == -1)
		throw std::exception();
}

Socket::Socket(const int port)
{
	std::cout << "\e[0;33mConstructor called of Socket\e[0m" << std::endl;
	int option;

	option = 1;
	this->socketAddressLen = sizeof(this->socketAddress);
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == -1)
		throw std::exception();
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) == -1)
		throw std::exception();
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_addr.s_addr = INADDR_ANY;
	this->socketAddress.sin_port = htons(port);
	if (bind(this->serverSocket, (struct sockaddr *)&this->socketAddress, (socklen_t)this->socketAddressLen) == -1)
		throw std::exception();
	if (listen(this->serverSocket, 1) == -1)
		throw std::exception();
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
	close(this->requestSocket);
	close(this->serverSocket);
}


// Operators
Socket & Socket::operator=(const Socket &assign)
{
	(void) assign;
	return *this;
}

// Getter
const char	*Socket::getRequest(void) const
{
	return ((char *)this->request);
}

// Member Functions

const char	*Socket::receiveRequest(void)
{
	this->requestSocket = accept(this->serverSocket, (struct sockaddr *)&this->socketAddress, (socklen_t *)&this->socketAddressLen);
	// fcntl(this->requestSocket, F_SETFL, O_NONBLOCK);
	if (this->requestSocket == -1)
		throw std::exception();
	read(this->requestSocket , this->request, BUFF_SIZE);
	//if read < 0
	return ((char *)this->request);
}

void	Socket::sendResponse(const std::string response)
{
	write(this->requestSocket, response.c_str(), response.size());
	// if write < 0
	close(this->requestSocket);
}
