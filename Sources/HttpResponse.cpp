#include "../Includes/HttpResponse.hpp"
#include <string>
#include <sys/stat.h>

std::map<size_t, std::string> StaticInit::STATUS_CODE_PHRASE;
std::map<std::string, std::string> StaticInit::MIME_TYPES;

HttpResponse::HttpResponse(void) : _statusCode(200){
    std::string body =  "<!DOCTYPE html>\r\n"
                        "<html lang=\"en\">\r\n"
                        "<head>\r\n"
                        "  <meta charset=\"UTF-8\" />\r\n"
                        "  <title>test test</title>\r\n"
                        "  <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\" />\r\n"
                        "  <meta name=\"description\" content=\"\" />\r\n"
                        "  <link rel=\"icon\" href=\"favicon.png\">\r\n"
                        "</head>\r\n"
                        "<body>\r\n"
                        "    <h1>Hello</h1>\r\n"
                        "    <form method=\"POST\">\r\n"
                        "        <label for=\"fname\">First name:</label><br>\r\n"
                        "        <input type=\"text\" id=\"fname\" name=\"fname\"><br>\r\n"
                        "        <label for=\"lname\">Last name:</label><br>\r\n"
                        "        <input type=\"text\" id=\"lname\" name=\"lname\"><br><br>\r\n"
                        "        <input type=\"submit\" value=\"Submit\">\r\n"
                        "    </form>\r\n"
                        "</body>\r\n"
                        "</html>";

    this->_statusPhrase = StaticInit::STATUS_CODE_PHRASE[_statusCode];

    this->setStartLine(_makeStartLine());
    this->addToHeaderField("Content-Type", "text/html");
    this->addToHeaderField("Content-Length", "617");
    this->setBody(body);
    std::cout << *this << std::endl;
}

//Check for valid code!;
HttpResponse::HttpResponse(std::string url, size_t code) {
    this->_statusCode = code;
    this->_statusPhrase = StaticInit::STATUS_CODE_PHRASE[code];

    this->setStartLine(_makeStartLine());
    this->_handleURL(url);
}

HttpResponse::HttpResponse(const HttpResponse &other) 
    : _statusCode(other._statusCode), _statusPhrase(other._statusPhrase) {
}

HttpResponse::~HttpResponse(void) {
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other) {

    this->_statusCode = other._statusCode;
    this->_statusPhrase = other._statusPhrase;
    return *this;
}

/******************************PRIVATE FUNCTIONS*******************************/
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

void    HttpResponse::_handleURL(std::string &url) {
    this->addToHeaderField("Content-Type", _getMIMEType(url));
    this->addToHeaderField("Content-Length", _valToString(_getFileSize(url)));
    this->setBody(_fileToString(url));
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
    std::ifstream file(url);
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
