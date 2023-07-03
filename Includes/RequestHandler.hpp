/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:07:29 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/02 03:30:25 by hdelmas          ###   ########.fr       */
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

# define BADPATH "badPath"
# define BADINDEX "badIndex"
# define BADREDIR "badRedir"

std::pair<std::string, size_t> requestHandler(Server server, HttpRequest request);

#endif
