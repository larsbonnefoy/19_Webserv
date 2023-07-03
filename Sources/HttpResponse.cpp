#include "../Includes/HttpResponse.hpp"
#include <string>
#include <sys/stat.h>

HttpResponse::HttpResponse(void) : _statusCode(200){
    std::string body =  StaticInit::DEF_FILE;

    this->_statusPhrase = StaticInit::STATUS_CODE_PHRASE[_statusCode];

    this->setStartLine(_makeStartLine());
    this->addToHeaderField("Content-Type", "text/html");
    this->addToHeaderField("Content-Length", _valToString(body.length()));
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
    : Http(other), _statusCode(other._statusCode), _statusPhrase(other._statusPhrase) {
}

HttpResponse::~HttpResponse(void) {
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other) {

    this->_statusCode = other._statusCode;
    this->_statusPhrase = other._statusPhrase;
    return *this;
}

/******************************PRIVATE FUNCTIONS*******************************/
void HttpResponse::_handleAutoIndex(const std::string &url) {
    std::cout << url << std::endl;
    std::string htmlAutoIndex;
    this->addToHeaderField("Content-Type", StaticInit::MIME_TYPES["html"]);
    htmlAutoIndex = _createHTMLAutoindex(url);
    this->addToHeaderField("Content-Length", _valToString(htmlAutoIndex.size()));
    this->setBody(htmlAutoIndex);
}

std::string HttpResponse::_createHTMLAutoindex(const std::string &url) {
    DIR *dir = opendir(url.c_str());
    if (dir == nullptr) {
        return "";
    }
    
    std::stringstream htmlContent;
    htmlContent << "<!DOCTYPE html>\n<html>\n<body>\n";
    htmlContent << "<h1> Index of " << url <<"</h1>\n"; 
    htmlContent << "<ul>\n";
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name == ".") {
            continue;
        }
        
        htmlContent << "<li><a href=\"" << name << "\">" << name << "</a></li>\n";
    }
    
    htmlContent << "</ul>\n</body>\n</html>";
    closedir(dir);
    
    return (htmlContent.str());
}

void    HttpResponse::_handleURL(std::string &url) {
    this->addToHeaderField("Content-Type", _getMIMEType(url));
    this->addToHeaderField("Content-Length", _valToString(_getFileSize(url)));
    this->setBody(_fileToString(url));
}

bool HttpResponse::_isDirectory(const std::string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

std::string HttpResponse::_valToString(size_t num) {
    std::stringstream ss;
    ss << num;
    return (ss.str());
}

std::string HttpResponse::_makeStartLine(void) {
    std::string startLine = "HTTP/1.1";

    startLine += " ";
    startLine += _valToString(_statusCode);
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
