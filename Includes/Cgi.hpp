#ifndef CGI_HPP
#define CGI_HPP

#include<map>
#include<unistd.h>

class CGI {
    private:
        std::map<std::string, std::string> _env;

        void _run(void);
        char **_envToCharPtr(void);

    public:
        CGI(void);
        CGI(const CGI &other);
        ~CGI(void);
        CGI &operator=(const CGI &other);
        std::string run(void);
};


#endif
