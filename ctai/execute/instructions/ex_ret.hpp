#pragma once

#include "ctai/instructions/ids_vaules.hpp"
#include "ctai/register.hpp"
#include "ctai/memory/memory.hpp"
#include "values/values.hpp"
#include "ctai/execute/eip_adjust.hpp"
#include "ctai/thread/thread.hpp"
#include "ctai/utils/mem_reg_operations.hpp"

namespace ctai
{
    namespace execute2
    {
        template <typename thread_t,
                typename machine_state_t,
                size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::RET>,
                rest_of_opcodes...>
        {

            using next_registers = utils::pop_reg_32<typename machine_state_t::memory,
                                                     typename thread_t::registers,
                                                     regs::id_t::EIP>;
            using result_thread = thread::thread<thread_t::finished,
                    thread_t::id,
                    thread_t::priority,
                    next_registers,
                    typename thread_t::flags>;

            using result_machine_state = machine_state_t;
        };
    }
}