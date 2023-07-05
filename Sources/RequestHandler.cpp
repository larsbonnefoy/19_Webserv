/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:07:26 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/05 16:47:47 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../Includes/HttpResponse.hpp"
#include "../Includes/RequestHandler.hpp"

// Change most error code to 403

static int	getType(std::string path)
{
	struct stat statbuf;
	
	if (stat(path.c_str(), &statbuf) != 0)
		return (BADTYPE);
	if ((statbuf.st_mode & S_IFMT) == S_IFREG)
	{
		ws_log("FILE");
		return (FILETYPE);	
	}
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		ws_log("DIRECTORY");
		return (DIRTYPE);
	}
	return (-1);
}

static Location	getLocation(Server server, HttpRequest request)
{
	Location	location;
	size_t		lastSize = 0;
	size_t		findRet = 0;
	
	for (size_t	i = 0 ; i < server.getLocations().size(); ++i)
	{
		
		std::string locationPath = server.getLocations()[i].getPath();
		if (*locationPath.rbegin() == '/') 
			locationPath.erase(locationPath.size() - 1);
		ws_log(locationPath);
		ws_log(request.getUri());
		findRet = request.getUri().find(locationPath,  0);
		ws_log(findRet);
		if (findRet == 0 && server.getLocations()[i].getPath().size() >= lastSize)
		{
			location = server.getLocations()[i];
			lastSize = server.getLocations()[i].getPath().size();
		}
	}
	ws_log(location.getPath());
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
	
	root = server.getServerRoot();
	if (*root.rbegin() == '/')
		root.erase(root.size() - 1);
	return (root);
}

void	HttpResponse::_setPath(Location location, HttpRequest request, int methode)
{
	std::string path;
	std::string locationRoot;
	struct stat statbuf;

	locationRoot =  location.getRoot();
	if (*locationRoot.rbegin() == '/')
		locationRoot.erase(locationRoot.size() - 1);

	this->_uri = request.getUri();
	this->_path = locationRoot + this->_uri;
	this->_statusCode = 200;
	ws_log("path");
	ws_log(this->_path);
	int ret = stat(this->_path.c_str(), &statbuf);
	if (ret < 0
		|| !((statbuf.st_mode & S_IFMT) == S_IFDIR || (statbuf.st_mode & S_IFMT) == S_IFREG))
	{
		this->_path = BADPATH;
		return ;
	}
	this->_pathtype = getType(this->_path);
	
	switch (methode)
	{
		case GET :
		{
			if (!((statbuf.st_mode & S_IRUSR )|| (statbuf.st_mode & S_IRGRP) || (statbuf.st_mode & S_IROTH)))
			{
				ws_log("READ NOT OK");
				this->_path = BADPATH;
				this->_pathtype = BADTYPE;
				return ;
			}
			ws_log("READ OK");
			break;
		}
		case POST :
			if (!(statbuf.st_mode & S_IWUSR || statbuf.st_mode & S_IWGRP || statbuf.st_mode & S_IWOTH)) // X ??
			{
				this->_path = BADPATH;
				this->_pathtype = BADTYPE;
				return ;
			}
			break;
		case DELETE :
			if (!(statbuf.st_mode & S_IWUSR || statbuf.st_mode & S_IWGRP || statbuf.st_mode & S_IWOTH))
			{
				this->_path = BADPATH;
				this->_pathtype = BADTYPE;
				return ;
			}
			break;
		default:
		{
			this->_path = BADPATH;
			this->_pathtype = BADTYPE;
			return ;
		}
	}
}


void	HttpResponse::_setIndex(Location location)
{
	struct stat statbuf;
	
	std::string index =  location.getIndex();

	ws_log("set index");
	ws_log(this->_uri);
	ws_log(location.getPath());
	if ((this->_uri != location.getPath() && this->_uri != location.getPath() + "/") && this->_pathtype == DIRTYPE)
	{
		this->_path = BADINDEX;
		this->_pathtype = BADTYPE;
		return ;
	}
	
	if (index == ""  || this->_pathtype == FILETYPE || this->_path == BADPATH) 
		return ;
	
	if (*this->_path.rbegin() == '/')
		this->_path.erase(this->_path.size() - 1);
	this->_path += "/" + index;
	ws_log("index");
	ws_log(this->_path);
	if (stat(this->_path.c_str(), &statbuf) != 0
		|| !((statbuf.st_mode & S_IFMT) == S_IFDIR || (statbuf.st_mode & S_IFMT) == S_IFREG)
		|| this->_path.rfind(".html") != this->_path.size() - 5)
	{
		this->_path = BADINDEX;
		this->_pathtype = BADTYPE;
		return ;
	}
	this->_pathtype = FILETYPE;
}

void HttpResponse::requestError(Server server, int code)
{
	ws_log("REQUEST ERROR");
	this->_path = getRoot(server);
	ws_log(this->_path);
	this->_statusCode = code;
	std::map<size_t, std::string> tmp = server.getErrors();
	std::map<size_t, std::string>::iterator it = tmp.find(code);
	if (it != server.getErrors().end())
		this->_path += "/" + server.getErrors()[code];
	return ;
}

void HttpResponse::requestSuccess(int code)
{
	ws_log("REQUEST SUCCESS");
	this->_statusCode = code;
	return ;
}
 
void	HttpResponse::_setRedir(Location location)
{
//	if (/*this->_pathtype == FILETYPE ||*/ this->_path == BADPATH) 
//		return ;
	ws_log(location.getRedirect().first);
	ws_log(location.getPath());
	if (location.getRedirect().first == 0)
		return ;
	this->_statusCode = location.getRedirect().first;
	std::string	redir = location.getRedirect().second;
	if (*redir.rbegin() == '/')
		redir.erase(redir.size() - 1);
	size_t locationSize = location.getPath().size();
	std::string query = this->_uri.substr(locationSize, this->_uri.size() - locationSize);
	if (*query.begin() != '/') 
		query = "/" + query;
	this->_path = redir +  query;
	this->_pathtype = FILETYPE;
	ws_log("redir");
	ws_log(this->_path);
}

void HttpResponse::_GETRequest(Location location, Server server)
{
	(void)location;
	ws_log("GET");
	if (this->_path == BADPATH)
		return (requestError(server, 404));
	if (this->_path == BADREDIR || this->_pathtype == BADTYPE || this->_path == BADINDEX || this->_autoindex == 0)
		return(requestError(server, 403));		
	if (this->_pathtype == FILETYPE || this->_autoindex == 1 || this->_path != "")
		return(requestSuccess(this->_statusCode));
}


HttpResponse::HttpResponse(Server &serv, HttpRequest &request)
{
	ws_log("Response constructor");
	// next to change: redir then _getrequest
	Location	location = getLocation(serv, request); //check for server name
	
	int methode = getMethode(location, request);
	
	this->_setPath(location, request, methode);
	//body
	
	switch (methode)
	{
		case GET:
			ws_log(this->_path);
			this->_setIndex(location);	
			ws_log(this->_path);
			this->_setRedir(location);	
			this->_autoindex = location.getAutoIndex();
			this->_GETRequest(location, serv);
			break;
		
		case POST:
			break ;
			// return (requestSuccess(getPath(location, request, GET), 204));
		
		case DELETE:
			break;
			
		default:
			requestError(serv, 400);
	}
	this->setStartLine(_makeStartLine());

    if (this->_statusCode == 200 && this->_autoindex == 1) {
        this->_handleAutoIndex(this->_path);
    }
	else if (this->_statusCode == 301)
	{
		ws_log(301);
//		this->_path = "URL=" + this->_path;
		ws_log(this->_path);
		this->addToHeaderField("Content-Type","Moved Permanently");
		this->addToHeaderField("Location", this->_path);
		this->_body = "";
//   		this->_body = "<!DOCTYPE html>\r\n"
//						"<head>\r\n"
//						"</head>\r\n"
//						"<body>\r\n"
//						"<meta http-equiv=\"Refresh\" content=\"0;\r\n";
//		this->_body += 	 this->_path + '"';
//		this->_body += 	" />\r\n</body>\r\n"
//						"</html>\r\n";
		this->addToHeaderField("Content-Length", _valToString(this->_body.size()));
//		ws_log(this->_body);

	}
    else {
		ws_log(this->_path);
        this->_handleURL(this->_path);
    }
}
