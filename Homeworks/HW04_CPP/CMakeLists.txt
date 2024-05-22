cmake_minimum_required(VERSION 3.26)
project(HW04_CPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW04_CPP prod-cons.cpp
        prod-cons.h
        utils.cpp
        utils.h)
