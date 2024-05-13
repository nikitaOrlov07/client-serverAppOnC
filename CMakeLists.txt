cmake_minimum_required(VERSION 3.27)
project(client_server C)

set(CMAKE_C_STANDARD 11)

add_library(client_server STATIC
        echoClient.c
        echoServer.c)
