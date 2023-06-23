/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mux.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 11:06:39 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/23 11:12:33 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUX_HPP
# define MUX_HPP

# include <iostream>
# include <string>
# include <string>
# include <cstring>
# include <iostream>

# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>

# include "../log/Log.hpp"

class Mux
{
	public:
		// Constructors
		Mux();
		Mux(const Mux &copy);
		
		// Destructor
		~Mux();
		
		// Operators
		Mux & operator=(const Mux &assign);
		
	private:
		
};

#endif