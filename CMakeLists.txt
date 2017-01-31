cmake_minimum_required(VERSION 3.6)
project(ctai)

add_definitions(-std=c++1z)

set(SOURCE_FILES
        main.cpp register.hpp flags.hpp stack.hpp)
add_executable(ctai ${SOURCE_FILES})