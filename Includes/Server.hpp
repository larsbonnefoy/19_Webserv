#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <cstdint>
#include <map>
#include <sys/types.h>

class Server {
    private:
        const std::string                   _ip;
        const uint32_t                      _port;
        const std::string                   _serverName;
        const std::map<int, std::string>    _errors;        

    public:
        Server(void);
        Server(std::string ip, uint32_t port, std::string serverName, std::map<int, std::string> &errors);
        Server(const Server &other);
        ~Server(void);
        Server &operator=(const Server &other);
        
        std::string   getIp(void) const;
        uint32_t      getPort(void) const;
        std::string   getName(void) const;
};

std::ostream &operator<<(std::ostream &out, const Server &pt);

#endif
