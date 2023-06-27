#include "../Includes/Server.hpp"
#include "../Includes/ConfigParser.hpp"
#include "../Includes/Config.hpp"
#include <string>
#include <iostream>
#include <vector>

int main (int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Unvalid Input Files" << std::endl;
        return (1);
    }
    try {
        Config *defaultConfig = parseConfig(argv[1]);
        std::map<size_t, Server> servers = defaultConfig->getServers();

        for (std::map<size_t, Server>::iterator it=servers.begin();  it !=  servers.end(); it++) {
            std::cout << it->second << std::endl;
        }

        delete defaultConfig;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
