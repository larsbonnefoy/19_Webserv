/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:07:26 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/01 01:17:30 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/RequestHandler.hpp"

static int	getMethode(Server server, HttpRequest request)
{
	size_t retVal = -1;

    for (size_t methodID = 0; methodID < 3; methodID++) {
        if (request.getMethode() == server.getLocations()[methodID]) {
            retVal = methodID;
        }
    } 
    return (retVal);
	
}

static std::string	getPath(Server server, HttpRequest request)
{
	
}

static int	getType(Server server, HttpRequest request)
{
	
}

static std::pair<std::string, size_t> requestError(Server server, HttpRequest request)
{
	
}

static std::pair<std::string, size_t> requestSuccess(std::string path, int code)
{
	std::pair<std::string, size_t> res;
	res.first = path;
	res.second = 200;
	return (res);
}

std::pair<std::string, size_t> requestHandler(Server server, HttpRequest request)
{
	int methode = getMethode(server, request);
	if (methode == -1)
		return (requestError(server, request, 403));
	
	switch (methode)
	{
		case GET:
		{
			std::string	path = getPath(server, request);
			if (path == "badUrl")
				return (requestError(server, request, 404));
			
			int	type = getType(path);
			if (type)
				return(requestSuccess(path, 200));
			
			std::string index = getIndex(server, request); //autoIndex and index ??? 
			if (index == "badIndex")
				return (requestError(server, request, 403));
			if (index != "autoIndex")
				return(requestSuccess(path + "/" + index, 200)); //??
			
			int	autoindex = getAutoIndex(server, request);
			if (autoindex == 0)
				return (requestError(server, request, 403));
			if (autoindex == 1)
				return(requestSuccess(path, 200));
			
			std::string	redir = getRedir(server, request);
			if (redir == "badRedir")	
				return (requestError(server, request, 403));
			return(requestSuccess(redir, 301));
		}
		
		case POST:
			break;
		
		case DELETE:
			break;
			
		default:
			break;
	}
	
}