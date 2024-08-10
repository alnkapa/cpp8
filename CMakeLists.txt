cmake_minimum_required(VERSION 3.25.1)

project(Bayan VERSION 0.0.1)

add_executable(bayan main.cpp)

add_subdirectory(argc)
add_subdirectory(hashing)

target_link_libraries(
    bayan
    PRIVATE
    argc
    hashing
)

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set(CMAKE_BUILD_TYPE Debug)
