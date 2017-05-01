#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "execute/eip_adjuster.hpp"
#include "thread/thread.hpp"
#include "utils/operand_size.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <typename thread_t,
                      regs::id_t reg,
                      utils::operand_size operand_size>
            struct ex_mul_impl;

            template <typename thread_t,
                      regs::id_t reg>
            struct ex_mul_impl<thread_t,
                               reg,
                               utils::operand_size::SIZE_8>
            {
                using registers = typename thread_t::registers;

                static constexpr auto al_value = get_reg<registers, regs::id_t::AL>;
                static constexpr auto reg_value = get_reg<registers, reg>;
                static constexpr auto result = static_cast<uint16_t>(al_value) * static_cast<uint16_t>(reg_value);

                using result_registers = set_reg<registers, regs::id_t::AX, result>;
            };

            template <typename thread_t,
                    regs::id_t reg>
            struct ex_mul_impl<thread_t,
                               reg,
                               utils::operand_size::SIZE_16>
            {
                using registers = typename thread_t::registers;

                static constexpr auto ax_value = get_reg<registers, regs::id_t::AX>;
                static constexpr auto reg_value = get_reg<registers, reg>;
                static constexpr auto result = static_cast<uint32_t>(ax_value) * static_cast<uint32_t>(reg_value);

                using regs_state_after_ax = set_reg<registers, regs::id_t::AX, result & 0xffff>;
                using result_registers = set_reg<regs_state_after_ax, regs::id_t::DX, ((result & 0xffff0000) >> 16)>;
            };

            template <typename thread_t,
                    regs::id_t reg>
            struct ex_mul_impl<thread_t,
                               reg,
                               utils::operand_size::SIZE_32>
            {
                using registers = typename thread_t::registers;

                static constexpr auto eax_value = get_reg<registers, regs::id_t::EAX>;
                static constexpr auto reg_value = get_reg<registers, reg>;
                static constexpr auto result = static_cast<uint32_t>(eax_value) * static_cast<uint32_t>(reg_value);

                using regs_state_after_eax = set_reg<registers, regs::id_t::EAX, result & 0xffffffff>;
                using result_registers = set_reg<regs_state_after_eax, regs::id_t::EDX, ((result & 0xffffffff00000000) >> 32)>;
            };
        }

        template <typename thread_t,
                  typename machine_state_t,
                  size_t reg,
                  size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              machine_state_t,
                              inst::to_size<inst::id_t::MUL_REG>,
                              reg,
                              rest_of_opcodes...>
        {
            static constexpr auto reg_size = utils::reg_size<regs::to_id<reg>>;

            using registers_after_mul = typename details::ex_mul_impl<thread_t,
                                                                      regs::to_id<reg>,
                                                                      reg_size>::result_registers;

            using final_regs_state = adjust_eip<registers_after_mul, inst::id_t::MUL_REG>;

            using result_thread = thread::set_registers<thread_t, final_regs_state>;
            using result_machine_state = machine_state_t;
        };
    }
}