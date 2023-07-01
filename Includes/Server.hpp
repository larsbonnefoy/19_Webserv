#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <ctype.h>
#include <map>
#include <vector>
#include <sys/types.h>
#include <iostream>
#include "../Includes/Location.hpp"

class Server {
    private:
        std::string                     _ip;
        size_t                          _port;
        size_t                          _maxBodySize;
        std::string                     _serverName;
        std::map<size_t, std::string>    _errors;
        std::vector<Location>            _locations;

    public:
        static std::string                  root;

        Server(void);
        Server(const Server &other);
        ~Server(void);
        Server &operator=(const Server &other);
        
        static void setServerRoot(std::string root);
        void        setIp(std::string ip);
        void        setPort(size_t port);
        void        setMaxBodySize(size_t size);
        void        setName(std::string name);
        void        setError(size_t errCode, std::string errPath);
        void        setLocation(Location &loc);

        std::string                     getIp(void)   const;
        size_t                          getPort(void) const;
        size_t                          getMaxBodySize(void) const;
        std::string                     getName(void) const;
        std::map<size_t, std::string>   getErrors(void) const;
        std::vector<Location>           getLocations(void) const;
};

std::ostream &operator<<(std::ostream &out, const Server &pt);

#endif
