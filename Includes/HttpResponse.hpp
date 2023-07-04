#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "../Includes/Http.hpp"
#include "../Includes/HttpRequest.hpp"

#include <dirent.h>
#include <cstddef>



class HttpResponse : public Http {
    private:
        size_t      _statusCode;
        std::string _statusPhrase;

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
