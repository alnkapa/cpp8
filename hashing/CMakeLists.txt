cmake_minimum_required(VERSION 3.25.1)


add_library(
    hashing 
    STATIC    
    hash.h
    hash.cpp
)

target_include_directories(
    hashing 
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)

find_package(OpenSSL REQUIRED)

target_link_libraries(
    hashing
    PRIVATE
    OpenSSL::SSL OpenSSL::Crypto
)
