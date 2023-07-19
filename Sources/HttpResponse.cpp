#include "../Includes/HttpResponse.hpp"
#include <string>
#include <sys/stat.h>

HttpResponse::HttpResponse(void) : _statusCode(200){
    std::string body =  StaticInit::DEF_FILE;

    this->_statusPhrase = StaticInit::STATUS_CODE_PHRASE[_statusCode];

    this->setStartLine(_makeStartLine());
    this->addToHeaderField("Content-Type", "text/html");

    this->addToHeaderField("Content-Length", valToString(body.length()));
    this->setBody(body);
}

//Check for valid code!;
HttpResponse::HttpResponse(std::string url, size_t code) {
    this->_statusCode = code;
    this->_statusPhrase = StaticInit::STATUS_CODE_PHRASE[code];

    this->setStartLine(_makeStartLine());

    if (code == 200 && _isDirectory(url)) {
        this->_handleAutoIndex(url);
    }
    else { 
        this->_handleURL(url);
    }
}

HttpResponse::HttpResponse(const HttpResponse &other) 
    : Http(other), _statusCode(other._statusCode), _statusPhrase(other._statusPhrase), _path(other._path), _autoindex(other._autoindex), _cgi(other._cgi), _methode(other._methode) {
}

HttpResponse::~HttpResponse(void) {
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other) {

    this->_statusCode = other._statusCode;
    this->_statusPhrase = other._statusPhrase;
    this->_path = other._path;
    this->_autoindex = other._autoindex;
    this->_cgi = other._cgi;
    this->_methode = other._methode; 

    return *this;
} 

/******************************PRIVATE FUNCTIONS*******************************/

void HttpResponse::_handleAutoIndex(const std::string &url) {
    std::string htmlAutoIndex;
    this->addToHeaderField("Content-Type", StaticInit::MIME_TYPES["html"]);
    htmlAutoIndex = _createHTMLAutoindex(url);
    this->addToHeaderField("Content-Length", valToString(htmlAutoIndex.size()));
    this->setBody(htmlAutoIndex);
}

std::string HttpResponse::_createHTMLAutoindex(const std::string &url) {
    DIR *dir = opendir(url.c_str());
    if (dir == NULL) {
        return "";
    }
    
    std::stringstream htmlContent;
    htmlContent << "<!DOCTYPE html>\n<html>\n";
    htmlContent << "<head>\n";
    htmlContent << "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css\">\n";
    htmlContent << "<title> Index " << this->_uri << "</title>"; 
    htmlContent << "</head>\n";
    htmlContent << "<body>\n";
    htmlContent << "<h1 style=\"font-family: Courier, monospace;\"> Index of " << this->_uri<<"</h1>\n"; 

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string objPath = url;
        if (url.back() != '/') { 
            objPath.append("/");
        }

        std::string name = entry->d_name;
        if (name == ".") {
            continue;
        }

        objPath += name;

        struct stat statbuf;
        if (stat(objPath.c_str(), &statbuf) != 0) {
            break;
        }
        if (name == ".") {
            continue;
        }

        htmlContent << "<p style=\"font-family: Courier, monospace;\">";
        if (S_ISDIR(statbuf.st_mode)) {
            htmlContent << "<i class=\"fas fa-folder\"></i>";
        }
        else {
            htmlContent << "<i class=\"fas fa-file\"></i>";
        }
        htmlContent << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
        if (this->_uri.back() != '/') {
            this->_uri += '/';
        }
        htmlContent << "<a href=\"" << this->_uri + name << "\">" << name << "</a>";
        for (int spaces = 50 - name.length(); spaces > 0; spaces--) {
            htmlContent << "&nbsp;";
        }
        htmlContent << statbuf.st_size << "b";
        htmlContent << "</p>\n";
    }
    
    htmlContent << "</body>\n</html>";
    closedir(dir);
    return (htmlContent.str());
}

void    HttpResponse::_handleURL(std::string &url) {
    this->addToHeaderField("Content-Type", _getMIMEType(url));
    this->addToHeaderField("Content-Length", valToString(_getFileSize(url)));
    this->setBody(_fileToString(url));
}

void    HttpResponse::_handleRedirection(void)
{
	this->addToHeaderField("Content-Type","Moved Permanently");
	this->addToHeaderField("Location", this->_path);
	this->_body = "";
	this->addToHeaderField("Content-Length", valToString(this->_body.size()));
}

bool HttpResponse::_isDirectory(const std::string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

std::string HttpResponse::_makeStartLine(void) {
    std::string startLine = "HTTP/1.1";

    startLine += " ";
    startLine += valToString(_statusCode);
    startLine += " ";
    startLine += _statusPhrase;

    return (startLine);
}

size_t      HttpResponse::_getFileSize(std::string &url) {
    struct stat st;
    stat(url.c_str(), &st);
    return (st.st_size);
}

/*
 * Returns file extension of url, returns empty string if file extension is Empty
 */ 
std::string HttpResponse::_getFileExtension(std::string &url) {
    size_t lastDot = url.find_last_of(".");
    if (lastDot == std::string::npos) {
        return ("");
    }
    return (url.substr(lastDot + 1));
}

std::string HttpResponse::_getMIMEType(std::string &url) {
    return (StaticInit::MIME_TYPES[_getFileExtension(url)]);
}

std::string HttpResponse::_fileToString(std::string &url) {
    std::ifstream file(url.c_str());
    std::stringstream buffer;
    buffer << file.rdbuf();
    return (buffer.str());
}
/******************************PUBLIC FUNCTIONS********************************/
void HttpResponse::setStatusCode(size_t code) {
    this->_statusCode = code;
}

void HttpResponse::setStatusPhrase(std::string phrase) {
    this->_statusPhrase = phrase;
}

size_t  HttpResponse::getStatusCode(void) const {
    return (this->_statusCode);
}

std::string HttpResponse::getStatusPhrase(void) const {
    return (this->_statusPhrase);
}

std::string HttpResponse::convertToStr(void) {
    std::string responseStr; 
    
    responseStr = getStartLine();
    responseStr += "\r\n";
    responseStr += headerToStr();
    responseStr += "\r\n";
    responseStr += getBody();
    return (responseStr);
}

std::ostream &operator<<(std::ostream &out, HttpResponse &httpRes) {
    out << httpRes.convertToStr();
    return (out);
}
