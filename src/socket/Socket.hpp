/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 11:19:01 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/19 11:19:17 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>

class Socket
{
	public:
		// Constructors
		Socket();
		Socket(const Socket &copy);
		Socket(const std::string &host, const std::string &port);
		// Destructor
		~Socket();
		
		// Operators
		Socket & operator=(const Socket &assign);
		
	private:
		
};

#endif