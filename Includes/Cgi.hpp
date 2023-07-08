#ifndef CGI_HPP
#define CGI_HPP

#include "HttpRequest.hpp"
#include "Location.hpp"
#include<map>
#include<unistd.h>
#include <vector>

class cgi {
    private:
        size_t                              _method;
        std::string                         _data;
        std::vector<std::string>            _av;
        std::map<std::string, std::string>  _env;
        std::string                         _pathInfo;

        char **_convToTab(std::map<std::string, std::string> env);
        char **_convToTab(std::vector<std::string> av);
    public:
        cgi(void);
        cgi(HttpRequest &request, std::string path);
        cgi(const cgi &other);
        ~cgi(void);
        cgi &operator=(const cgi &other);
        std::string run(void);
};


#endif
