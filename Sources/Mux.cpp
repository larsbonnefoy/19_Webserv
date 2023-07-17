#include "../Includes/Mux.hpp"
#include "../Includes/HttpResponse.hpp"
#include "../Includes/HttpRequest.hpp"
#include "../Includes/RequestHandler.hpp"

// Constructors
Mux::Mux()
{
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
	size_t	nbrfds = this->_nbrSocket;
	// size_t	newNbrfds = this->_nbrSocket;
	std::map<int, Socket*>	fdToSocket;
	while (1)
	{
		int returnPoll = poll(this->_pollSocketFds, nbrfds, TIMEOUT);
		if (returnPoll < 0)
			throw PollException();
		else if (returnPoll > 0)
		{
			size_t	nbrfds = this->_nbrSocket;
			for (size_t i = 0; i < nbrfds; ++i)
			{
				if (this->_pollSocketFds[i].revents & POLLIN)
				{
					try
					{
						if (i < this->_nbrSocket)
						{
							int clientFd = this->_Sockets[i]->connectClient();
							nbrfds++;

							pollfd *tmp = new pollfd[nbrfds];
							for (size_t j = 0; j < nbrfds - 1; ++j)
							{
								tmp[j].fd = this->_pollSocketFds[j].fd;
								tmp[j].events = this->_pollSocketFds[j].events;
								tmp[j].revents = this->_pollSocketFds[j].revents;
							}
							tmp[nbrfds - 1].fd = clientFd;
							tmp[nbrfds - 1].events = POLLIN;
							tmp[nbrfds - 1].revents = POLLIN;
							delete [] this->_pollSocketFds;
							this->_pollSocketFds = tmp;

							fdToSocket[clientFd] = this->_Sockets[i];
							this->_pollSocketFds[i].revents = 0;
						}
						else
						{
							if (fdToSocket[this->_pollSocketFds[i].fd])
							{
								Socket	*sock = fdToSocket[this->_pollSocketFds[i].fd];

								const std::string request = sock->receiveRequest(this->_pollSocketFds[i].fd);
								ws_log(request);
								HttpRequest Request(request);
								HttpResponse response(this->_serverMap[sock->getPort()], Request);
                	    		// ws_log(response.convertToStr());
								sock->sendResponse(this->_pollSocketFds[i].fd, response.convertToStr());	
								close(this->_pollSocketFds[i].fd);
								ws_log("Connection closed");
								this->_pollSocketFds[i].fd = -1;
								this->_pollSocketFds[i].revents = 0;
								this->_pollSocketFds[i].events = 0;
							}
						}
					}
					catch(const std::exception& e)
					{
						this->_pollSocketFds[i].fd = -1;
						if (i < this->_nbrSocket) 
						{
							this->_Sockets[i]->sc_close();
						}
						ws_logErr(e.what());
					}
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
		this->_Sockets.push_back(new Socket(it->second.getIp(), it->first));	
	this->_pollSocketFds = new pollfd[this->_nbrSocket];
	for (size_t i = 0; i < this->_nbrSocket; ++i)
	{
		this->_pollSocketFds[i].fd = this->_Sockets[i]->getServerSocket();
		this->_pollSocketFds[i].events = POLLIN;
	}
}

// Execptions

const char* Mux::PollException::what() const throw()
{
	return ("[Mux] : Poll Failure");
}

const char* Mux::InitSocketException::what() const throw()
{
	return ("[Mux] : Socket Initialization Failure");
}
