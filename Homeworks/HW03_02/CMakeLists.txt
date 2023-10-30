cmake_minimum_required(VERSION 3.26)
project(HW03_02 C)

set(CMAKE_C_STANDARD 99)

add_executable(HW03_02 forkpipe.c
        gen.c
        nsd_p.c
        parent.c
        util.c
)

target_compile_options(HW03_02 PRIVATE -Wall)

add_executable(nsd.exe nsd/nsd_main.c
        nsd/nd.c
        nsd/nsd.c
)

target_compile_options(nsd.exe PRIVATE -Wall)
