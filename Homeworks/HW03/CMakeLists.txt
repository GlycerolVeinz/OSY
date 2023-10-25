cmake_minimum_required(VERSION 3.26)
project(HW03 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(nsd)

add_executable(HW03
        nsd/nd.c
        nsd/nd.h
        nsd/nsd.c
        nsd/nsd.h
        nsd/nsd_main.c
        forkpipe.c
        forkpipe.h)
