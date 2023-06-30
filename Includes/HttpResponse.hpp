#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "../Includes/Http.hpp"

class HttpResponse : public Http {
    private:
        size_t      _statusCode;
        std::string _statusPhrase;

    public:
        HttpResponse(void);
        HttpResponse(std::string uri, size_t code);
        HttpResponse(const HttpResponse &other);
        ~HttpResponse(void);
        HttpResponse &operator=(const HttpResponse &other);

        void setStatusCode(size_t code);
        void setStatusPhrase(std::string);

        size_t      getStatusCode(void) const;
        std::string getStatusPhrase(void) const;
        
        std::string convertToStr(void);
        std::string makeStartLine(void);

};

std::ostream &operator<<(std::ostream &out, HttpResponse &httpRes);
#endif
