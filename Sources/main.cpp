#include "../Includes/Server.hpp"
#include "../Includes/ConfigParser.hpp"
#include "../Includes/Config.hpp"
#include "../Includes/Mux.hpp"
#include "../Includes/Socket.hpp"
#include "../Includes/Log.hpp"
#include "../Includes/Http.hpp"
#include "../Includes/HttpResponse.hpp"
#include "../Includes/Signal.hpp"
#include "../Includes/Log.hpp"
#include "../Includes/isGood.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sys/types.h>

int main (int argc, char *argv[])
{
    //should check for input file validity here
    //If file exists and if permissions on file
    //or empty file
    if (argc != 2) {
        std::cout << "Unvalid Input File" << std::endl;
        return (1);
    }

    Config conf;

    try {
        ws_signal();
        parseConfig(argv[1], conf);
        StaticInit::initStatic();
        Mux	mux(conf);
        mux.run();
    }
    catch (std::exception &e) {
        ws_log(e.what());
    }
    return 0;
}
