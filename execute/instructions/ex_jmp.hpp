#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"
#include "execute/eip_adjuster.hpp"
#include "thread/thread.hpp"

#include <type_traits>

namespace ctai
{
    namespace execute2
    {
        template<typename thread_t, typename machine_state_t, size_t ip, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::JMP>, ip, rest_of_opcodes...>
        {
            using final_regs_state = set_reg<typename thread_t::registers, regs::id_t::EIP, static_cast<uint32_t>(ip)>;
            using result_thread = thread::set_registers<thread_t, final_regs_state>;
            using result_machine_state = machine_state_t;
        };
    }
}