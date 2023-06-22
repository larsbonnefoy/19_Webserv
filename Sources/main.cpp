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

    Config *defaultConfig = parseConfig(argv[1]);
    std::vector<Server> testServers = defaultConfig->getServers();

    for (uint32_t i = 0; i < testServers.size(); i++) {
        std::cout << testServers[i];
    }
    delete defaultConfig;
    return 0;
}
