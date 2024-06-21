#ifndef CPP_ENV_HTTPCLIENT_H
#define CPP_ENV_HTTPCLIENT_H

#include <string>
#include <stdexcept>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <memory>

#include "../include/HttpResponse.h"

class HttpClient {
public:
    HttpClient(const std::string &hostname, bool enableSSL = true);
    ~HttpClient();

    HttpResponse get(const std::string& path);
    void setBufferSize(const unsigned long bytes);
    void setPort(int port);

private:

    std::string hostname;
    int port;
    int sockfd;
    unsigned long bufferSize;
    bool enableSSL;

    SSL_CTX *ctx;
    SSL *ssl;

    void connectToServer();
    void closeConnection();
};

#endif