#include "../Includes/Config.hpp"
#include "../Includes/Server.hpp"
#include <vector>

Config::Config(void) {
}

Config::Config(const Config &other) : _servers(other._servers) {
}

Config::~Config(void) {
}

Config &Config::operator=(const Config &other) {
    (void) other;
    return *this;
}

std::vector<Server> &Config::getServers(void) {
    return (this->_servers);
}

//overload not working idk why
std::ostream &operator<<(std::ostream &out, Config &config) {

    std::vector<Server> servers = config.getServers();
    int32_t size = servers.size();

    for (int32_t i = 0; i < size; i++) {
        out << servers[i];
    }
    return (out);
}
