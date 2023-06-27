#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <cstddef>
#include <netinet/in.h>
#include <vector>
#include "../Includes/Server.hpp"

//essayer de mettre _server en const mais tendu i guess
class Config {
    private:
        std::map<size_t, Server> _servers; 

    public:
        Config(void);
        Config(const Config &other);
        ~Config(void);
        Config &operator=(const Config &other); 

        std::map<size_t, Server> &getServers(void);
};

std::ostream &operator<<(std::ostream &out, Config &pt);

#endif
