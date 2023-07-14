#include "../Includes/Socket.hpp"

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

static uint32_t ipStringToULong(const std::string &ip) {
    
	std::stringstream	ipSream(ip);
	uint32_t			res = 0;
	int iter = 3;

	for (std::string digits; std::getline(ipSream, digits, '.'); iter--)
	{

		for (size_t i = 0; i < digits.size(); ++i)
		{
			if (!std::isdigit(digits[i]) || digits.size() > 3)
			{
				ws_logErr("badip");
				throw Socket::InitSocketException();
			}
		}

		std::stringstream	digitsStream(digits);
		uint32_t			toAdd = 0;

		digitsStream >> toAdd;
		ws_log(digitsStream.str());
		ws_log(toAdd);
		res += toAdd << (8 * iter);
		ws_log(res);
	}
	if (iter != -1)
	{
		ws_logErr("badip format");
		throw Socket::InitSocketException();
	}
	ws_log(ipAddressToString(res));	
	return (res);
}

void	Socket::socketInit(const std::string ip, const uint32_t port)
{
	int option;

	option = 1;
	if (this->_serverSocket == -1)
		throw InitSocketException();
	fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK);
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &option,
					static_cast<socklen_t>(sizeof(option))))
		throw InitSocketException();

	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEPORT, &option,
					static_cast<socklen_t>(sizeof(option))))
		throw InitSocketException();

	this->_socketAddress.sin_family = AF_INET;
	this->_socketAddress.sin_addr.s_addr = htonl(ipStringToULong(ip));
	this->_socketAddress.sin_port = htons(port);

	if (bind(this->_serverSocket,
					reinterpret_cast<struct sockaddr *>(&this->_socketAddress),
					static_cast<socklen_t>(this->_socketAddressLen)) == -1)
		throw InitSocketException();
	if (listen(this->_serverSocket, 50) == -1)
		throw InitSocketException();
}

// Constructors
Socket::Socket()
{
}

Socket::Socket(const std::string ip, const uint32_t port) :	_serverSocket(socket(AF_INET, SOCK_STREAM, 0)),
										_socketAddressLen(sizeof(this->_socketAddress)),
										_port(port)
{
	this->socketInit(ip, port);
}

Socket::Socket(const Socket &copy)
{
	*this = copy;
	if (listen(this->_serverSocket, 50) == -1)
		throw InitSocketException();
}


// Destructor
void	Socket::sc_close(void)
{
	// close(this->_clientSocket);
	close(this->_serverSocket);
}

Socket::~Socket()
{
	this->sc_close();
}


// Operators
Socket & Socket::operator=(const Socket &assign)
{
	// this->_clientSocket = assign._clientSocket;
	this->_serverSocket = assign._serverSocket;
	this->_socketAddress = assign._socketAddress;
	this->_port = assign._port;
	this->_socketAddressLen = assign._socketAddressLen;
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
const std::string	Socket::getRequest(void) const
{
	return (this->_request);
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
	
	int	clientSocket = accept(this->_serverSocket,
			(struct sockaddr *)&clientAddress, (socklen_t *)&clientAddressLen);
	if (clientSocket == -1)
		throw InitSocketException();
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	std::stringstream stream;
	this->_clientIp = ipAddressToString(htonl(clientAddress.sin_addr.s_addr));
	stream << "Client connected on port: "
			<< ntohs(this->_socketAddress.sin_port)
			<< " from ip: " << this->_clientIp;
	ws_log(stream.str());
	return (clientSocket);
}

static bool	isChounked(std::string request)
{
	bool				res = false;
	std::stringstream	requestStream(request);

	while (!res && requestStream.good())
	{
		std::string line;
		std::getline(requestStream, line);
		if(line.find("Transfer-Encoding:") == 0)
			if(line.find("chunked") < std::string::npos)
				res = true;
	}
	ws_log("isChounked");
	ws_log(res);
	return (res);
}

static std::string	unChounk(std::stringstream &stream, size_t size)
{
	if (size == 0)
		return ("");
	std::string	res = "";
	std::string	line;

	ws_log("YOOO");
	std::getline(stream, line);
	ws_log(line);
	if (!stream.good())
		return (res);
	if (line.size() < size)
			size = line.size();
	for (size_t	i = 0; i < size; ++i) 
		res += line[i];
	res.append("\r\n");
	return (res);
}

static std::string	unChounkInit(std::string request)
{
	std::string			newRequest;
	std::stringstream	requestStream(request);
	bool				inBody = false;

	for  (std::string line; std::getline(requestStream, line); requestStream.good())
	{
		if (!inBody)
		{
			if (line == "\r")
			{
				inBody = true;
				newRequest.append("\r\n");
			}
			else
			{
				newRequest.append(line);
				newRequest.append("\r\n");
			}
		}
		else
		{
			size_t	size;
			std::istringstream(line) >> std::hex >> size;
			newRequest.append(unChounk(requestStream, size));
		}
	}
	return (newRequest);
}

const std::string	Socket::receiveRequest(int clientFd)
{
	ssize_t	returnRead = 1;
	this->_request = "";
	while (returnRead != -1)
	{
		char	buffer[BUFF_SIZE + 1];
		returnRead = recv(clientFd, buffer, BUFF_SIZE, 0);
		ws_log(strerror(errno));
		ws_log(returnRead);
		ws_log("client");
		ws_log(clientFd);
		if (returnRead < 0)
		{
			this->_request.append("\0");
			break ;
		}
		buffer[returnRead] = 0;
		this->_request.append(buffer);
		if (returnRead < BUFF_SIZE)
			break ;
	}
	bool chounkedCheck = isChounked(this->_request);
	if (chounkedCheck)
	{
		returnRead = 1;
		ws_log("hmmm");
		this->_request = unChounkInit(this->_request);
		this->sendResponse(clientFd, "HTTP/1.1 100 Continue");
		size_t	size = 1;
		char	buffer[BUFF_SIZE + 1];

		while (size != 0)
		{
			returnRead = recv(clientFd, buffer, BUFF_SIZE, 0);
			if (returnRead < 0)
				return (this->_request);
			buffer[returnRead] = 0;

			std::stringstream	bufferStream(buffer);
			while (bufferStream.good())
			{
				std::string	sizeStr;
				std::getline(bufferStream, sizeStr);
				if (!bufferStream.good())
					break ;
				std::istringstream(sizeStr) >> std::hex >> size;
				if (size == 0)
					break ;
				this->_request.append(unChounk(bufferStream, size));
			}
		}		
	}	
	return (this->_request);
}

void	Socket::sendResponse(int clientFd, const std::string response)
{
	size_t	i;
	size_t	size = BUFF_SIZE;
	const char *toWrite = response.c_str();
	size_t	remainingSize = response.size();

	i = 0;
	while (i < response.size())
	{
		if (remainingSize < BUFF_SIZE)
			size = remainingSize;
		ssize_t	retWrite = write(clientFd, &toWrite[i], size);
		if (retWrite < 0)
		{
			ws_log("Need to close socket and not kill the server");
			throw IoException();
		}
		remainingSize -= retWrite;
		i += retWrite;
	}
}

// void	Socket::closeClient(void)
// {
// 	std::stringstream stream;
// 	stream << "Client disconnected on port: "
// 			<< ntohs(this->_socketAddress.sin_port)
// 			<< " from ip: " << this->_clientIp; 
// 	ws_log(stream.str());
// 	close(this->_clientSocket);
// }

const char* Socket::InitSocketException::what() const throw()
{
	ws_logErr("[Socket] : Socket Initialization Failure");
	return ("[Socket] : Socket Initialization Failure");
}

const char* Socket::IoException::what() const throw()
{
	ws_logErr("[Socket] : IO Failure");
	return ("[Socket] : IO Failure");
}
