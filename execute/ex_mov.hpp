#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "execute/eip_adjuster.hpp"
#include "instructions/ids_vaules.hpp"

namespace ctai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_REG>, reg1, reg2, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>; // get reg2 value

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val2)>; //move register to register
            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_REG>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        template<typename state_t, size_t reg1, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_VAL>, reg1, val, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val)>; //move register to register
            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_VAL>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };


        // mov * ptr [mem_ptr_reg - mem_ptr_const], val
        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const>, mem_ptr_reg, mem_ptr_const, mem_size, val, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val - mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };

        // mov * ptr [mem_ptr_reg + mem_ptr_const], val
        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>, mem_ptr_reg, mem_ptr_const, mem_size, val, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val + mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };

        // mov * ptr [mem_ptr_reg + mem_ptr_const], reg
        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const>, mem_ptr_reg, mem_ptr_const, mem_size, reg, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto mem_ptr = mem_reg_val - mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(reg_val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };

        // mov * ptr [mem_ptr_reg + mem_ptr_const], reg
        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>, mem_ptr_reg, mem_ptr_const, mem_size, reg, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto mem_ptr = mem_reg_val + mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(reg_val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };

        // mov reg, * ptr [mem_ptr_reg - mem_ptr_const]
        template<typename state_t, size_t reg, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>, reg, mem_ptr_reg, mem_ptr_const, mem_size, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val - mem_ptr_const;
            static constexpr auto mem_val = stack_get<memory::to_mem_type<memory::to_id<mem_size>>, mem_ptr, typename state::stack_t>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg>, static_cast<uint32_t>(mem_val)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        // mov reg, * ptr [mem_ptr_reg - mem_ptr_const]
        template<typename state_t, size_t reg, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const>, reg, mem_ptr_reg, mem_ptr_const, mem_size, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val + mem_ptr_const;
            static constexpr auto mem_val = stack_get<memory::to_mem_type<memory::to_id<mem_size>>, mem_ptr, typename state::stack_t>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg>, static_cast<uint32_t>(mem_val)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        //
        // TESTS
        //
        namespace tests
        {
            static_assert(get_reg<ex_instruction<startup_machine_state,
                                  inst::to_size<inst::id_t::MOV_REG_REG>,
                                  regs::to_size<regs::id_t::AL>,
                                  regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                                  regs::id_t::AL> == static_cast<uint8_t>(0xcc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::AX>>::next_machine_state::registers_state_t,
                          regs::id_t::AL> == static_cast<uint8_t>(0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::EAX>>::next_machine_state::registers_state_t,
                          regs::id_t::AL> == static_cast<uint8_t>(0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                          regs::id_t::EAX> == static_cast<uint32_t>(0xaabbcccc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AX>,
                          regs::to_size<regs::id_t::AL>>::next_machine_state::registers_state_t,
                          regs::id_t::AX> == static_cast<uint32_t>(0xdd)
                    ,"");

            using mov_tests_machine_state = machine_state<startup_stack, startup_flags_state, registers_state<6, 0xaabbccdd,0,0,0,0,0,0, 0>>;


            //mov byte ptr [eax - 1], 0xff ; eax = 6
            static_assert(stack_get<uint8_t,
                                    5,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_8>,
                                                   0xfa>::next_machine_state::stack_t> == 0xfa
                    ,"");

            //mov byte ptr [eax + 1], 0xfa ; eax == 6
            static_assert(stack_get<uint8_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_8>,
                                                   0xfa>::next_machine_state::stack_t> == 0xfa
                    ,"");

            //mov word ptr [eax + 1], 0xfafb ; eax == 6
            static_assert(stack_get<uint16_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_16>,
                                                   0xfafb>::next_machine_state::stack_t> == 0xfafb
                    ,"");

            //mov dword ptr [eax + 1], 0xfafbfcfd ; eax == 6
            static_assert(stack_get<uint32_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_32>,
                                                   0xfafbfcfd>::next_machine_state::stack_t> == 0xfafbfcfd
                    ,"");



            //mov dword ptr [eax - 1], ebx ; eax == 6, ebx = 0xaabbccdd
            static_assert(stack_get<uint32_t,
                                    5,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_32>,
                                                   regs::to_size<regs::id_t::EBX>>::next_machine_state::stack_t> == 0xaabbccdd
                    ,"");

            //mov dword ptr [eax + 1], ebx ; eax == 6, ebx = 0xaabbccdd
            static_assert(stack_get<uint32_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_32>,
                                                   regs::to_size<regs::id_t::EBX>>::next_machine_state::stack_t> == 0xaabbccdd
                    ,"");

            //mov eax, dword ptr [eax - 1] ; eax == 6
            static_assert(get_reg<ex_instruction<mov_tests_machine_state,
                                                 inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 1,
                                                 memory::to_size<memory::id_t::s_32>>::next_machine_state::registers_state_t,
                                                 regs::id_t::EAX> == 0xaabbccdd
                    ,"");

            //mov eax, dword ptr [eax + 1] ; eax == 6
            static_assert(get_reg<ex_instruction<mov_tests_machine_state,
                                                 inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 1,
                                                 memory::to_size<memory::id_t::s_32>>::next_machine_state::registers_state_t,
                                                 regs::id_t::EAX> == 0x8899aabb
                    ,"");
        }
    }
}