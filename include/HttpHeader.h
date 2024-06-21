#ifndef CPP_ENV_HTTPHEADER_H
#define CPP_ENV_HTTPHEADER_H

#include <string>

class HttpHeader {
public:
    HttpHeader(const std::string name, const std::string value);

    std::string getName();
    std::string getValue();

private:
    std::string name;
    std::string value;
};

#endif
