#include "../Includes/HttpResponse.hpp"
#include <string>

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

    this->setStartLine(makeStartLine());
    this->addToHeaderField("Content-Type", "text/html");
    this->addToHeaderField("Content-Length", "617");
    this->setBody(body);
    std::cout << *this << std::endl;
}

//Check for valid code!;
HttpResponse::HttpResponse(std::string uri, size_t code) {
    (void) uri;
    this->_statusCode = code;
    this->_statusPhrase = StaticInit::STATUS_CODE_PHRASE[_statusCode];
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

std::string HttpResponse::makeStartLine(void) {
    std::string startLine = "HTTP/1.1";

    startLine += " ";
    startLine += std::to_string(_statusCode);
    startLine += " ";
    startLine += _statusPhrase;

    return (startLine);
}

std::ostream &operator<<(std::ostream &out, HttpResponse &httpRes) {
    out << httpRes.convertToStr();
    return (out);
}
