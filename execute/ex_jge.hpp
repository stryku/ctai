#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"
#include "execute/eip_adjuster.hpp"

#include <type_traits>

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t ip, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::JGE>, ip, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto flags_v = typename state::flags_t{};

            static constexpr bool should_jmp = ((flags_v.SF ^ flags_v.OF) == 0);
            using final_regs_state = std::conditional_t<
                    should_jmp,
                    set_reg<typename state::registers_state_t, regs::id_t::EIP, static_cast<uint32_t>(ip)>,
                    adjust_eip<typename state::registers_state_t, inst::id_t::JGE>>;

            using type = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        namespace tests
        {
        }
    }
}