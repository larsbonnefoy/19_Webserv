#include "Mux.hpp"

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
	std::cout << "\e[0;31mDestructor called of Mux\e[0m" << std::endl;
}


// Operators
Mux & Mux::operator=(const Mux &assign)
{
	(void) assign;
	return *this;
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
			this->_Sockets.push_back(Socket(server.getPort()));		
			ports.insert(server.getPort());
			nbrSocket++;
		}
	}
	if (nbrSocket != this->_Sockets.size() != ports.size())
	{
		//some error
	}
	//init pollfd new struct pollfd[nbrSockeT]

}

