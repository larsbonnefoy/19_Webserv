#include "../Includes/Cgi.hpp" 
#include <cstdlib>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFERSIZE 500

int main(int ac, char **av) {
    CGI test;
    std::string out = test.run();
    std::cout << "[" << out << "]" << std::endl;
    return 0;
}

CGI::CGI(void) {
}

CGI::CGI(const CGI &other) : _env(other._env) {
}

CGI::~CGI(void) {
}

CGI &CGI::operator=(const CGI &other) {
    this->_env = other._env;
    return *this;
}

/*
 * Return internal error if CGI fails;
 */ 
std::string CGI::run(void) {
    int fdPipe[2];
    pid_t pid;
    std::string CgiOut;

    if (pipe(fdPipe) == -1) {
        std::cout << "Error" << std::endl;
    }
    pid = fork();
    if (pid == -1) {
        std::cout << "Error" << std::endl;
    }
    else if (pid == 0) {
        close(fdPipe[0]);
        dup2(fdPipe[1], STDOUT_FILENO);
        close(fdPipe[1]);

        char *cgi_path = "/Users/larsbonnefoy/projects/19_Webserv/site/data/cgi-bin/add.cgi";

        //eft somehow osef que argv[0] == cgi name, ce qui comte c'est cgi_path;
        char *argv[] = {"", NULL};
        char *envp[] = {"test=salut", "MEEP=ALLO", NULL};
        if (execve(cgi_path, argv, envp) == -1) {
            std::cerr << "Execve failed" << std::endl;
        }
    }
    else {
        close(fdPipe[1]);

        int status;
        waitpid(-1, &status , WNOHANG);
        

        //check for new return if fail
        char    *buffer = new char[BUFFERSIZE + 1];
        int     charRead;

        while ((charRead = read(fdPipe[0], buffer, BUFFERSIZE))) {
                buffer[charRead] = '\0';
                CgiOut.append(buffer);
            if (charRead == -1) {
                std::cout << "[ERROR charRead == -1]" << std::endl;
                CgiOut += '\0';
                break;
            }
        }
        if (charRead == 0) {
            std::cout << "[EOF REACHED]" << std::endl;
            CgiOut += '\0';
        }
        delete[] buffer;
        close(fdPipe[0]);
    }
    return (CgiOut);
}
