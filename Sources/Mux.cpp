#include "../Includes/Mux.hpp"
#include "../Includes/HttpResponse.hpp"

static const std::string httpResponse = "HTTP/1.1 200 OK\r\n"
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
	// this->_conf = DEFAULTCONF;
	// this->initSockets();
}

Mux::Mux(Config	&conf) :	_conf(conf),
							_serverMap(conf.getServers())
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
	for (size_t i = 0; i < this->_nbrSocket; ++i)
	{
		delete this->_Sockets[i];
	}
	this->_Sockets.clear();
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
		if (returnPoll < 0)
			throw PollException();
		else if (returnPoll > 0)
		{
			for (size_t i = 0; i < this->_nbrSocket; ++i)
			{
				if (this->_pollSocketFds[i].revents & POLLIN)
				{
					//connect client maybe change program flow from receive parse read to receive all parse all send all 
					this->_Sockets[i]->connectClient();
					const std::string request = this->_Sockets[i]->receiveRequest();
					ws_logFile(request);
                    HttpResponse placeholder("/Users/larsbonnefoy/projects/19_Webserv/site/data/www/home.html", 200);
					this->_Sockets[i]->sendResponse(placeholder.convertToStr());	
					this->_Sockets[i]->closeClient();			
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

	this->_nbrSocket = this->_conf.getServers().size();
	for (std::map<size_t, Server>::iterator it = this->_serverMap.begin() ; it != this->_serverMap.end(); ++it)
		this->_Sockets.push_back(new Socket(it->first));	
	this->_pollSocketFds = new pollfd[this->_nbrSocket];
	for (size_t i = 0; i <this->_nbrSocket; ++i)
	{
		this->_pollSocketFds[i].fd = this->_Sockets[i]->getServerSocket();
		this->_pollSocketFds[i].events = POLLIN | POLLPRI;
	}
}

// Execptions

const char* Mux::PollException::what() const throw()
{
	ws_logErr("[Mux] : Poll Failure");
	return ("[Mux] : Poll Failure");
}

const char* Mux::InitSocketException::what() const throw()
{
	ws_logErr("[Mux] : Socket Initialization Failure");
	return ("[Mux] : Socket Initialization Failure");
}
