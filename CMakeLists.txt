cmake_minimum_required(VERSION 3.5)
project(ctai)

add_definitions(-std=c++1z -ftemplate-backtrace-limit=0 -ftemplate-depth=1024)

set(CMAKE_CXX_COMPILER clang++-4.0)

set(INCLUDE_FILES
        register.hpp
        flags.hpp
        stack.hpp
        machine_state.hpp
        tuple.hpp
        static_warning.hpp
        traits.hpp
        string.hpp
        labels.hpp
        eip_change.hpp

        containers/values_container.hpp

        instructions/push.hpp
        instructions/instruction.hpp
        instructions/ids_vaules.hpp
        instructions/decode.hpp

        instructions/matchers/instructions_matchers.hpp
        instructions/matchers/jmp_matcher.hpp
        instructions/matchers/jge_matcher.hpp
        instructions/matchers/cmp_matcher.hpp
        instructions/matchers/register_matcher.hpp
        instructions/matchers/pop_matcher.hpp
        instructions/matchers/mov_matcher.hpp
        instructions/matchers/plus_minus_decoder.hpp
        instructions/matchers/mem_size_decoder.hpp
        instructions/matchers/add_matcher.hpp
        instructions/matchers/push_matcher.hpp
        instructions/matchers/operand_decoder.hpp

        execute/ex_push.hpp
        execute/ex_instruction.hpp
        execute/ex_add.hpp
        execute/ex_cmp.hpp
        execute/ex_mul.hpp
        execute/ex_pop.hpp
        execute/ex_sub.hpp
        execute/ex_mov.hpp
        execute/ex_jmp.hpp
        execute/ex_jge.hpp
        execute/eip_adjuster.hpp
        execute.hpp

        tokenize/tokens.hpp
        tokenize/tokenizer.hpp

        assembler/assembler.hpp
        assembler/label_substitute.hpp
        assembler/label_extract.hpp)


set(SOURCE_FILES
        main.cpp  )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ctai
        ${INCLUDE_FILES}
        ${SOURCE_FILES})
