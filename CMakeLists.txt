cmake_minimum_required(VERSION 3.15)
project(SocketTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(SRV server.cpp)
add_executable(CLT client.cpp)
add_executable(ELT_CLT EliteClient.cpp)