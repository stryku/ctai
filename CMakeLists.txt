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
        assembler/label_extract.hpp

        memory/memory.hpp
        memory/memory_alloc.hpp
        memory/memory_metadata.hpp

        utils/empty_type.hpp
        utils/predicate.hpp

        tests/tests_values_container.hpp
        tests/tests_macros.hpp
        tests/tests_tuple.hpp
        tests/tests_memory_metadata.hpp utils/bad_value.hpp tests/tests_memory_alloc.hpp syscalls/sys_malloc.hpp syscalls/syscall.hpp tokenize/syscalls_tokens.hpp thread/thread.hpp thread/scheduler_round_robin.hpp containers/queue.hpp machine/machine_state.hpp execute/execute_thread.hpp execute/instructions/ex_push.hpp values/values.hpp execute/instructions/execute_instruction.hpp execute/execute_code.hpp kernel/thread.hpp declare_code.hpp instructions/matchers/sys_exit_thread_matcher.hpp instructions/matchers/call_matcher.hpp execute/instructions/sys/ex_sys_exit_thread.hpp execute/instructions/ex_call.hpp tests/instructions/matchers/tests_call_matcher.hpp tests/instructions/tests_instructions.hpp tests/instructions/matchers/tests_matchers.hpp tests/instructions/matchers/tests_sys_exit_thread_matcher.hpp instructions/matchers/sys_create_thread_matcher.hpp execute/instructions/sys/ex_sys_create_thread.hpp instructions/matchers/pusha_matcher.hpp tests/instructions/matchers/tests_pusha_matcher.hpp execute/instructions/ex_pusha.hpp instructions/matchers/popa_matcher.hpp execute/instructions/ex_popa.hpp tests/instructions/execute/tests_ex_pusha.hpp tests/instructions/execute/tests_execute.hpp tests/instructions/execute/tests_ex_popa.hpp tests/tests_memory.hpp instructions/matchers/ret_matcher.hpp execute/instructions/ex_mov.hpp execute/instructions/ex_pop.hpp utils/mem_reg_operations.hpp execute/instructions/ex_ret.hpp io/output.hpp io/io.hpp runtime/io.hpp execute/execution_result.hpp instructions/matchers/sys_write_matcher.hpp execute/instructions/sys/ex_sys_write.hpp kernel/io.hpp instructions/matchers/sys_is_thread_running_matcher.hpp execute/instructions/sys/ex_sys_is_thread_running.hpp instructions/matchers/jne_matcher.hpp execute/instructions/ex_jne.hpp execute/instructions/ex_cmp.hpp execute/instructions/ex_add.hpp io/input.hpp instructions/matchers/sys_read_matcher.hpp execute/instructions/sys/ex_sys_read.hpp kernel/utils.hpp execute/instructions/ex_jg.hpp instructions/matchers/jg_matcher.hpp instructions/matchers/jl_matcher.hpp execute/instructions/ex_jl.hpp execute/instructions/ex_jmp.hpp instructions/matchers/inc_matcher.hpp execute/instructions/ex_inc.hpp instructions/matchers/je_matcher.hpp instructions/matchers/sub_matcher.hpp execute/instructions/ex_sub.hpp)


set(SOURCE_FILES
        main.cpp tests/tests_main.hpp)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ctai
        ${INCLUDE_FILES}
        ${SOURCE_FILES})
