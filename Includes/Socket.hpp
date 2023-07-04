/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 11:19:01 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/03 12:58:45 by hdelmas          ###   ########.fr       */
/*   Updated: 2023/06/19 18:10:45by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>
# include <string>
# include <cstring>
# include <iostream>
# include <exception>

# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>

# include "Log.hpp"

# define PORT 8282
# define DEFAULTPORT 8080
# define BUFF_SIZE 30000 

class Socket
{
	public:
		// Constructors
		Socket();
		Socket(const Socket &copy);
		Socket(const uint32_t port);

		// Destructor
		~Socket();
		
		// Operators
		Socket & operator=(const Socket &assign);
		bool operator==(const Socket &rhs);
		bool operator<(const Socket &rhs);
		bool operator<=(const Socket &rhs);
		bool operator>(const Socket &rhs);
		bool operator>=(const Socket &rhs);
		
		// Getter
		const std::string	getRequest(void) const;
		int					getServerSocket(void) const;
		uint32_t			getPort(void) const;

		// MemberFunctions
		const std::string	receiveRequest(void);
		int					connectClient(void);
		void				sendResponse(const std::string response);		
		void				socketInit(const uint32_t port);
		void				closeClient(void);

	private:
		int 				_serverSocket;
		int					_socketAddressLen;
		uint32_t			_port; //ID
	 	struct sockaddr_in	_socketAddress;
		int 				_clientSocket;
		std::string			_clientIp;
		std::string			_request;

		// Exceptions

	class InitSocketException : public std::exception
	{
		public:
			virtual const char* what() const throw();
	};

	class IoException : public std::exception
	{
		public:
			virtual const char* what() const throw();
	};
};
#endif
