cmake_minimum_required(VERSION 3.25.1)


add_library(
    block 
    STATIC    
    block.h
    block.cpp
)

target_include_directories(
    block 
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)

# target_link_libraries(
#     argc
#     PRIVATE
#     Boost::boost_multi_index
# )
