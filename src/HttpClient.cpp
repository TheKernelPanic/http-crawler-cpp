#include "../include/HttpClient.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define DEFAULT_BUFFER_SIZE 1024 * 1024

HttpClient::HttpClient(const std::string &hostname, bool enableSSL)
    : hostname(hostname), enableSSL(enableSSL) {

    this->port = enableSSL ? 443 : 80;
    this->bufferSize = DEFAULT_BUFFER_SIZE;
    this->sockfd = -1;

    if (this->enableSSL) {
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();

        ctx = SSL_CTX_new(SSLv23_client_method());
        if (ctx == nullptr) {
            throw std::runtime_error("Failed to create SSL context");
        }
    }
    connectToServer();
}

HttpClient::~HttpClient() {
    closeConnection();
}

void HttpClient::connectToServer() {

    struct hostent* server = gethostbyname(hostname.c_str());
    if (server == nullptr) {
        throw std::runtime_error("No such host");
    }

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    server_addr.sin_port = htons(port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("Opening socket failed");
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sockfd);
        throw std::runtime_error("Socket connection failed");
    }

    if (this->enableSSL) {
        ssl = SSL_new(ctx);
        if (ssl == nullptr) {
            throw std::runtime_error("Create ssl object failed");
        }
        // Create file descriptor
        SSL_set_fd(ssl, sockfd);
        if (SSL_connect(ssl) <= 0) {
            throw std::runtime_error("SSL connection failed");
        }
    }
}

void HttpClient::closeConnection() {

    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        ssl = nullptr;
    }
    if (sockfd >= 0) {
        close(sockfd);
        sockfd = -1;
    }
    if (ctx) {
        SSL_CTX_free(ctx);
        ctx = nullptr;
    }
}

HttpResponse HttpClient::get(const std::string& path) {

    std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + hostname +  "\r\nConnection: close\r\n\r\n";
    ssize_t sent = this->enableSSL ? SSL_write(ssl, request.c_str(), request.length()) : send(sockfd, request.c_str(), request.length(), 0);
    if (sent < 0) {
        throw std::runtime_error("Error writing to socket");
    }

    char buffer[this->bufferSize];
    std::memset(buffer, 0, this->bufferSize);

    std::string fullResponse;
    ssize_t received;
    while ((received = this->enableSSL ? SSL_read(ssl, buffer, this->bufferSize - 1) : recv(sockfd, buffer, this->bufferSize - 1, 0)) > 0) {
        fullResponse.append(buffer, received);
    }
    return HttpResponse(fullResponse);
}

void HttpClient::setPort(const int value) {
    if (value < 1 || value > 65535) {
        std::runtime_error("Invalid port number");
    }
    this->port = value;
}

void HttpClient::setBufferSize(const unsigned long value) {
    this->bufferSize = value;
}