cmake_minimum_required(VERSION 3.25.1)


add_library(
    files 
    STATIC    
    files.h
    files.cpp
)

target_include_directories(
    files 
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
