cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 14)

add_executable(ELT_SRV server.cpp)
add_executable(ELT_CLT client.cpp)