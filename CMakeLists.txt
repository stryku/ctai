cmake_minimum_required(VERSION 3.6)
project(ctai)

add_definitions(-std=c++1z)

set(INCLUDE_FILES
        register.hpp
        flags.hpp
        stack.hpp
        machine_state.hpp
        instructions/push.hpp
        instructions/instruction.hpp
        instructions/ids_vaules.hpp
        instructions/decode.hpp execute/ex_push.hpp execute/ex_instruction.hpp)

set(SOURCE_FILES main.cpp static_warning.hpp traits.hpp execute/ex_pop.hpp)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ctai
        ${INCLUDE_FILES}
        ${SOURCE_FILES})