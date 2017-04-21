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