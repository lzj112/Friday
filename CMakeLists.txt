cmake_minimum_required (VERSION 3.0)
project (Friday_main)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/base SOURCE_BASE)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/net SOURCE_NET)

add_definitions("-g -Wall -std=c++11")

add_executable(main
               ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
               ${SOURCE_HELLO}
               ${SOURCE_WORLD})
