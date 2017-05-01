#pragma once

#include "ctai/register.hpp"
#include "ctai/traits.hpp"

#include <cstddef>

namespace ctai
{
    namespace utils
    {
        enum class operand_size
        {
            SIZE_8,
            SIZE_16,
            SIZE_32
        };

        template <regs::id_t reg>
        constexpr auto reg_size = is_value_in_v<reg, regs::id_t::AL, regs::id_t::AH, regs::id_t::BL, regs::id_t::BH, regs::id_t::CL, regs::id_t::CH, regs::id_t::DL, regs::id_t::DH>
                                  ? operand_size::SIZE_8
                                  : is_value_in_v<reg, regs::id_t::AX, regs::id_t::BX, regs::id_t::CX, regs::id_t::DX, regs::id_t::SI, regs::id_t::SP, regs::id_t::BP, regs::id_t::DI>
                                    ? operand_size::SIZE_16
                                    : operand_size::SIZE_32;
    }
}
