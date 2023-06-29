/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:15 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/29 15:42:41 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <string>

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

class Http
{
	public:
		// Constructors
		Http();
		Http(const Http &copy);
		 
		// Destructor
		virtual	~Http() = 0;
		
		// Operators
		Http & operator=(const Http &assign);

		// Setters
		virtual void	setStartLine(std::string startLine);
		virtual void	setBody(std::string body);
		virtual void	setHeaderField(std::map<std::string, std::string> headerField);
		virtual void	addToHeaderField(std::string headerToAdd);

		// Getters
		virtual std::string							GetStartLine(void) const;		
		virtual std::string							GetBody(void) const;
		virtual std::map<std::string, std::string>	GetHeaderField(void) const;


	protected:

		std::string							_startLine;
		std::string 						_body;
		std::map<std::string, std::string>	_headerField;
};

#endif