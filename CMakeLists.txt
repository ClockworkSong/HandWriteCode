cmake_minimum_required(VERSION 3.15)
project(SocketTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(client test/client.cpp)
add_executable(server test/server.cpp)
#add_executable(ELT_CLT src/elite_client.cpp)