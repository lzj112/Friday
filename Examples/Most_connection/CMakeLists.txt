cmake_minimum_required (VERSION 3.0)

project (MOst_Conn)


add_subdirectory(src)

add_definitions("-g -Wall -std=c++11")

add_executable(server Server.cpp)
target_link_libraries(server FridayNET pthread)

add_executable(client Client.cpp)
target_link_libraries(client FridayNET pthread)

