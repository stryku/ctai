cmake_minimum_required(VERSION 3.6)
project(ctai)

add_definitions(-std=c++1z -ftemplate-backtrace-limit=0)

set(INCLUDE_FILES
        register.hpp
        flags.hpp
        stack.hpp
        machine_state.hpp
        tuple.hpp
        static_warning.hpp
        traits.hpp
        string.hpp

        instructions/push.hpp
        instructions/instruction.hpp
        instructions/ids_vaules.hpp
        instructions/decode.hpp

        instructions/matchers/instructions_matchers.hpp

        execute/ex_push.hpp
        execute/ex_instruction.hpp
        execute/ex_add.hpp
        execute/ex_cmp.hpp
        execute/ex_mul.hpp
        execute/ex_pop.hpp
        execute/ex_sub.hpp
        execute/ex_mov.hpp
        execute.hpp

        tokenize/tokens.hpp
        tokenize/tokenizer.hpp


        assembler/assembler.hpp instructions/matchers/register_matcher.hpp)


set(SOURCE_FILES
        main.cpp instructions/matchers/push_matcher.hpp instructions/matchers/operand_decoder.hpp)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ctai
        ${INCLUDE_FILES}
        ${SOURCE_FILES})