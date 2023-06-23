#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <ctype.h>
#include <map>
#include <sys/types.h>

class Server {
    private:
        std::string                   _ip;
        uint32_t                      _port;
        std::string                   _serverName;
        std::map<uint32_t, std::string>    _errors;

    public:
        Server(void);
        Server(std::string ip, uint32_t port, std::string serverName, std::map<uint32_t, std::string> &errors);
        Server(const Server &other);
        ~Server(void);
        Server &operator=(const Server &other);
        
        void        setIp(std::string ip);
        void        setPort(uint32_t port);
        void        setName(std::string name);
        void        setError(uint32_t errCode, std::string errPath);

        std::string                     getIp(void)   const;
        uint32_t                        getPort(void) const;
        std::string                     getName(void) const;
        std::map<uint32_t, std::string> getErrors(void) const;
};

std::ostream &operator<<(std::ostream &out, const Server &pt);

#endif
