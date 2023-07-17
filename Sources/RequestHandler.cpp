/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:07:26 by hdelmas           #+#    #+#             */
/*   Updated: 2023/07/17 11:00:29 by lbonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../Includes/HttpResponse.hpp"
#include "../Includes/RequestHandler.hpp"
#include <sstream>

void HttpResponse::_isCgi(void) {
	if (this->_uri.rfind(".cgi") != this->_uri.size() - 4)
			this->_cgi = false;
	else
			this->_cgi = true;
	return ;
}

static int	getType(std::string path)
{
	struct stat statbuf;
	
	if (stat(path.c_str(), &statbuf) != 0)
		return (BADTYPE);
	if ((statbuf.st_mode & S_IFMT) == S_IFREG)
	{
		return (FILETYPE);	
	}
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
		return (DIRTYPE);
	}
	return (-1);
}

static Location	getLocation(Server &server, HttpRequest &request)
{
	Location	location;
	size_t		lastSize = 0;
	size_t		findRet = 0;
	
	for (size_t	i = 0 ; i < server.getLocations().size(); ++i)
	{
		
		std::string locationPath = server.getLocations()[i].getPath();

		if (*locationPath.rbegin() == '/') 
			locationPath.erase(locationPath.size() - 1);
		findRet = request.getUri().find(locationPath,  0);
		if (findRet == 0 && server.getLocations()[i].getPath().size() >= lastSize)
		{
			location = server.getLocations()[i];
			lastSize = server.getLocations()[i].getPath().size();
		}
	}
	return (location);
}

void	HttpResponse::_setMethode(Location &location, HttpRequest &request)
{
	int methode = matchMethod(request.getMethode());
	this->_methode = -1;
	switch (methode)
	{
		case GET :
			if (location.getGetVal())
				this->_methode = GET;
			break ;
		
		case POST : 
			if (location.getPostVal())
			{
				this->_methode = POST;
			}
			break ;
	
		case DELETE :
			if (location.getDelVal())
				this->_methode = DELETE;
			break ;
		default :
			this->_methode = -1;
	}
}

static std::string	getRoot(Server &server)
{
	std::string root;
	
	root = server.getServerRoot();
	if (*root.rbegin() == '/')
		root.erase(root.size() - 1);
	return (root);
}

void	HttpResponse::_setPath(Location &location, HttpRequest &request, int methode)
{
	std::string path;
	std::string locationRoot;
	struct stat statbuf;

	locationRoot =  location.getRoot();
	if (*locationRoot.rbegin() == '/')
		locationRoot.erase(locationRoot.size() - 1);
	this->_uri = request.getUri();
	this->_isCgi();
	if (this->_cgi)
	{
		std::string locationCgi;
		locationCgi =  location.getCGIPath();
		if (*locationCgi.rbegin() == '/')
			locationCgi.erase(locationCgi.size() - 1);
		this->_path = locationRoot + "/"  + locationCgi + this->_uri;
	}
	else
		this->_path = locationRoot + this->_uri;
	this->_statusCode = 200;
	int ret = stat(this->_path.c_str(), &statbuf);
	if (ret < 0)
	{
		this->_path = BADPATH;
		this->_pathtype = BADTYPE;
		return ;
	}
	this->_pathtype = getType(this->_path);
	if (this->_pathtype == BADTYPE)
	{
		this->_path = BADPATH;
		return ;
	}
	if (this->_cgi)
	{
		if (!((statbuf.st_mode & S_IXUSR )|| (statbuf.st_mode & S_IXGRP) || (statbuf.st_mode & S_IXOTH)))
		{
			this->_path = BADPATH;
			this->_pathtype = BADTYPE;
			return ;
		}
		return ;
	}
	switch (methode)
	{
		case GET :
		{
			if (!((statbuf.st_mode & S_IRUSR )|| (statbuf.st_mode & S_IRGRP) || (statbuf.st_mode & S_IROTH)))
			{
				this->_path = BADPATH;
				this->_pathtype = BADTYPE;
				return ;
			}
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

void	HttpResponse::_setIndex(Location &location)
{
	struct stat statbuf;
	
	std::string index =  location.getIndex();

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

void HttpResponse::_requestError(Server &server, int code)
{
	this->_path = getRoot(server);
	this->_statusCode = code;
	std::map<size_t, std::string> tmp = server.getErrors();
	std::map<size_t, std::string>::iterator it = tmp.find(code);
	if (it != server.getErrors().end())
		this->_path += "/" + server.getErrors()[code];
	return ;
}

void HttpResponse::_requestSuccess(int code)
{
	this->_statusCode = code;
	return ;
}
 
void	HttpResponse::_setRedir(Location &location)
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

void HttpResponse::_GETRequest(Server &server)
{
	if (this->_path == BADPATH)
		return (_requestError(server, 404));
	if (this->_path == BADREDIR || this->_pathtype == BADTYPE || this->_path == BADINDEX || this->_autoindex == 0)
		return(_requestError(server, 403));		
	if (this->_pathtype == FILETYPE || this->_autoindex == 1 || this->_path != "")
		return(_requestSuccess(this->_statusCode));
}

void	HttpResponse::_DELETERequest(Server &server)
{
	if (this->_path == BADPATH)
		return (_requestError(server, 404));
	if (std::remove(this->_path.c_str()) != 0)
		return(_requestError(server, 500)); //Accept);		
	return (_requestSuccess(204));
}

void HttpResponse::_handleSuccessRequest(void) {

    if (this->_autoindex == 1) {
        this->_handleAutoIndex(this->_path);
    }
    else if (this->_cgi == true) {
        return ;
    }
    else {
        this->_handleURL(this->_path);
    }
}

void	HttpResponse::_createResponse(void)
{
    this->_statusPhrase = StaticInit::STATUS_CODE_PHRASE[_statusCode];
	this->setStartLine(_makeStartLine());

	switch (this->_statusCode)
	{
		case 200 :
            this->_handleSuccessRequest();
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

void HttpResponse::_handleCgiResponse(std::string response) {
    std::stringstream ss(response);
    std::string line;
    bool body = false;

    while (std::getline(ss, line)) {
        if (line == "") {
            body = true;
        }
        if (body) {
            this->_body += (line + "\n");
        }
        else {
            addToHeaderField(line);
        }
    }
    addToHeaderField("Content-Length", valToString(this->_body.size()));
}


HttpResponse::HttpResponse(Server &serv, HttpRequest &request)
{
    if (serv.getName() != "" && serv.getName() != request.getName() && !((serv.getName() == "localhost" && request.getName() == "127.0.0.1") || (serv.getName() == "127.0.0.1" && request.getName() == "localhost" ))) {
		this->_requestError(serv, 403);
    }
    else if (request.getBody().size() > serv.getMaxBodySize()) {
		this->_requestError(serv, 413); //or 416 ??
    }
    else {
        Location    location = getLocation(serv, request); //check for server name
	
		this->_setMethode(location, request);
		this->_setPath(location, request, this->_methode);
        std::string uploadDir = location.getUploadDir();
		
		//cgi response can have
		//  startline with response code 
		//  Http Headers (file type,...)
		//  Body
        switch (this->_methode)
        {
            case GET:
                if (this->_cgi)
                {
                    try {
                        Cgi res(request, this->_path, uploadDir);
                        std::string response = res.run();
                        _requestSuccess(200);
                        _handleCgiResponse(response);
                    }
                    catch (std::exception &e) {
                        _requestError(serv, 500);
                    }
                    break ;
                }
                this->_setIndex(location);	
                this->_setRedir(location);	
                if (this->_pathtype == DIRTYPE) {
                    this->_autoindex = location.getAutoIndex();
                }
                else {
                    this->_autoindex = UNDEFINED;
                }
                this->_GETRequest(serv);
                break;
            
            case POST:
                if (this->_cgi)
                {
                    try { 
                        Cgi res(request, this->_path, uploadDir);
                        std::string response = res.run();
                        _requestSuccess(200);
                        _handleCgiResponse(response);
                    }
                    catch (std::exception &e) {
                        _requestError(serv, 500);
                    }
                    break ;
                }
                _requestSuccess(204); 
                break ;
            
            case DELETE:
                this->_DELETERequest(serv);
                break;
                
            default:
                _requestError(serv, 405);
        }
    }
    this->_createResponse();
	ws_log("\n-----Response-----");
	ws_log(*this);
}

/*-----------------------------EXCEPTION--------------------------------------*/

const char* InternalError::what(void) const throw() {
        return("[RequestHandler::InternalError] : Interal Server Error : Cgi failed.");
}
