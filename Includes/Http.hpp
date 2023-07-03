/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 11:11:15 by hdelmas           #+#    #+#             */
/*   Updated: 2023/06/29 11:42:43 by hdelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <string>

# include <iostream>
# include <string>
# include <sstream>
# include <iostream>
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
# include <fcntl.h>

# include "Log.hpp"
# include "Socket.hpp"
# include "Config.hpp"
# include "ConfigParser.hpp"
# include "Server.hpp"

struct StaticInit {

    static std::map<size_t, std::string>        STATUS_CODE_PHRASE;

    static std::map<std::string, std::string>   MIME_TYPES;

    static std::string                          DEF_ERROR;
    
    static void initStatic(void) {
        
        STATUS_CODE_PHRASE[100] = "Continue";
        STATUS_CODE_PHRASE[101] = "Switching Protocols";
        STATUS_CODE_PHRASE[200] = "OK";
        STATUS_CODE_PHRASE[201] = "Created";
        STATUS_CODE_PHRASE[202] = "Accepted";
        STATUS_CODE_PHRASE[203] = "Non-Authoritative Information";
        STATUS_CODE_PHRASE[204] = "No Content";
        STATUS_CODE_PHRASE[205] = "Reset Content";
        STATUS_CODE_PHRASE[206] = "Partial Content";
        STATUS_CODE_PHRASE[300] = "Multiple Choices";
        STATUS_CODE_PHRASE[301] = "Moved Permanently"; 
        STATUS_CODE_PHRASE[302] = "Found";
        STATUS_CODE_PHRASE[303] = "See Other";
        STATUS_CODE_PHRASE[304] = "Not Modified";
        STATUS_CODE_PHRASE[305] = "Use Proxy";
        STATUS_CODE_PHRASE[307] = "Temporary Redirect";
        STATUS_CODE_PHRASE[400] = "Bad Request";
        STATUS_CODE_PHRASE[401] = "Unauthorized";
        STATUS_CODE_PHRASE[402] = "Payment Required";
        STATUS_CODE_PHRASE[403] = "Forbidden";
        STATUS_CODE_PHRASE[404] = "Not Found";
        STATUS_CODE_PHRASE[405] = "Method Not Allowed";
        STATUS_CODE_PHRASE[406] = "Not Acceptable";
        STATUS_CODE_PHRASE[407] = "Proxy Authentication Required";
        STATUS_CODE_PHRASE[408] = "Request Timeout";
        STATUS_CODE_PHRASE[409] = "Conflict";
        STATUS_CODE_PHRASE[410] = "Gone";
        STATUS_CODE_PHRASE[411] = "Length Required";
        STATUS_CODE_PHRASE[412] = "Precondition Failed";
        STATUS_CODE_PHRASE[413] = "Request Entity Too Large";
        STATUS_CODE_PHRASE[414] = "Request-URI Too Long";
        STATUS_CODE_PHRASE[415] = "Unsupported Media Type";
        STATUS_CODE_PHRASE[416] = "Requested Range Not Satisfiable";
        STATUS_CODE_PHRASE[417] = "Expectation Failed";
        STATUS_CODE_PHRASE[500] = "Internal Server Error";
        STATUS_CODE_PHRASE[501] = "Not Implemented";
        STATUS_CODE_PHRASE[502] = "Bad Gateway";
        STATUS_CODE_PHRASE[503] = "Service Unavailable";
        STATUS_CODE_PHRASE[504] = "Gateway Timeout";
        STATUS_CODE_PHRASE[505] = "HTTP Version Not Supported";

        MIME_TYPES["html"] = "text/html";
        MIME_TYPES["txt"] = "text/plain";
        MIME_TYPES["jpg"] = "image/jpeg";
        MIME_TYPES["jpeg"] = "image/jpeg";
        MIME_TYPES["gif"] = "image/gif";

        DEF_ERROR = "<!DOCTYPE html>\r\n"
                        "<html>\r\n"
                        "<head>\r\n"
                        "  <title>Default File</title>\r\n"
                        "</head>\r\n"
                        "<body>\r\n"
                        "   <center>"
                        "       <h1>Default Error File</h1>\r\n"
                        "   </center>"
                        "   <hr>"
                        "   <center>"
                        "       <h1>webserv/1.25.1</h1>\r\n"
                        "   </center>"
                        "</body>\r\n"
                        "</html>";
    }
};

class Http
{
	public:
		// Constructors
		Http(void);
		Http(const Http &copy);
		 
		// Destructor
		virtual	~Http(void) = 0;
		
		// Operators
		Http & operator=(const Http &assign);

		// Setters
		void	setStartLine(std::string startLine);
		void	setBody(std::string body);
		void	setHeaderField(std::map<std::string, std::string> headerField);
		void	addToHeaderField(std::string headerToAdd);
        void	addToHeaderField(std::string header, std::string value);

		// Getters
		std::string							getStartLine(void) const;		
		std::string							getBody(void) const;
		std::map<std::string, std::string>	getHeaderField(void) const;


        std::string headerToStr(void);


	protected:

		std::string							_startLine;
		std::string 						_body;
		std::map<std::string, std::string>	_headerField;
};

#endif
