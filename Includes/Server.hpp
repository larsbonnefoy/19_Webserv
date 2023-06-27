#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <ctype.h>
#include <map>
#include <vector>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include "../Includes/Location.hpp"

class Server {
    private:
        std::string                         _ip;
        uint32_t                            _port;
        uint32_t                            _maxBodySize;
        std::string                         _serverName;
        std::map<uint32_t, std::string>     _errors;
        std::vector<Location>               _locations;

    public:
        Server(void);
        Server(const Server &other);
        ~Server(void);
        Server &operator=(const Server &other);
        
        void        setIp(std::string ip);
        void        setPort(uint32_t port);
        void        setMaxBodySize(uint32_t size);
        void        setName(std::string name);
        void        setError(uint32_t errCode, std::string errPath);
        void        setLocation(Location &loc);

        std::string                     getIp(void)   const;
        uint32_t                        getPort(void) const;
        uint32_t                        getMaxBodySize(void) const;
        std::string                     getName(void) const;
        std::map<uint32_t, std::string> getErrors(void) const;
        std::vector<Location>           getLocations(void) const;
};

std::ostream &operator<<(std::ostream &out, const Server &pt);

#endif
