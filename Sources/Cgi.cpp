#include "../Includes/Cgi.hpp" 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFERSIZE 2048
static void freeTab(char **tab) {
    size_t i = 0;

    while (tab[i] != NULL) {
        delete[] tab[i];
    }
    delete[] tab;
}

Cgi::Cgi(void) {
}

//Si GET: CONTENT_LENGTH = body len,  on met les infos dans query QUERY_STRING 
//Si POST: CONTENT_LENGHT = NULL, write le body dans pipe et dup pipe sur stdin;
Cgi::Cgi(HttpRequest &request, std::string path, std::string uploadDir) {

    this->_pathInfo = path;

    this->_av.push_back("");
    
    this->_env["REQUEST_METHOD="] = request.getMethode();
    this->_env["UPLOAD_DIR="] = uploadDir;
    if (request.getMethode() == "GET") {
        this->_method = GET;
        this->_data = request.getPayload();
        this->_env["CONTENT_LENGTH="] = "NULL";
        this->_env["QUERY_STRING="] = this->_data;
    } 
    else if (request.getMethode() == "POST") {
        this->_method = POST;
        this->_data = request.getBody() + '\0';
        this->_env["CONTENT_LENGTH="] = request.valToString(_data.length());
        this->_env["QUERY_STRING="] = "NULL";
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
char **Cgi::_convToTab(std::map<std::string, std::string> env) {
    size_t size = env.size();

    char **envp = new char*[size + 1];
    if (envp == NULL) {
        ws_logErr(strerror(errno));
        exit(1);
    }
    size_t i = 0;
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); ++it) {
        std::string str = it->first + it->second;
        envp[i] = new char[str.length() + 1];
        if (envp[i] == NULL) {
            freeTab(envp);
            ws_logErr(strerror(errno));
            exit(1);
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
        ws_logErr(strerror(errno));
        exit(1);
    }
    size_t i = 0;
    for (std::vector<std::string>::iterator it = av.begin(); it != av.end(); ++it) {
        avp[i] = new char[av[i].length() + 1];
        if (avp[i] == NULL) {
            freeTab(avp);
            ws_logErr(strerror(errno));
            exit(1);
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

    char  buffer[BUFFERSIZE];
    while ((charRead = read(pipeRead, buffer, BUFFERSIZE))) {
        if (charRead == -1) {
            ws_logErr(strerror(errno));
            throw InternalError();
        }
        out.append(buffer, charRead);
    }
    return (out);
}

/*
 * Return internal error if Cgi fails;
 */ 
std::string Cgi::run(void) {
    int pipeCGI[2];
    pid_t pid;
    std::string cgiOut;

    if (pipe(pipeCGI) == -1) {
        ws_logErr(strerror(errno));
        throw InternalError();
    }

    pid = fork();
    if (pid == -1) {
        close(pipeCGI[0]);
        close(pipeCGI[1]);
        ws_logErr(strerror(errno));
        throw InternalError();
    }
    
    //CHILD (executes CGI)
    //  writes POST DATA to tmpFileWrite
    //  sets tmpFileWrite to STDIN_FILENO (CGI process can then read from tmpFileRead on stdIn)
    //  sets pipeCGI[1] to STDOUT_FILENO (CGI process can write its output into the pipe)
    else if (pid == 0) {
        FILE *tmpFileWrite = std::fopen("/tmp/CgiDataUpload", "w");
        if (tmpFileWrite == NULL) {
            ws_logErr(strerror(errno));
            exit(1);
        }
        if (this->_method == POST) {
            char const *toWrite = this->_data.c_str();

            if (write(fileno(tmpFileWrite), toWrite, this->_data.length()) == -1) {
                ws_logErr(strerror(errno));
                exit(1);
            }
        
        }
        close(fileno(tmpFileWrite));
        
        FILE *tmpFileRead = std::fopen("/tmp/CgiDataUpload", "r"); 
        dup2(fileno(tmpFileRead), STDIN_FILENO);
        close(fileno(tmpFileRead));

        dup2(pipeCGI[1], STDOUT_FILENO);
        close(pipeCGI[1]);

        char **av = _convToTab(this->_av);
        char **env = _convToTab(this->_env);
        
        size_t lastSlashIdx = this->_pathInfo.find_last_of("/");
		
		int ret = chdir(this->_pathInfo.substr(0, lastSlashIdx).c_str());
        if (ret == -1) {
            freeTab(av);
            freeTab(env);
            ws_logErr(this->_pathInfo);
            ws_logErr(strerror(errno));
            exit(1);
        }
		ret = execve(this->_pathInfo.c_str(), av, env);
        if (ret == -1) {
            freeTab(av);
            freeTab(env);
            std::exit(1);
        }
    }

    //PARENT 
    // Reads CGI ouput from pipeCGI[0] 
    else {

        close(pipeCGI[1]);

        int status;
        waitpid(pid, &status , 0);
        if (WIFEXITED(status)) {
            int exitCode = WEXITSTATUS(status);
            if (exitCode != 0) {
                close(pipeCGI[0]);
                ws_logErr("CGI script failed\n");
                throw InternalError();
            }
        }
        cgiOut = _readFromPipe(pipeCGI[0]);
        close(pipeCGI[0]);
    }
    std::remove("/tmp/CgiDataUpload");
    return (cgiOut);
}

/*-----------------------------EXCEPTIONS-----------------------------------*/

const char *Cgi::InternalError::what() const throw() {
        return("[Cgi::InternalError] : Interal Server Error Cgi.");
}
