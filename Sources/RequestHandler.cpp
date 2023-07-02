/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:07:26 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/02 18:03:30 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/RequestHandler.hpp"
// Change most error code to 403
static Location	getLocation(Server server, HttpRequest request)
{
	Location	location;
	size_t		lastSize = 0;
	
	for (size_t	i = 0 ; i < server.getLocations().size(); ++i)
	{
		server.getLocations()[i];
		size_t findRet = server.getLocations()[i].getPath().find(request.getUri().c_str(), 0, server.getLocations()[i].getPath().size());
		if (findRet == 0 && server.getLocations()[i].getPath().size() >= lastSize)
			location = server.getLocations()[i];
		lastSize = server.getLocations()[i].getPath().size();
	}
	return (location);
}

static int	getMethode(Location location, HttpRequest request)
{
	int methode = matchMethod(request.getMethode());
	
	switch (methode)
	{
		case GET :
			if (location.getGetVal())
				return (GET);
			break ;
		
		case POST : 
			if (location.getPostVal())
				return (POST);
			break ;
	
		case DELETE :
			if (location.getDelVal())
				return (DELETE);
			break ;
	}
	return (-1);
}

static std::string	getRoot(Server server)
{
	std::string root;
	
	root = server.root;
	if (*root.rbegin() == '/')
		root.erase(root.size() - 1);
	return (root);
}

static std::string	getPath(Location location, HttpRequest request, int methode)
{
	std::string path;
	std::string locationRoot;
	struct stat statbuf;

	locationRoot = location.getRoot();
	if (*locationRoot.rbegin() == '/')
		locationRoot.erase(locationRoot.size() - 1);
	path = locationRoot + request.getUri();
	ws_log(path);
	int ret = stat(path.c_str(), &statbuf);
	if (ret < 0)
	{
		ws_log("stat not ok");
		ws_log(ret);
		ws_log(errno);
		ws_log(EACCES);
		return (BADPATH);
	}
	ws_log("stat ok");
	if (!((statbuf.st_mode & S_IFMT) == S_IFDIR || (statbuf.st_mode & S_IFMT) == S_IFREG))
		return (BADPATH);
	

	switch (methode)
	{
		case GET :
		{
			if (!((statbuf.st_mode & S_IRUSR )|| (statbuf.st_mode & S_IRGRP) || (statbuf.st_mode & S_IROTH)))
			{
				ws_log("READ NOT OK");
				return (BADPATH);
			}
			ws_log("READ OK");
			break;
		}
		case POST :
			if (!(statbuf.st_mode & S_IWUSR || statbuf.st_mode & S_IWGRP || statbuf.st_mode & S_IWOTH)) // X ??
				return (BADPATH);
			break;
		case DELETE :
			if (!(statbuf.st_mode & S_IWUSR || statbuf.st_mode & S_IWGRP || statbuf.st_mode & S_IWOTH))
				return (BADPATH);
			break;
		default:
			return (BADPATH);
	}
	return (path);
}

static int	getType(std::string path)
{
	struct stat statbuf;
	
	if (stat(path.c_str(), &statbuf) != 0)
		return (-1);
	if ((statbuf.st_mode & S_IFMT) == S_IFREG)
	{
		ws_log("FILE");
		return (1);	
	}
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		ws_log("DIRECTORY");
		return (0);
	}
	return (-1);
}

static std::string	getIndex(Location location, std::string path)
{
	struct stat statbuf;

	if (location.getIndex() == "")
		return ("");
	
	if (*path.rbegin() == '/')
		path.erase(path.size() - 1);
	std::string index = path + "/" + location.getIndex();
	ws_log("index");
	ws_log(index);
	if (stat(index.c_str(), &statbuf) != 0)
		return (BADINDEX);
	ws_log("hmmm");
	if (!((statbuf.st_mode & S_IFMT) == S_IFDIR || (statbuf.st_mode & S_IFMT) == S_IFREG))
		return (BADINDEX);
	ws_log("hmmm");
	if (index.rfind(".html") != index.size() - 5)
		return (BADINDEX);
	return (index);
}

static std::pair<std::string, size_t> requestError(Server server, int code)
{
	ws_log("REQUEST ERROR");
	std::pair<std::string, size_t> res;
	res.first = getRoot(server);
	res.second = code;
	std::map<size_t, std::string>::iterator it = server.getErrors().find(code);
	if (it != server.getErrors().end())
		res.first += "/" + server.getErrors()[code];
	return (res);
}

static std::pair<std::string, size_t> requestSuccess(std::string path, int code)
{
	ws_log("REQUEST SUCCESS");
	std::pair<std::string, size_t> res;
	res.first = path;
	res.second = code;
	return (res);
}
 


static std::string getRedir(Location location)
{
	struct stat statbuf;
	
	std::string redir = location.getRedirect().second;//????
	ws_log("redir");
	ws_log(redir);
	if (stat(redir.c_str(), &statbuf) != 0)
		return (BADREDIR);
	if (!((statbuf.st_mode & S_IFMT) == S_IFDIR || (statbuf.st_mode & S_IFMT) == S_IFREG))
		return (BADREDIR);
	return (redir);
}

std::pair<std::string, size_t> GETRequest(Location location, Server server, HttpRequest request)
{
	ws_log("GET");
	std::string	path = getPath(location, request, GET);
	if (path == BADPATH)
		return (requestError(server, 404));	
				
	int	type = getType(path);
	if (type == -1)
		return(requestError(server, 402));
	if (type == 1)
		return(requestSuccess(path, 200));
		
	std::string index = getIndex(location, path);
	if (index == "badIndex")
		return (requestError(server, 405));
	if (index != "")
		return(requestSuccess(index, 200));
		
	ws_log("autoIndex");
	int	autoindex = location.getAutoIndex();
	if (autoindex == 0)
		return (requestError(server, 406));
	if (autoindex == 1)
		return(requestSuccess(path, 200));// ??? no workey
		
	std::string	redir = getRedir(location);
	if (redir == "badRedir")	
		return (requestError(server, 407));
	return(requestSuccess(redir, 301));
}

std::pair<std::string, size_t> requestHandler(Server server, HttpRequest request)
{
	Location	location = getLocation(server, request);
	int methode = getMethode(location, request);
	if (methode == -1)
		return (requestError(server, 400));
	
	switch (methode)
	{
		case GET:
			return (GETRequest(location, server, request));
		
		case POST:
			break;
		
		case DELETE:
			break;
			
		default:
			break;
	}
	std::pair<std::string, size_t> res;
	res.first = "";
	res. second = 1;
	return res;
}