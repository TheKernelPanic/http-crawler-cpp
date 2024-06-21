#include <iostream>
#include <memory>
#include "../include/HttpClient.h"

int main() {

    try {
        HttpClient client("localhorse.net");
        HttpResponse response = client.get("/");

        std::cout << "Http request responds with " << response.getStatusCode() << std::endl;

    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    return 0;
}