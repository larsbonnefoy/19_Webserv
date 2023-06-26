#ifndef MUX_HPP
# define MUX_HPP

# include <iostream>
# include <string>
# include <iostream>
# include <vector>
# include <algorithm>
# include <set>
# include <cstring>
# include <iostream>

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
		Config				_conf;
		std::vector<Socket *>	_Sockets;
		size_t				_nbrSocket;
		struct pollfd		*_pollSocketFds;
		
		// Private Functions
		void	initSockets();
		
};

#endif