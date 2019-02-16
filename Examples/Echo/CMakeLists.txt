cmake_minimum_required (VERSION 3.0)

project (echo)


add_subdirectory(friday)

add_definitions("-g -Wall -std=c++11")
add_executable(echo EchoServer.cpp)

target_link_libraries(echo FridayNET pthread)