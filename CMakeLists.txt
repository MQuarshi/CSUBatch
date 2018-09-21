cmake_minimum_required(VERSION 3.12)
project(CSUBatch C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(CSUBatch
        main.c
        Queue.h menu.c)
