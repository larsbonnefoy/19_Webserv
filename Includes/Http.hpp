#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <string>

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

class Http
{
	public:
		// Constructors
		Http() {};
		Http(const Http &copy) {(void)copy;};
		 
		// Destructor
		virtual	~Http() = 0;
		
		// Operators
		Http & operator=(const Http &assign) {(void)assign;};

		// Setters
		virtual void	setVersion(std::string version);
		virtual void	setBody(std::string body);
		virtual void	setHeaderField(std::map<std::string, std::string> headerField);

		// Getters
		virtual std::string							GetVersion(void);		
		virtual std::string							GetBody(void);
		virtual std::map<std::string, std::string>	GetHeaderField(void);


	protected:

		std::string							_version;
		std::string 						_body;
		std::map<std::string, std::string>	_headerField;
};

#endif