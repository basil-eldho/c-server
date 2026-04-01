cmake_minimum_required(VERSION 3.10)
project(server C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(server src/server.c)