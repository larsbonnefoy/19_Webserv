#include "../Includes/Server.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main (int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Unvalid Input Files" << std::endl;
        return (1);
    }
    std::ifstream file(argv[1]); 
    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    Server defaultServer;
    std::cout << defaultServer;
    return 0;
}
