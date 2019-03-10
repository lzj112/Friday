cmake_minimum_required (VERSION 3.0)

project (Connect)


add_subdirectory(src)

add_definitions("-g -Wall -std=c++11")

add_executable(server server.cpp)
target_link_libraries(server FridayNET pthread)

add_executable(client client.cpp)
target_link_libraries(client FridayNET pthread)

