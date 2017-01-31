cmake_minimum_required(VERSION 3.6)
project(ctai)

add_definitions(-std=c++1z)

set(SOURCE_FILES
        main.cpp register.hpp)
add_executable(repo ${SOURCE_FILES})