#pragma once

#include "memory/memory.hpp"
#include "register.hpp"

namespace ctai
{
    namespace utils
    {
        namespace details
        {
            template <typename memory_t,
                    typename registers,
                    regs::id_t reg>
            struct pop_reg_32_impl
            {
                static constexpr auto esp = get_reg<registers, regs::id_t::ESP>;
                static constexpr auto next_esp = esp + 4;
                static constexpr auto value = memory::get_32<memory_t, esp>;
                using registers_after_esp = set_reg<registers, regs::id_t::ESP, next_esp>;
                using result_registers = set_reg<registers, reg, value>;
            };
        }

        template <typename memory_t,
                  typename registers,
                  regs::id_t reg>
        using pop_reg_32 = typename details::pop_reg_32_impl<memory_t, registers, reg>::result_registers;

    }
}
