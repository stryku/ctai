#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"
#include "execute/eip_adjuster.hpp"

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t ip, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::JMP>, ip, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            using final_regs_state = set_reg<typename state::registers_state_t, regs::id_t::EIP, static_cast<uint32_t>(ip)>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        namespace tests
        {
        }
    }
}