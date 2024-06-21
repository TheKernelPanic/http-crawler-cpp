#include "../include/HttpHeader.h"
#include <string>

HttpHeader::HttpHeader(const std::string name, const std::string value):
    name(name), value(value){
}

std::string HttpHeader::getName() {
    return this->name;
}

std::string HttpHeader::getValue() {
    return this->value;
}