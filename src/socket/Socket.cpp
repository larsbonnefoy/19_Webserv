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

void	Socket::socketInit(const int port)
{
	int option;

	option = 1;
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

// Constructors
Socket::Socket() :	serverSocket(socket(AF_INET, SOCK_STREAM, 0)),
					socketAddressLen(sizeof(this->socketAddress))

{
	this->socketInit(DEFAULTPORT);
}

Socket::Socket(const int port) :	serverSocket(socket(AF_INET, SOCK_STREAM, 0)),
									socketAddressLen(sizeof(this->socketAddress))
{
	this->socketInit(port);
}

Socket::Socket(const Socket &copy) : 	serverSocket(copy.serverSocket),
										socketAddressLen(sizeof(this->socketAddress))
{
	this->socketAddress = copy.socketAddress;
	if (listen(this->serverSocket, 1) == -1)
		throw std::exception();
}


// Destructor
Socket::~Socket()
{
	close(this->clientSocket);
	close(this->serverSocket);
}


// Operators
Socket & Socket::operator=(const Socket &assign)
{
	(void)assign;
	return (*this);
}

// Getter
const char	*Socket::getRequest(void) const
{
	return ((char *)this->request);
}

int	Socket::getServerSocket(void) const
{
	return (this->serverSocket);
}

// Member Functions

int	Socket::connectClient(void)
{

	struct sockaddr_in	clientAddress;
	int					clientAddressLen = sizeof(clientAddress);
	
	this->clientSocket = accept(this->serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddressLen);
	std::stringstream stream;
	stream << "Client connected on port: " << ntohs(this->socketAddress.sin_port) << " from ip: " <<  ipAddressToString(htonl(clientAddress.sin_addr.s_addr));
	log(stream.str());
	if (this->clientSocket == -1)
		throw std::exception();
	return (this->clientSocket);
}

const char	*Socket::receiveRequest(void)
{
	// fcntl(this->clientSocket, F_SETFL, O_NONBLOCK);
	read(this->clientSocket , this->request, BUFF_SIZE);
	//if read < 0
	return ((char *)this->request);
}

void	Socket::sendResponse(const std::string response)
{
	write(this->clientSocket, response.c_str(), response.size());
	// if write < 0
	close(this->clientSocket);
}
