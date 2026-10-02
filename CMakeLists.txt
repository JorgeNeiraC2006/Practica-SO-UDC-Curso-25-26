cmake_minimum_required(VERSION 3.30)
project(P0 C)

set(CMAKE_C_STANDARD 99)

add_executable(P0 main.c
        commands.h
        p0.h
        p0.c
        dynamic_list.c
        dynamic_list.h
        file_list.c
        file_list.h
        p1.c
        p1.h)
