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
	extern bool g_isGood;
    g_isGood = true;

	size_t	nbrfds = this->_nbrSocket;
	size_t	newNbrfds = this->_nbrSocket;
	std::map<int, Socket*>	fdToSocket;
	while (g_isGood)
	{
		nbrfds = newNbrfds;
		int returnPoll = poll(this->_pollSocketFds, nbrfds, TIMEOUT);
		if (g_isGood == false)
			return ;
		if (returnPoll < 0)
			throw PollException();
		else if (returnPoll > 0)
		{
			for (size_t i = 0; i < nbrfds; ++i)
			{
				if (this->_pollSocketFds[i].revents & POLLIN)
				{
					try
					{
						if (i < this->_nbrSocket)
						{
							int clientFd = this->_Sockets[i]->connectClient();
							newNbrfds++;

							pollfd *tmp = new pollfd[newNbrfds];
							for (size_t j = 0; j < newNbrfds - 1; ++j)
							{
								tmp[j].fd = this->_pollSocketFds[j].fd;
								tmp[j].events = this->_pollSocketFds[j].events;
								tmp[j].revents = this->_pollSocketFds[j].revents;
							}
							tmp[newNbrfds - 1].fd = clientFd;
							tmp[newNbrfds - 1].events = POLLIN;
							tmp[newNbrfds - 1].revents = 0;
							delete [] this->_pollSocketFds;
							this->_pollSocketFds = tmp;
							fdToSocket[clientFd] = this->_Sockets[i];
							this->_pollSocketFds[i].revents = 0;
							// usleep(2000);
						}
						else if (fdToSocket[this->_pollSocketFds[i].fd] && this->_pollSocketFds[i].revents & POLLIN)
						{
							Socket	*sock = fdToSocket[this->_pollSocketFds[i].fd];
							const std::string request = sock->receiveRequest(this->_pollSocketFds[i].fd);
							ws_logFile(request);
							HttpRequest Request(request);
							HttpResponse response(this->_serverMap[sock->getPort()], Request);
							sock->sendResponse(this->_pollSocketFds[i].fd, response.convertToStr());	
							close(this->_pollSocketFds[i].fd);
							ws_logFile("Client closed");
							this->_pollSocketFds[i].fd = -1;
							this->_pollSocketFds[i].revents = 0;
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
			size_t newNbrSave = newNbrfds;		
			for (size_t i = this->_nbrSocket; i < newNbrSave; ++i)
			{
				if(this->_pollSocketFds[i].fd == -1)
					newNbrfds--;
			}
			pollfd *tmp = new pollfd[newNbrfds];
			size_t	j = 0;
			for (size_t i = 0; i < newNbrSave; ++i)
			{
				if (j < this->_nbrSocket || this->_pollSocketFds[i].fd != -1)
				{
					tmp[j].fd = this->_pollSocketFds[i].fd;
					tmp[j].events = this->_pollSocketFds[i].events;
					tmp[j].revents = this->_pollSocketFds[i].revents;
					j++;
				}
			}	
			delete [] this->_pollSocketFds;
			this->_pollSocketFds = tmp;
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
