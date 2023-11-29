cmake_minimum_required(VERSION 3.26)
project(tryNo2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(tryNo2
        mem.c
        mem.h
        LinkedList.c
        LinkedList.h
        LinkedListQueue.c
        LinkedListQueue.h
        prod-cons.c
        prod-cons.h
        utils_2.c
        utils_2.h)
