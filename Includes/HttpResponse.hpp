#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <set>
# include <cstring>
# include <exception>
#include <dirent.h>
#include <cstddef>


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
#include "../Includes/Http.hpp"
#include "../Includes/HttpRequest.hpp"

# define BADPATH "badPath"
# define BADINDEX "badIndex"
# define BADREDIR "badRedir"

class HttpResponse : public Http {
    private:

		// Private Varaible
        size_t      _statusCode;
        std::string _statusPhrase;
		std::string _path;
		int			_pathtype;
		std::string	_index;
		size_t		_autoindex;

		// Private methode
        std::string _makeStartLine(void);
        void        _handleURL(std::string &url);
        std::string _getFileExtension(std::string &url);
        size_t      _getFileSize(std::string &url);
        std::string _valToString(size_t num);
        std::string _fileToString(std::string &url);
        std::string _getMIMEType(std::string &url);
        bool        _isDirectory(const std::string& path);
        void        _handleAutoIndex(const std::string &path);
        std::string _createHTMLAutoindex(const std::string &url);

		// Request Handling 
		void	_GETRequest(Location location, Server server);
		void	_setPath(Location location, HttpRequest request, int methode);
		void	_setIndex(Location location);
		void requestError(Server server, int code);
		void requestSuccess(int code);
    
	public:
        HttpResponse(void);
        HttpResponse(std::string url, size_t code);
        HttpResponse(Server &serv, HttpRequest &request);
        HttpResponse(const HttpResponse &other);
        ~HttpResponse(void);
        HttpResponse &operator=(const HttpResponse &other);

        void setStatusCode(size_t code);
        void setStatusPhrase(std::string);

        size_t      getStatusCode(void) const;
        std::string getStatusPhrase(void) const;
        
        std::string convertToStr(void);


};

std::ostream &operator<<(std::ostream &out, HttpResponse &httpRes);
#endif
