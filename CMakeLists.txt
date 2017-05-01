cmake_minimum_required(VERSION 3.5)
project(ctai)

add_definitions(-std=c++1z -ftemplate-backtrace-limit=0 -ftemplate-depth=8096)
set(CMAKE_CXX_COMPILER clang++-4.0)

add_subdirectory(ctai)