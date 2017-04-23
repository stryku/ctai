#pragma once

#include "containers/values_container.hpp"

namespace ctai
{
    namespace execute2
    {
        template<typename thread_t, typename memory_t, auto ...opcodes>
        struct ex_instruction;
    }
}

#include "execute/instructions/ex_push.hpp"
#include "execute/instructions/ex_pusha.hpp"
#include "execute/instructions/ex_pop.hpp"
#include "execute/instructions/ex_ret.hpp"
#include "execute/instructions/ex_call.hpp"
#include "execute/instructions/ex_popa.hpp"
#include "execute/instructions/ex_mov.hpp"
#include "execute/instructions/ex_jne.hpp"
#include "execute/instructions/ex_je.hpp"
#include "execute/instructions/ex_jg.hpp"
#include "execute/instructions/ex_jl.hpp"
#include "execute/instructions/ex_jmp.hpp"
#include "execute/instructions/ex_inc.hpp"
#include "execute/instructions/ex_cmp.hpp"
#include "execute/instructions/ex_add.hpp"
#include "execute/instructions/ex_mul.hpp"
#include "execute/instructions/ex_sub.hpp"
#include "execute/instructions/ex_div.hpp"
#include "execute/instructions/ex_dec.hpp"
#include "execute/instructions/sys/ex_sys_exit_thread.hpp"
#include "execute/instructions/sys/ex_sys_create_thread.hpp"
#include "execute/instructions/sys/ex_sys_write.hpp"
#include "execute/instructions/sys/ex_sys_read.hpp"
#include "execute/instructions/sys/ex_sys_is_thread_running.hpp"
#include "execute/instructions/sys/ex_sys_malloc.hpp"
#include "execute/instructions/sys/ex_sys_free.hpp"

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
