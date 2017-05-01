include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(INCLUDE_FILES
        register.hpp
        flags.hpp
        tuple.hpp
        traits.hpp
        string.hpp
        labels.hpp
        eip_change.hpp
        declare_code.hpp

        containers/values_container.hpp
        containers/queue.hpp

        execute/eip_adjust.hpp
        execute.hpp
        execute/execute_thread.hpp
        execute/execute_code.hpp
        execute/execution_result.hpp

        execute/instructions/ex_push.hpp
        execute/instructions/execute_instruction.hpp
        execute/instructions/sys/ex_sys_exit_thread.hpp
        execute/instructions/ex_call.hpp
        execute/instructions/sys/ex_sys_create_thread.hpp
        execute/instructions/ex_pusha.hpp
        execute/instructions/ex_popa.hpp
        execute/instructions/ex_mov.hpp
        execute/instructions/ex_pop.hpp
        execute/instructions/ex_ret.hpp
        execute/instructions/sys/ex_sys_write.hpp
        execute/instructions/sys/ex_sys_is_thread_running.hpp
        execute/instructions/ex_jne.hpp
        execute/instructions/ex_cmp.hpp
        execute/instructions/ex_add.hpp
        execute/instructions/sys/ex_sys_read.hpp
        execute/instructions/ex_jg.hpp
        execute/instructions/ex_jl.hpp
        execute/instructions/ex_jmp.hpp
        execute/instructions/ex_inc.hpp
        execute/instructions/ex_sub.hpp
        execute/instructions/ex_je.hpp
        execute/instructions/ex_mul.hpp
        execute/instructions/sys/ex_sys_malloc.hpp
        execute/instructions/sys/ex_sys_free.hpp
        execute/instructions/ex_div.hpp
        execute/instructions/ex_dec.hpp

        tokenize/tokens.hpp
        tokenize/tokenizer.hpp
        tokenize/syscalls_tokens.hpp

        assembler/assembler.hpp
        assembler/label_substitute.hpp
        assembler/label_extract.hpp

        instructions/ids_vaules.hpp

        instructions/matchers/instructions_matchers.hpp
        instructions/matchers/jmp_matcher.hpp
        instructions/matchers/jge_matcher.hpp
        instructions/matchers/cmp_matcher.hpp
        instructions/matchers/pop_matcher.hpp
        instructions/matchers/mov_matcher.hpp
        instructions/matchers/plus_minus_decoder.hpp
        instructions/matchers/mem_size_decoder.hpp
        instructions/matchers/add_matcher.hpp
        instructions/matchers/push_matcher.hpp
        instructions/matchers/operand_decoder.hpp
        instructions/matchers/sys_exit_thread_matcher.hpp
        instructions/matchers/call_matcher.hpp
        instructions/matchers/sys_create_thread_matcher.hpp
        instructions/matchers/pusha_matcher.hpp
        instructions/matchers/popa_matcher.hpp
        instructions/matchers/ret_matcher.hpp
        instructions/matchers/sys_write_matcher.hpp
        instructions/matchers/sys_is_thread_running_matcher.hpp
        instructions/matchers/jne_matcher.hpp
        instructions/matchers/sys_read_matcher.hpp
        instructions/matchers/jg_matcher.hpp
        instructions/matchers/jl_matcher.hpp
        instructions/matchers/inc_matcher.hpp
        instructions/matchers/je_matcher.hpp
        instructions/matchers/sub_matcher.hpp
        instructions/matchers/mul_matcher.hpp
        instructions/matchers/sys_malloc_matcher.hpp
        instructions/matchers/sys_free_matcher.hpp
        instructions/matchers/div_matcher.hpp
        instructions/matchers/dec_matcher.hpp
        instructions/matchers/sys_try_lock_mutex_matcher.hpp

        io/output.hpp
        io/io.hpp
        io/input.hpp

        kernel/io.hpp
        kernel/utils.hpp
        kernel/memory.hpp
        kernel/mutex.hpp opcode/opcodes.hpp
        kernel/thread.hpp

        memory/memory.hpp
        memory/memory_alloc.hpp
        memory/memory_metadata.hpp
        memory/memory_free.hpp

        utils/empty_type.hpp
        utils/predicate.hpp
        utils/bad_value.hpp
        utils/mem_reg_operations.hpp
        utils/operand_size.hpp

        tests/tests_values_container.hpp
        tests/tests_macros.hpp
        tests/tests_tuple.hpp
        tests/tests_memory_metadata.hpp
        tests/tests_memory_alloc.hpp
        tests/instructions/matchers/tests_call_matcher.hpp
        tests/instructions/tests_instructions.hpp
        tests/instructions/matchers/tests_matchers.hpp
        tests/instructions/matchers/tests_sys_exit_thread_matcher.hpp
        tests/instructions/matchers/tests_pusha_matcher.hpp
        tests/instructions/execute/tests_ex_pusha.hpp
        tests/instructions/execute/tests_execute.hpp
        tests/instructions/execute/tests_ex_popa.hpp
        tests/tests_memory.hpp
        tests/tests_main.hpp

        thread/thread.hpp
        thread/scheduler_round_robin.hpp

        machine/machine_state.hpp

        runtime/io.hpp
        values/values.hpp
        )


set(SOURCE_FILES
        main.cpp execute/instructions/sys/ex_sys_try_lock_mutex.hpp)

include_directories(..)

add_executable(ctai
        ${INCLUDE_FILES}
        ${SOURCE_FILES})
