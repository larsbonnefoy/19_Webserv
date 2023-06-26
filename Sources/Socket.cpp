#include "Socket.hpp"

// Static functions

static std::string ipAddressToString(uint32_t ipAddress) {
    std::stringstream ipString;
    ipString << ((ipAddress >> 24) & 0xFF);
    ipString << ".";
    ipString << ((ipAddress >> 16) & 0xFF);
    ipString << ".";
    ipString << ((ipAddress >> 8) & 0xFF);
    ipString << ".";
    ipString << (ipAddress & 0xFF);
    return (ipString.str());
}

void	Socket::socketInit(const uint32_t port)
{
	int option;

	option = 1;
	if (this->_serverSocket == -1)
		throw std::exception();
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, static_cast<socklen_t>(sizeof(option))))
		throw std::exception();
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEPORT, &option, static_cast<socklen_t>(sizeof(option))))
		throw std::exception();
	this->_socketAddress.sin_family = AF_INET;
	this->_socketAddress.sin_addr.s_addr = INADDR_ANY;
	this->_socketAddress.sin_port = htons(port);
	if (bind(this->_serverSocket, reinterpret_cast<struct sockaddr *>(&this->_socketAddress), static_cast<socklen_t>(this->_socketAddressLen)) == -1)
		throw std::exception();
	if (listen(this->_serverSocket, 1) == -1)
		throw std::exception();
}

// Constructors
Socket::Socket() :	_serverSocket(socket(AF_INET, SOCK_STREAM, 0)),
					_socketAddressLen(sizeof(this->_socketAddress)),
					_port(DEFAULTPORT)

{
	this->socketInit(DEFAULTPORT);
}

Socket::Socket(const uint32_t port) :	_serverSocket(socket(AF_INET, SOCK_STREAM, 0)),
									_socketAddressLen(sizeof(this->_socketAddress)),
									_port(port)
{
	this->socketInit(port);
}

Socket::Socket(const Socket &copy) : 	_serverSocket(copy._serverSocket),
										_socketAddressLen(sizeof(this->_socketAddress)),
										_port(copy.getPort())
{
	this->_socketAddress = copy._socketAddress;
	if (listen(this->_serverSocket, 1) == -1)
		throw std::exception();
}


// Destructor
Socket::~Socket()
{
	close(this->_clientSocket);
	close(this->_serverSocket);
}


// Operators
Socket & Socket::operator=(const Socket &assign)
{
	(void)assign;
	return (*this);
}

bool Socket::operator==(const Socket &rhs)
{
	return (this->_port == rhs.getPort());
}

bool Socket::operator<(const Socket &rhs)
{
	return (this->_port < rhs.getPort());
}

bool Socket::operator<=(const Socket &rhs)
{
	return (*this < rhs || *this == rhs);
}


bool Socket::operator>(const Socket &rhs)
{
	return (!(*this < rhs || *this == rhs));
}


bool Socket::operator>=(const Socket &rhs)
{
	return (!(*this < rhs));
}

// Getter
const char	*Socket::getRequest(void) const
{
	return ((char *)this->_request);
}

int	Socket::getServerSocket(void) const
{
	return (this->_serverSocket);
}

uint32_t	Socket::getPort(void) const
{
	return (this->_port);
}

// Member Functions

int	Socket::connectClient(void)
{

	struct sockaddr_in	clientAddress;
	int					clientAddressLen = sizeof(clientAddress);
	
	this->_clientSocket = accept(this->_serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddressLen);
	std::stringstream stream;
	stream << "Client connected on port: " << ntohs(this->_socketAddress.sin_port) << " from ip: " <<  ipAddressToString(htonl(clientAddress.sin_addr.s_addr));
	ws_log(stream.str());
	if (this->_clientSocket == -1)
		throw std::exception();
	fcntl(this->_clientSocket, F_SETFL, O_NONBLOCK);
	return (this->_clientSocket);
}

const char	*Socket::receiveRequest(void)
{
	// fcntl(this->_clientSocket, F_SETFL, O_NONBLOCK);
	read(this->_clientSocket , this->_request, BUFF_SIZE);
	//if read < 0
	return ((char *)this->_request);
}

void	Socket::sendResponse(const std::string response)
{
	write(this->_clientSocket, response.c_str(), response.size());
	// if write < 0
	close(this->_clientSocket);
}

void	Socket::close(void)
{
	close(this->_clientSocket);
}