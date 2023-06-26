#include "../Includes/Mux.hpp"

static const char *httpResponse = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: 617\r\n"
                          "\r\n"
                          "<!DOCTYPE html>\r\n"
                          "<html lang=\"en\">\r\n"
                          "<head>\r\n"
                          "  <meta charset=\"UTF-8\" />\r\n"
                          "  <title>Hello, world!</title>\r\n"
                          "  <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\" />\r\n"
                          "  <meta name=\"description\" content=\"\" />\r\n"
                          "  <link rel=\"icon\" href=\"favicon.png\">\r\n"
                          "</head>\r\n"
                          "<body>\r\n"
                          "    <h1>Hello</h1>\r\n"
                          "    <form method=\"POST\">\r\n"
                          "        <label for=\"fname\">First name:</label><br>\r\n"
                          "        <input type=\"text\" id=\"fname\" name=\"fname\"><br>\r\n"
                          "        <label for=\"lname\">Last name:</label><br>\r\n"
                          "        <input type=\"text\" id=\"lname\" name=\"lname\"><br><br>\r\n"
                          "        <input type=\"submit\" value=\"Submit\">\r\n"
                          "    </form>\r\n"
                          "</body>\r\n"
                          "</html>";
// Constructors
Mux::Mux()
{
	std::cout << "\e[0;33m TODO Default Constructor called of Mux\e[0m" << std::endl;
	// this->init(deflautConf);
}

Mux::Mux(Config	&conf) : _conf(conf)
{
	this->initSockets();	
}


Mux::Mux(const Mux &copy)
{
	(void) copy;
}


// Destructor
Mux::~Mux()
{
	delete [] this->_pollSocketFds;
}


// Operators
Mux & Mux::operator=(const Mux &assign)
{
	(void) assign;
	return *this;
}

// Member Functions
void	Mux::run(void)
{
	while (1)
	{
		int returnPoll = poll(this->_pollSocketFds, this->_nbrSocket , TIMEOUT);
		// ws_log(returnPoll);
		if (returnPoll < 0)
			throw std::exception();
		else if (returnPoll > 0)
		{
			for (size_t i = 0; i < this->_nbrSocket; ++i)
			{
				// ws_log(this->_pollSocketFds[i].fd);
				if (this->_pollSocketFds[i].revents & POLLIN)
				{
					ws_log("in");
					this->_Sockets[i].connectClient();
					const char *buffer = this->_Sockets[i].receiveRequest();
					ws_logFile(buffer);
					ws_log("DONE");
					this->_Sockets[i].sendResponse(httpResponse);			
					this->_Sockets[i].closeClient();			
					ws_logFile(httpResponse);
					this->_pollSocketFds[i].revents = 0;
				}
			}			
		}
	}
}

// Private Function
void	Mux::initSockets()
{
	size_t				nbrServer;
	size_t				nbrSocket;
	std::set<uint32_t>	ports;

	nbrServer = this->_conf.getServers().size();
	nbrSocket = 0; 
	for (size_t i = 0; i < nbrServer; ++i)
	{
		Server server = this->_conf.getServers()[i];

		if (std::find(ports.begin(), ports.end(), server.getPort()) == ports.end())
		{
			ws_log(server.getPort());
			Socket sock(server.getPort());
			this->_Sockets.push_back(sock);		
			// this->_Sockets.push_back(Socket(server.getPort()));		
			ws_log(this->_Sockets[i].getServerSocket());
			ports.insert(server.getPort());
			nbrSocket++;
		}
	}
	if (nbrSocket != this->_Sockets.size() || nbrSocket != ports.size())
	{
		//some error
		throw std::exception();
	}
	//init pollfd new struct pollfd[nbrSockeT]
	this->_nbrSocket = nbrSocket;
	this->_pollSocketFds = new pollfd[nbrSocket];
	// this->_pollSocketFds = fdpoll;
	for (size_t i = 0; i < nbrSocket; ++i)
	{
		this->_pollSocketFds[i].fd = this->_Sockets[i].getServerSocket();
		this->_pollSocketFds[i].events = POLLIN | POLLPRI;

		ws_log(i);
		ws_log(this->_pollSocketFds[i].events);
	}
}

