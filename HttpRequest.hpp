#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <string>
# include <string>
# include <iostream>
# include <vector>
# include <map>
# include <algorithm>
# include <set>
# include <cstring>
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

class HttpRequest
{
	public:
		// Constructors
		HttpRequest();
		HttpRequest(const HttpRequest &copy);
		
		// Destructor
		~HttpRequest();
		
		// Operators
		HttpRequest & operator=(const HttpRequest &assign);
		
	private:
		int									methode
		std::string							startLine;
		std::map<std::string, std::string>	headerField;
		std::string							body;
};

#endif