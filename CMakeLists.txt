cmake_minimum_required(VERSION 3.10)

project(Panik0Crawler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
        include
)

set(SOURCES
        src/HttpClient.cpp
        src/HttpResponse.cpp
        src/HttpHeader.cpp
        src/main.cpp
        src/HttpResponse.cpp
        src/HttpResponse.cpp
)

add_executable(
        Panik0Crawler ${SOURCES}
        src/HttpResponse.cpp
)

find_package(OpenSSL REQUIRED)

target_link_libraries(Panik0Crawler OpenSSL::SSL OpenSSL::Crypto)