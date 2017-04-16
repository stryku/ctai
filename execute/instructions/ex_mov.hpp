#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "values/values.hpp"
#include "execute/eip_adjuster.hpp"
#include "thread/thread.hpp"

#include <cstddef>

namespace ctai
{
    namespace execute2
    {
        //mov reg, val
        template <typename thread_t, typename machine_state_t, size_t reg, size_t value, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::MOV_REG_VAL>,
                reg,
                value,
                rest_of_opcodes...>
        {
            using registers_after_mov = set_reg<typename thread_t::registers,
                    regs::to_id<reg>,
                    value>;

            using final_registers = adjust_eip<registers_after_mov, inst::id_t::MOV_REG_VAL>;

            using result_thread = thread::set_registers<thread_t, final_registers>;
            using result_machine_state = machine_state_t;
        };

        //mov reg, * PTR [ reg + val ]
        template <typename thread_t, typename machine_state_t, size_t reg, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const>,
                reg,
                mem_ptr_reg,
                mem_ptr_const,
                mem_size,
                rest_of_opcodes...>
        {
            static constexpr auto mem_reg_val = get_reg<typename thread_t::registers, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val + mem_ptr_const;
            static constexpr auto mem_val = memory::get<typename machine_state_t::memory,
                                                        memory::to_mem_type<memory::to_id<mem_size>>,
                                                        mem_ptr>;


    using registers_after_mov = set_reg<typename thread_t::registers,
                    regs::to_id<reg>,
            mem_val>;

            using final_registers = adjust_eip<registers_after_mov, inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const>;

            using result_thread = thread::set_registers<thread_t, final_registers>;
            using result_machine_state = machine_state_t;
        };
    }
}
