cmake_minimum_required (VERSION 3.0)

project (echo)


add_subdirectory(src)

add_definitions("-g -Wall -std=c++11")
add_executable(echo EchoServer.cpp)
target_link_libraries(echo FridayNET pthread)
add_executable(client EchoClient.cpp)
target_link_libraries(client FridayNET pthread)