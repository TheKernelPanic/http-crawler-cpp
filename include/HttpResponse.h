#ifndef CPP_ENV_RESPONSE_H
#define CPP_ENV_RESPONSE_H

#include <string>
#include <vector>
#include <regex>
#include "HttpHeader.h"

class HttpResponse {

public:

    HttpResponse(const std::string &buffer);

    int getStatusCode();
    std::string getBody();
    std::string getHeader(const std::string name);

private:
    std::vector<HttpHeader> headers;
    int statusCode = 0;
    std::string body;
};

#endif
