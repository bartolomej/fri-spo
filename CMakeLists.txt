cmake_minimum_required(VERSION 3.29)
project(spo C)

set(CMAKE_C_STANDARD 11)

add_executable(spo tee1.c
        rep.c)
