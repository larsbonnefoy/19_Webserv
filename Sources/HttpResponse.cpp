#include "../Includes/HttpResponse.hpp"

HttpResponse::HttpResponse(void) : _statusCode(0), _statusPhrase("") {
    std::string body =  "<!DOCTYPE html>\r\n"
                        "<html lang=\"en\">\r\n"
                        "<head>\r\n"
                        "  <meta charset=\"UTF-8\" />\r\n"
                        "  <title>Hello, world!</title>\r\n"
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
    this->setStartLine("HTTP/1.1 200 OK");
    this->addToHeaderField("Content-Type", "text/html");
    this->addToHeaderField("Content-Length", "617");
    this->setBody(body);
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
    responseStr += "\n";
    responseStr += headerToStr();
    responseStr += "\r\n";
    responseStr += getBody();

    return (responseStr);
}
