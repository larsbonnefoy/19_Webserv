/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:07:26 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/07 15:39:09 by hdelmas          ###   ########.fr       */
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
	ws_log(this->_pathtype);
	if (index == ""  || this->_pathtype == FILETYPE || this->_path == BADPATH) 
		return ;
	if ((this->_uri != location.getPath() && this->_uri != location.getPath() + "/") && this->_pathtype == DIRTYPE)
	{
		this->_path = BADINDEX;
		this->_pathtype = BADTYPE;
		return ;
	}
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

void HttpResponse::_requestError(Server server, int code)
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

void HttpResponse::_requestSuccess(int code)
{
	ws_log("REQUEST SUCCESS");
	this->_statusCode = code;
	return ;
}
 
void	HttpResponse::_setRedir(Location location)
{
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
}

void HttpResponse::_GETRequest(Location location, Server server)
{
	(void)location;
	ws_log("GET");
	if (this->_path == BADPATH)
		return (_requestError(server, 404));
	if (this->_path == BADREDIR || this->_pathtype == BADTYPE || this->_path == BADINDEX || this->_autoindex == 0)
		return(_requestError(server, 403));		
	if (this->_pathtype == FILETYPE || this->_autoindex == 1 || this->_path != "")
		return(_requestSuccess(this->_statusCode));
}

void	HttpResponse::_DELETERequest(Server server)
{
	if (this->_path == BADPATH)
		return (_requestError(server, 404));
	if (std::remove(this->_path.c_str()) != 0)
		return(_requestError(server, 500)); //Accept);		
	return (_requestSuccess(204));
}

void	HttpResponse::_createResponse(void)
{

	this->setStartLine(_makeStartLine());
	ws_log(this->_startLine);
	switch (this->_statusCode)
	{
		case 200 :
 			if (this->_autoindex == 1)
    		    this->_handleAutoIndex(this->_path);
            else
    	    	this->_handleURL(this->_path);
			break;
		
		case 301 :
	  	    this->_handleRedirection();
			break ;

		case 204 : 
			break ;		
		
		default:
    	    this->_handleURL(this->_path);
			break;
	}
}

bool HttpResponse::_isCgi(std::string path, Location loc) {
    ws_log("CGI CHECK");
    ws_log(path);
    ws_log(loc.getCGIPath());
    ws_log("-----");
    if (path.find(loc.getCGIPath()) != std::string::npos) {
        ws_log("true");
        return (true);
    }
    ws_log("false");
    return (false);
}

HttpResponse::HttpResponse(Server &serv, HttpRequest &request)
{
	ws_log("Response constructor");
	Location	location = getLocation(serv, request); //check for server name
	
	int methode = getMethode(location, request);
	
	this->_setPath(location, request, methode);
	//this->_body = parse(request.getBody()); ??

    //cgi response can have
    //  startline with response code 
    //  Http Headers (file type,...)
    //  Body
    if (_isCgi(this->_path, location)) {
        cgi res(request, this->_path);
        //Interal error if something wrong happens with CGI;
        try { 
            std::string response = res.run();
            std::cout << response << std::endl;
        }
        catch (std::exception &e) {
            ws_log(e.what());
        }
        this->_cgi = true;
    }
    else {
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
                _requestSuccess(204); //NO CGI
                break ;
            
            case DELETE:
                this->_DELETERequest(serv);
                break;
                
            default:
                _requestError(serv, 400);
        }
    }
	this->_createResponse();
}
