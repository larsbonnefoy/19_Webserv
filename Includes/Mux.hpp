#ifndef MUX_HPP
# define MUX_HPP

# include <iostream>
# include <string>
# include <iostream>
# include <vector>
# include <map>
# include <algorithm>
# include <set>
# include <cstring>
# include <iostream>
# include <exception>

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>

# include "Log.hpp"
# include "Socket.hpp"
# include "Config.hpp"
# include "ConfigParser.hpp"
# include "Server.hpp"

# define TIMEOUT -1 

class Mux
{
	public:
		// Constructors
		Mux();
		Mux(Config &conf);
		Mux(const Mux &copy);
		
		// Destructor
		~Mux();
		
		// Operators
		Mux & operator=(const Mux &assign);

		// Member Functions
		void run(void);

	private:
		Config						_conf;
		std::vector<Socket *>		_Sockets;
		size_t						_nbrSocket;// nbrsoxket = nbrserver
		struct pollfd				*_pollSocketFds;
		std::map<size_t, Server>	_serverMap;
		
		// Private Functions
		void	initSockets();
	
	// Exceptions
	class PollException : public std::exception
	{
		public:
			virtual const char* what() const throw();
	};
	
	class InitSocketException : public std::exception
	{
		public:
			virtual const char* what() const throw();
	};


};

#endif