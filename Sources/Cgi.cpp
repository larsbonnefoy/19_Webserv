#include "../Includes/Cgi.hpp" 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFERSIZE 500

Cgi::Cgi(void) {
}

//Si GET: CONTENT_LENGTH = body len,  on met les infos dans query QUERY_STRING 
//Si POST: CONTENT_LENGHT = NULL, write le body dans pipe et dup pipe sur stdin;
Cgi::Cgi(HttpRequest &request, std::string path) {
    std::cout << "===CGI Constructor===" << std::endl;

    this->_pathInfo = path; //change once path as been added to request;
    ws_log(path);
    ws_log(request.getPayload());
    ws_log(request.getMethode());

    this->_av.push_back("");
    
    this->_env["REQUEST_METHOD="] = request.getMethode();
    if (request.getMethode() == "GET") {
        this->_method = GET;
        this->_data = request.getPayload();
        this->_env["CONTENT_LENGTH="] = "NULL";
        this->_env["QUERY_STRING="] = this->_data;
        ws_log(this->_data);
    } 
    else if (request.getMethode() == "POST") {
        this->_method = POST;
        this->_data = request.getBody() + '\0';
        this->_env["CONTENT_LENGTH="] = request.valToString(_data.length());
        this->_env["QUERY_STRING="] = "NULL";
        ws_log(this->_data);
    }

    std::map<std::string, std::string> requestHeaderFields = request.getHeaderField();

    this->_env["CONTENT_TYPE="] = requestHeaderFields["Content-Type"];
    this->_env["HTTP_ACCEPT="] = requestHeaderFields["Accept"];
    this->_env["HTTP_ACCEPT_ENCODING="] = requestHeaderFields["Accept-Encoding"];
    this->_env["HTTP_ACCEPT_LANGUAGE="] = requestHeaderFields["Accept-Language"];
    this->_env["PATH_INFO="] = this->_pathInfo;
    this->_env["SERVER_PROTOCOL="] = "HTTP/1.1";
    this->_env["HOST="] = requestHeaderFields["Host"];
    this->_env["HTTP_USER_AGENT="] = requestHeaderFields["User-Agent"];
    this->_env["HTTP_CONNECTION="] = requestHeaderFields["Connection"];

    std::cout << "==CGI END====" << std::endl;
}

Cgi::Cgi(const Cgi &other) : _env(other._env) {
}

Cgi::~Cgi(void) {
}

Cgi &Cgi::operator=(const Cgi &other) {
    this->_env = other._env;
    return *this;
}
/*------------------------Private member functions----------------------------*/
//_convToTab should be template but un peu la flemme comme on dit chez moi
//Dans le cas d'un pd d'execve ou d'alloc ici il faut FREEEEEE!!!!
char **Cgi::_convToTab(std::map<std::string, std::string> env) {
    size_t size = env.size();

    char **envp = new char*[size + 1];
    if (envp == NULL) {
        ws_log(strerror(errno));
        throw InternalError();
    }
    size_t i = 0;
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); ++it) {
        std::string str = it->first + it->second;
        envp[i] = new char[str.length() + 1];
        if (envp[i] == NULL) {
            ws_log(strerror(errno));
            throw InternalError();
        }
        strcpy(envp[i], str.c_str());
        i++; 
    }
    envp[i] = NULL;
    return (envp);
}

char **Cgi::_convToTab(std::vector<std::string> av) {
    size_t size = av.size();

    char **avp = new char*[size + 1];
    if (avp == NULL) {
        ws_log(strerror(errno));
        throw InternalError();
    }
    size_t i = 0;
    for (std::vector<std::string>::iterator it = av.begin(); it != av.end(); ++it) {
        avp[i] = new char[av[i].length() + 1];
        if (avp[i] == NULL) {
            ws_log(strerror(errno));
            throw InternalError();
        }
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
std::string Cgi::_readFromPipe(int pipeRead){
    int charRead;
    std::string out = "";

    char *buffer = new char[BUFFERSIZE + 1];
    if (buffer == NULL) {
        ws_log(strerror(errno));
        throw InternalError();
    }

    //Dans quel cas peut on ne pas avoir de '\0' quand charRead == 0?
    while ((charRead = read(pipeRead, buffer, BUFFERSIZE))) {
        if (charRead == -1) {
            std::cerr << "Read form pipe failed" << std::endl; 
            delete[] buffer;
            ws_log("read");
            throw InternalError();
        }
        buffer[charRead] = '\0';
        out += buffer;
    }
    delete[] buffer;
    return (out);
}
/*
 * Return internal error if Cgi fails;
 */ 
std::string Cgi::run(void) {
    ws_log("1");
    int pipeData[2], pipeCGI[2];
    pid_t pid;
    std::string cgiOut;

    if (pipe(pipeData) == -1 || pipe(pipeCGI) == -1) {
        ws_log(strerror(errno));
        throw InternalError();
    }
    ws_log("2");
    pid = fork();
    if (pid == -1) {
        ws_log(strerror(errno));
        throw InternalError();
    }
    
    //CHILD (executes CGI)
    //  writes POST DATA to pipeData[1]
    //  sets pipeData[0] to STDIN_FILENO (CGI process can then read from pipeData[1] on stdIn)
    //  sets pipeCGI[1] to STDOUT_FILENO (CGI process can write its output into the pipe)
    //  TO CLOSE pipeData[1], pipeCGI[2]???
    else if (pid == 0) {
        ws_log("In child");
        if (this->_method == POST) {
            char const *toWrite = this->_data.c_str();
            ws_log("writing to pipe alllooo");

//            std::cerr << "-----------\nIN CHILD: Write data to pipe for cgi script\n";
  //          std::cerr << toWrite << " (len = " << this->_data.length()<< ")\n------------\n";
            
            std::cerr << PIPE_BUF * 8 << std::endl;
            std::cerr << "Writing " << this->_data.length() << " bytes to pipe" << std::endl;
            //should write to tmp file here
            if (write(pipeData[1], toWrite, this->_data.length()) == -1) {
                std::cerr << "WRITE FAILED" << std::endl;
                ws_logErr(strerror(errno));
                throw InternalError();
            };
            std::cout << "HUUUUUUUUUUM" << std::endl; 
        }

        std::cerr << "2" << std::endl;
        dup2(pipeData[0], STDIN_FILENO);
        close(pipeData[0]);

        std::cerr << "3" << std::endl;
        dup2(pipeCGI[1], STDOUT_FILENO);
        close(pipeCGI[1]);

        std::cerr << "4" << std::endl;
        char **av = _convToTab(this->_av);
        char **env = _convToTab(this->_env);
        
        if (execve(this->_pathInfo.c_str(), av, env) == -1) {
            delete[] av;
            delete[] env;
            std::exit(1);
        }
    }

    //PARENT 
    // Reads CGI ouput from pipeCGI[0] 
    else {
        ws_log("In Parent");

        close(pipeData[0]);
        close(pipeData[1]);
        close(pipeCGI[1]);

        int status;
        waitpid(pid, &status , 0);
        if (WIFEXITED(status)) {
            int exitCode = WEXITSTATUS(status);
            if (exitCode != 0) {
                ws_log("CGI script failed\n");
                throw InternalError();
            }
        }
        cgiOut = _readFromPipe(pipeCGI[0]);
        ws_log("CGI Response Recieved");
        close(pipeCGI[0]);
    }
    return (cgiOut);
}

/*-----------------------------EXCEPTIONS-----------------------------------*/

const char *Cgi::InternalError::what() const throw() {
        return("[Cgi::InternalError] : Interal Server Error Cgi.");
}
