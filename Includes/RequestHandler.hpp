/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:07:29 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/07 15:39:11 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <set>
# include <cstring>
# include <cstdio>
# include <exception>

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <poll.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "Log.hpp"
# include "Socket.hpp"
# include "Config.hpp"
# include "ConfigParser.hpp"
# include "Server.hpp"
# include "Http.hpp"
# include "HttpRequest.hpp"
#include "../Includes/ConfigParser.hpp"
# include "Cgi.hpp"

# define BADPATH "badPath"
# define BADINDEX "badIndex"
# define BADREDIR "badRedir"
# define BADTYPE -1
# define FILETYPE 1
# define DIRTYPE 0

// void	requestHandler(Server server, HttpRequest request);

#endif
