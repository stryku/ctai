#pragma once

#include "register.hpp"
#include "flags.hpp"

namespace cai
{
    template <typename stack_t, typename flags_t, typename registers_state_t>
    struct machine_state
    {
        static constexpr typename to_flags<flags_t> flags{};
        static constexpr typename to_register_state <registers_state_t> reg_state{};
    };
}
