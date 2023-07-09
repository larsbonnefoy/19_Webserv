#include "../Includes/Cgi.hpp" 
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFERSIZE 500

cgi::cgi(void) {
}

//Si GET: CONTENT_LENGTH = body len,  on met les infos dans query QUERY_STRING 
//Si POST: CONTENT_LENGHT = NULL, write le body dans pipe et dup pipe sur stdin;
cgi::cgi(HttpRequest &request, std::string path) {
    std::cout << "===CGI Constructor===" << std::endl;

    this->_pathInfo = path;
    ws_log(path);

    this->_av.push_back("");

    this->_env["REQUEST_METHOD="] = request.getMethode();
    std::cout << request.getMethode() << std::endl;
    if (request.getMethode() == "GET") {
        this->_method = GET;
        this->_data = "placeholder";
        this->_env["CONTENT_LENGTH="] = "NULL";
        this->_env["QUERY_STRING="] = this->_data;
    } 
    else if (request.getMethode() == "POST") {
        this->_method = POST;
        this->_data = request.getBody() + '\0';
        //un peu degeu d'utiliser valToString de Http
        this->_env["CONTENT_LENGTH="] = request.valToString(_data.length());
        this->_env["QUERY_STRING="] = "NULL";
    }
    this->_env["CONTENT_TYPE="] = "";
    this->_env["PATH_INFO="] = "";
    std::cout << "======" << std::endl;
}

cgi::cgi(const cgi &other) : _env(other._env) {
}

cgi::~cgi(void) {
}

cgi &cgi::operator=(const cgi &other) {
    this->_env = other._env;
    return *this;
}
/*------------------------Private member functions----------------------------*/
//_convToTab should be template but un peu la flemme comme on dit chez moi
//Dans le cas d'un pd d'execve ou d'alloc ici il faut FREEEEEE!!!!
char **cgi::_convToTab(std::map<std::string, std::string> env) {
    size_t size = env.size();

    char **envp = new char*[size + 1];
    if (envp == NULL) {
        std::cerr << "bad alloc" << std::endl;
        return (NULL);
    }
    size_t i = 0;
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); ++it) {
        std::string str = it->first + it->second;
        envp[i] = new char[str.length() + 1];
        strcpy(envp[i], str.c_str());
        i++; 
    }
    envp[i] = NULL;
    return (envp);
}

char **cgi::_convToTab(std::vector<std::string> av) {
    size_t size = av.size();

    char **avp = new char*[size + 1];
    if (avp == NULL) {
        std::cerr << "bad alloc" << std::endl;
        return (NULL);
    }
    size_t i = 0;
    for (std::vector<std::string>::iterator it = av.begin(); it != av.end(); ++it) {
        avp[i] = new char[av[i].length() + 1];
        strcpy(avp[i], av[i].c_str());
        i++; 
    }
    avp[i] = NULL;
    return (avp);
}
/*-------------------------Public member functions----------------------------*/
/*
 *  
 */ 
std::string cgi::_readFromPipe(int pipeRead){
    int charRead;
    std::string out = "";

    char *buffer = new char[BUFFERSIZE + 1];
    if (buffer == NULL) {
        std::cerr << "PTDR ERROR MALLOC" << std::endl;
    }

    //Dans quel cas peut on ne pas avoir de '\0' quand charRead == 0?
    while ((charRead = read(pipeRead, buffer, BUFFERSIZE))) {
        if (charRead == -1) {
            perror("read");
            out += '\0';
            break;
        }
        buffer[charRead] = '\0';
        out += buffer;
    }
    delete[] buffer;
    return (out);
}
/*
 * Return internal error if cgi fails;
 */ 
std::string cgi::run(void) {
    int pipeData[2], pipeCGI[2];
    pid_t pid;
    std::string cgiOut;

    if (pipe(pipeData) == -1 || pipe(pipeCGI) == -1) {
        std::cerr<< "Error" << std::endl;
    }
    pid = fork();
    if (pid == -1) {
        std::cerr << "Error" << std::endl;
    }
    
    //CHILD (executes CGI)
    //  writes POST DATA to pipeData[1]
    //  sets pipeData[0] to STDIN_FILENO (CGI process can then read from pipeData[1] on stdIn)
    //  sets pipeCGI[1] to STDOUT_FILENO (CGI process can write its output into the pipe)
    //  TO CLOSE pipeData[1], pipeCGI[2]???
    else if (pid == 0) {
        if (this->_method == POST) {
            char const *toWrite = this->_data.c_str();

            std::cerr << "-----------\nIN CHILD: Write data to pipe for cgi script\n";
            std::cerr << toWrite << " (len = " << this->_data.length()<< ")\n------------\n";
            
            if (write(pipeData[1], toWrite, this->_data.length()) == -1) {
                perror("Write");
                std::cerr << "Write failed" << std::endl;
            };
        }

        dup2(pipeData[0], STDIN_FILENO);
        close(pipeData[0]);

        dup2(pipeCGI[1], STDOUT_FILENO);
        close(pipeCGI[1]);

        char **av = _convToTab(this->_av);
        char **env = _convToTab(this->_env);

        //eft somehow osef que argv[0] == cgi name, ce qui comte c'est cgi_path;
        if (execve(this->_pathInfo.c_str(), av, env) == -1) {
            std::cerr << "Execve failed" << std::endl;
            delete[] av;
            delete[] env;
            exit(1);
        }
    }

    //PARENT 
    // Reads CGI ouput from pipeCGI[0] 
    else {

        close(pipeData[0]);
        close(pipeData[1]);
        close(pipeCGI[1]);

        int status;
        waitpid(-1, &status , WNOHANG);
    
        cgiOut = _readFromPipe(pipeCGI[0]);
        close(pipeCGI[0]);
    }
    return (cgiOut);
}
