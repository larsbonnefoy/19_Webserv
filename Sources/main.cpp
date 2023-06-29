#include "../Includes/Server.hpp"
#include "../Includes/ConfigParser.hpp"
#include "../Includes/Config.hpp"
#include "../Includes/Mux.hpp"
#include "../Includes/Socket.hpp"
#include "../Includes/Log.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sys/types.h>

int main (int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Unvalid Input Files" << std::endl;
        return (1);
    }
    try {
        Config *defaultConfig = parseConfig(argv[1]);
        // std::map<size_t, Server> servers = defaultConfig->getServers();
		try
		{
			Mux	mux(*defaultConfig);
			mux.run();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		delete defaultConfig;

        // for (std::map<size_t, Server>::iterator it=servers.begin();  it !=  servers.end(); it++) {
        //     std::cout << it->second << std::endl;
        // }
        // delete defaultConfig;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
