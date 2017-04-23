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
            struct ex_div_impl;

            template <typename thread_t,
                      regs::id_t reg>
            struct ex_div_impl<thread_t,
                               reg,
                               utils::operand_size::SIZE_8>
            {
                using registers = typename thread_t::registers;

                static constexpr auto ax_value = get_reg<registers, regs::id_t::AX>;
                static constexpr auto reg_value = get_reg<registers, reg>;

                using registers_after_al = set_reg<registers, regs::id_t::AL, ax_value / reg_value>;
                using result_registers = set_reg<registers_after_al, regs::id_t::AH, ax_value % reg_value>;
            };

            template <typename thread_t,
                    regs::id_t reg>
            struct ex_div_impl<thread_t,
                               reg,
                               utils::operand_size::SIZE_16>
            {
                using registers = typename thread_t::registers;

                static constexpr auto ax_value = get_reg<registers, regs::id_t::AX>;
                static constexpr auto dx_value = get_reg<registers, regs::id_t::DX>;
                static constexpr auto reg_value = get_reg<registers, reg>;
                static constexpr auto value_to_div = (static_cast<uint32_t>(dx_value) << 16 ) + static_cast<uint32_t>(ax_value);

                using registers_after_ax = set_reg<registers, regs::id_t::AX, value_to_div / reg_value>;
                using result_registers = set_reg<registers_after_ax, regs::id_t::DX, value_to_div % reg_value>;
            };

            template <typename thread_t,
                    regs::id_t reg>
            struct ex_div_impl<thread_t,
                               reg,
                               utils::operand_size::SIZE_32>
            {
                using registers = typename thread_t::registers;

                static constexpr auto eax_value = get_reg<registers, regs::id_t::EAX>;
                static constexpr auto edx_value = get_reg<registers, regs::id_t::EDX>;
                static constexpr auto reg_value = get_reg<registers, reg>;
                static constexpr auto value_to_div = (static_cast<uint64_t>(edx_value) << 32 ) + static_cast<uint64_t>(eax_value);

                using registers_after_eax = set_reg<registers, regs::id_t::EAX, value_to_div / reg_value>;
                using result_registers = set_reg<registers_after_eax, regs::id_t::EDX, value_to_div % reg_value>;
            };
        }

        template <typename thread_t,
                  typename machine_state_t,
                  size_t reg,
                  size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              machine_state_t,
                              inst::to_size<inst::id_t::DIV_REG>,
                              reg,
                              rest_of_opcodes...>
        {
            static constexpr auto reg_size = utils::reg_size<regs::to_id<reg>>;

            using registers_after_div = typename details::ex_div_impl<thread_t,
                                                                      regs::to_id<reg>,
                                                                      reg_size>::result_registers;

            using final_regs_state = adjust_eip<registers_after_div, inst::id_t::DIV_REG>;

            using result_thread = thread::set_registers<thread_t, final_regs_state>;
            using result_machine_state = machine_state_t;
        };
    }
}