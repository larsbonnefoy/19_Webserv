#include "../Includes/Cgi.hpp" 
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFERSIZE 500
/*
int main(int ac, char **av) {
    cgi test;
    std::string out = test.run();
    std::cout << "[" << out << "]" << std::endl;
    return 0;
}
*/
cgi::cgi(void) {
}

//Si GET: CONTENT_LENGTH = body len,  on met les infos dans query QUERY_STRING 
//Si POST: CONTENT_LENGHT = NULL, write le body dans pipe et dup pipe sur stdin;
cgi::cgi(HttpRequest &request, std::string path) {
    std::cout << "---CGI Constructor---" << std::endl;

    this->_pathInfo = path;
    ws_log(path);

    this->_av.push_back("");

    this->_env["REQUEST_METHOD="] = request.getMethode();
    std::cout << request.getMethode() << std::endl;
    //un peu degeu d'utiliser valToString de Http
    if (request.getMethode() == "GET") {
        this->_method = GET;
        this->_data = "placeholder";
        this->_env["CONTENT_LENGTH="] = "NULL";
        this->_env["QUERY_STRING="] = this->_data;
    } 
    else if (request.getMethode() == "POST") {
        this->_method = POST;
        this->_data = request.getBody() + '\0';
        this->_env["CONTENT_LENGTH="] = request.valToString(_data.length());
        this->_env["QUERY_STRING="] = "NULL";
    }
    this->_env["CONTENT_TYPE="] = "";
    this->_env["PATH_INFO="] = "";
    std::cout << "------" << std::endl;
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
 * Return internal error if cgi fails;
 */ 
std::string cgi::run(void) {
    int fdPipe[2];
    pid_t pid;
    std::string CgiOut;

    if (pipe(fdPipe) == -1) {
        std::cerr<< "Error" << std::endl;
    }
    pid = fork();
    if (pid == -1) {
        std::cerr << "Error" << std::endl;
    }

    //child
    else if (pid == 0) {
        if (this->_method == POST) {
            char const *toWrite = this->_data.c_str();
            std::cerr << "-----------\nIN CHILD: Write data to pipe for cgi script\n" << toWrite << " (len = " << this->_data.length()<< ")\n------------\n" << std::endl;
            if (write(fdPipe[1], &toWrite, this->_data.length()) == -1) {
                perror("Write");
                std::cerr << "Write failed" << std::endl;
            };
            //script cannot read from pipe, or has to much info double pipe?
            //dup2(fdPipe[0], STDIN_FILENO);
        }
        close(fdPipe[0]);

        dup2(fdPipe[1], STDOUT_FILENO);
        close(fdPipe[1]);

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

    //Parent
    else {
        close(fdPipe[1]);

        int status, charRead;
        waitpid(-1, &status , WNOHANG);
        
        //check for new return if fail
        char    *buffer = new char[BUFFERSIZE + 1];
        if (buffer == NULL) {
            std::cerr << "Bad alloc" << std::endl;
        }

        //for some reason y a de la merde dans le tuyeaux
        while ((charRead = read(fdPipe[0], buffer, BUFFERSIZE))) {
            if (charRead == -1) {
                std::cerr << "[ERROR charRead == -1]" << std::endl;
                CgiOut += '\0';
                break;
            }
            std::cout << "bufstart {" << buffer[0] << "} bufend {" << buffer[charRead] << "}\n";
            buffer[charRead] = '\0';
            std::cerr << "PARENT buffer content [" << buffer << "]" << std::endl;
            CgiOut.append(buffer);
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
