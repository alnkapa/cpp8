cmake_minimum_required(VERSION 3.25.1)


add_library(
    hash 
    STATIC    
    hash.cpp 
    hash.h
)

target_include_directories(
    hash 
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(
    hash
    PRIVATE
    Boost::program_options
)

