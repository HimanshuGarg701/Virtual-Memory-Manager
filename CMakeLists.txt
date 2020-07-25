cmake_minimum_required(VERSION 3.5)
project(vmm)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(SOURCE_FILES vmm.c)
add_executable(vmm ${SOURCE_FILES})