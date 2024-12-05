cmake_minimum_required(VERSION 3.29)
project(spo C)

set(CMAKE_C_STANDARD 11)

add_executable(spo stee.c
        stail.c
        sls.c
        schmod.c
        buf_write_benchmark.c
        side.c
        sterm.c
        monte_carlo_integral.c)
