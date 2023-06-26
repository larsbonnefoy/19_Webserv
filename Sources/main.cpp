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
		ws_log("mux");
		Mux	mux(*defaultConfig);
		mux.run();

        // std::vector<Server> testServers = defaultConfig->getServers();

        // for (uint32_t i = 0; i < testServers.size(); i++) {
        //     std::cout << "===========SERV " << i << " ===============" << std::endl;
        //     std::cout << testServers[i];
        // }
        // delete defaultConfig;
		delete defaultConfig;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
