#ifndef CGI_HPP
#define CGI_HPP

#include "HttpRequest.hpp"
#include "Location.hpp"
#include <exception>
#include<map>
#include<unistd.h>
#include <vector>

class Cgi {
    private:
        size_t                              _method;
        std::string                         _data;
        std::vector<std::string>            _av;
        std::map<std::string, std::string>  _env;
        std::string                         _pathInfo;

        char **_convToTab(std::map<std::string, std::string> env);
        char **_convToTab(std::vector<std::string> av);
        std::string _readFromPipe(int pipeRead);

    public:
        Cgi(void);
        Cgi(HttpRequest &request, std::string path, std::string uploadDir);
        Cgi(const Cgi &other);
        ~Cgi(void);

        Cgi &operator=(const Cgi &other);
        std::string run(void);

        class InternalError : public std::exception {
            public:
                const char* what(void) const throw();
        };
};


#endif
