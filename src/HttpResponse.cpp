#include "../include/HttpResponse.h"
#include <iostream>

HttpResponse::HttpResponse(const std::string &buffer) {

    // Retrieve status code
    std::regex statusCodeRegex("^HTTP/\\d\\.\\d (\\d{3}) .*");
    std::smatch statusCodeMatch;

    if (std::regex_search(buffer, statusCodeMatch, statusCodeRegex)) {
        this->statusCode = std::stoi(statusCodeMatch[1].str());
    } else {
        throw std::runtime_error("Cannot retrieve HTTP status code");
    }

    // Retrieve response headers
    std::string headersStr = buffer.substr(0, buffer.find("\r\n\r\n"));

    std::regex headerRegex(R"(^([^:]+):\s*(.*)\r?$)");
    std::smatch headerMatch;
    std::istringstream headersStream(headersStr);
    std::string line;

    while (std::getline(headersStream, line)) {
        if (std::regex_match(line, headerMatch, headerRegex)) {
            std::string name = headerMatch[1].str();
            std::string value = headerMatch[2].str();

            this->headers.push_back(HttpHeader(name, value));
        }
    }

    // Retrieve body
    this->body = buffer.substr(buffer.find("\r\n\r\n"), buffer.size());
}

int HttpResponse::getStatusCode() {
    return this->statusCode;
}

std::string HttpResponse::getHeader(const std::string name) {
    for (HttpHeader httpHeader : this->headers) {
        if (httpHeader.getName() == name) {
            return httpHeader.getValue();
        }
    }
    return nullptr;
}

std::string HttpResponse::getBody() {
    return this->body;
}