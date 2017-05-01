#pragma once

#include "ctai/containers/values_container.hpp"

namespace ctai
{
    namespace execute2
    {
        template<typename thread_t, typename memory_t, auto ...opcodes>
        struct ex_instruction;
    }
}

#include "ex_push.hpp"
#include "ex_pusha.hpp"
#include "ex_pop.hpp"
#include "ex_ret.hpp"
#include "ex_call.hpp"
#include "ex_popa.hpp"
#include "ex_mov.hpp"
#include "ex_jne.hpp"
#include "ex_je.hpp"
#include "ex_jg.hpp"
#include "ex_jl.hpp"
#include "ex_jmp.hpp"
#include "ex_inc.hpp"
#include "ex_cmp.hpp"
#include "ex_add.hpp"
#include "ex_mul.hpp"
#include "ex_sub.hpp"
#include "ex_div.hpp"
#include "ex_dec.hpp"
#include "ctai/execute/instructions/sys/ex_sys_exit_thread.hpp"
#include "ctai/execute/instructions/sys/ex_sys_create_thread.hpp"
#include "ctai/execute/instructions/sys/ex_sys_write.hpp"
#include "ctai/execute/instructions/sys/ex_sys_read.hpp"
#include "ctai/execute/instructions/sys/ex_sys_is_thread_running.hpp"
#include "ctai/execute/instructions/sys/ex_sys_malloc.hpp"
#include "ctai/execute/instructions/sys/ex_sys_free.hpp"
#include "ctai/execute/instructions/sys/ex_sys_try_lock_mutex.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template<typename thread_t,
                     typename machine_state_t,
                     typename opcodes>
            struct execute_instruction_impl;

            template <typename thread_t,
                      typename machine_state_t,
                      auto ...opcodes>
            struct execute_instruction_impl<thread_t,
                    machine_state_t,
                                            values_container_n::values_container<opcodes...>>
            {
                using result = ex_instruction<thread_t,
                        machine_state_t,
                                              opcodes...>;
            };
        }

        template<typename thread_t,
                 typename machine_state_t,
                 typename opcodes>
        using execute_instruction = typename details::execute_instruction_impl<thread_t,
                machine_state_t,
                                                                               opcodes>::result;
    }
}
