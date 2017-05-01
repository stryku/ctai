#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "eip_change.hpp"

#include <cstddef>
#include <tuple.hpp>

namespace ctai
{
    namespace details
    {
        template <typename regs_state_t, inst::id_t instruction_opcode>
        struct adjust_eip_impl
        {
            static constexpr auto eip_val = get_reg<regs_state_t, regs::id_t::EIP> + eip_change<instruction_opcode>;
            using regs_state = set_reg<regs_state_t, regs::id_t::EIP, eip_val>;
        };
    }

    template <typename regs_state_t, inst::id_t instruction>
    using adjust_eip = typename details::adjust_eip_impl<regs_state_t, instruction>::regs_state;
}
