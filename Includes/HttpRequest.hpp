/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:06 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/29 16:47:51 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <string>
# include <sstream>
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
# include "Http.hpp"

class HttpRequest : public Http
{
	public:
		// Constructors
		HttpRequest();
		HttpRequest(std::string request);
		HttpRequest(const HttpRequest &copy);
		
		// Destructor
		~HttpRequest();
		
		// Operators
		HttpRequest & operator=(const HttpRequest &assign);

		//Member Functions
		void	requestParser(std::string request);
		void	HttpRequest::parseFirstLine(void);
		
	private:
		std::string _version;
		std::string _methode;
		std::string _uri;
		std::string _protocol;
		bool		_hasBody;


};

#endif