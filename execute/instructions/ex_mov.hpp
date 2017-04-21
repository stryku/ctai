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

        //mov reg, * PTR [ reg ]
        template <typename thread_t, typename machine_state_t, size_t reg, size_t mem_ptr_reg, size_t mem_size, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              machine_state_t,
                              inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg>,
                              reg,
                              mem_ptr_reg,
                              mem_size,
                              rest_of_opcodes...>
        {
            static constexpr auto mem_reg_val = get_reg<typename thread_t::registers, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val ;
            static constexpr auto mem_val = memory::get<typename machine_state_t::memory,
                                                        memory::to_mem_type<memory::to_id<mem_size>>,
                                                        mem_ptr>;

            using registers_after_mov = set_reg<typename thread_t::registers,
                    regs::to_id<reg>,
                    mem_val>;

            using final_registers = adjust_eip<registers_after_mov, inst::id_t::MOV_REG_MEM__mem_eq_reg>;

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

        //mov * PTR [ reg ] , reg
        template <typename thread_t, typename machine_state_t, size_t reg, size_t mem_ptr_reg, size_t mem_size, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              machine_state_t,
                              inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg>,
                              mem_ptr_reg,
                              mem_size,
                              reg,
                              rest_of_opcodes...>
        {
            static constexpr auto value = get_reg<typename thread_t::registers, regs::to_id<reg>>;
            static constexpr auto mem_reg_val = get_reg<typename thread_t::registers, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val;
            using memory_after_set = memory::set<typename machine_state_t::memory,
                                                 memory::to_mem_type<memory::to_id<mem_size>>,
                                                 mem_ptr,
                                                 value>;

            using final_registers = adjust_eip<typename thread_t::registers, inst::id_t::MOV_MEM_REG__mem_eq_reg>;

            using result_thread = thread::set_registers<thread_t, final_registers>;
            using result_machine_state = machine::set_memory<machine_state_t, memory_after_set>;
        };

        //mov reg, reg
        template <typename thread_t, typename machine_state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::MOV_REG_REG>,
                reg1,
                reg2,
                rest_of_opcodes...>
        {
            static constexpr auto reg2_val = get_reg<typename thread_t::registers, regs::to_id<reg2>>;
            using registers_after_mov = set_reg<typename thread_t::registers,
                    regs::to_id<reg1>,
                    reg2_val>;

            using final_registers = adjust_eip<registers_after_mov, inst::id_t::MOV_REG_REG>;

            using result_thread = thread::set_registers<thread_t, final_registers>;
            using result_machine_state = machine_state_t;
        };
    }
}
