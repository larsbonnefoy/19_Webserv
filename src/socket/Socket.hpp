/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 11:19:01 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/19 18:10:45by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string>
# include <cstring>
# include <iostream>
# include <poll.h>
# include <fcntl.h>
// # include <cstdint>
# define PORT 8080
# define DEFAULTPORT 8080
# define BUFF_SIZE 1024

class Socket
{
	public:
		// Constructors
		Socket();
		Socket(const Socket &copy);
		Socket(const int port);
		// Destructor
		~Socket();
		
		// Operators
		Socket & operator=(const Socket &assign);
		
		// Getter
		const char	*getRequest(void) const;

		// MemberFunctions
		const char	*receiveRequest(void);
		void		sendResponse(const std::string response);		

	private:
		int 				serverSocket;
		int					socketAddressLen;
		struct sockaddr_in	socketAddress;
		int 				requestSocket;
		char				request[BUFF_SIZE];
		char				badbit;
		
		
};

#endif