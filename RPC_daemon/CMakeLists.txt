cmake_minimum_required(VERSION 3.16)
project(RPC_daemon)

set(CMAKE_CXX_STANDARD 14)

add_executable(RPC_daemon main.cpp server.h client.h)