

cmake_minimum_required(VERSION 3.0)

project(Friday)

set(CXX_FLAGS 
    -g 
    -Wall 
    -std=c++11)

link_libraries(pthread)

add_subdirectory(friday/net)
add_subdirectory(friday/base)


