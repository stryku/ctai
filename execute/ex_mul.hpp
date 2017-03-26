#pragma once

#include "machine_state.hpp"
#include "instructions/instruction.hpp"
#include "execute/ex_instruction.hpp"
#include "instructions/ids_vaules.hpp"

namespace cai
{
    namespace execute
    {
        // mul al, ...
        template<typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MUL_REG_REG>, regs::to_size<regs::id_t::AL>, reg>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto al_val = get_reg<typename state::registers_state_t, regs::id_t::AL>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto result = static_cast<uint16_t>(al_val) * static_cast<uint16_t>(reg_val);

            using new_regs_state = set_reg<typename state::registers_state_t, regs::id_t::AX, static_cast<uint32_t>(result)>; //move value to register

            static constexpr bool cf = ((result & 0xff00) != 0);

            using new_flags_state = set_cf<typename state::flags_t, cf>;

            using next_machine_state = machine_state<typename state::stack_t, new_flags_state, new_regs_state>;
        };

        // mul ax, ...
        template<typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MUL_REG_REG>, regs::to_size<regs::id_t::AX>, reg>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto ax_val = get_reg<typename state::registers_state_t, regs::id_t::AX>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto result = static_cast<uint32_t>(ax_val) * static_cast<uint32_t>(reg_val);

            using regs_state_after_ax = set_reg<typename state::registers_state_t, regs::id_t::AX, result & 0xffff>;
            using regs_state_after_dx = set_reg<regs_state_after_ax, regs::id_t::DX, ((result & 0xffff0000) >> 16)>;

            static constexpr bool cf = ((result & 0xffff0000) != 0);

            using new_flags_state = set_cf<typename state::flags_t, cf>;

            using next_machine_state = machine_state<typename state::stack_t, new_flags_state, regs_state_after_dx>;
        };

        // mul eax, ...
        template<typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MUL_REG_REG>, regs::to_size<regs::id_t::EAX>, reg>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto eax_val = get_reg<typename state::registers_state_t, regs::id_t::EAX>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto result = static_cast<uint64_t>(eax_val) * static_cast<uint64_t>(reg_val);

            using regs_state_after_eax = set_reg<typename state::registers_state_t, regs::id_t::EAX, result & 0xffffffff>;
            using regs_state_after_edx = set_reg<regs_state_after_eax, regs::id_t::EDX, ((result & 0xffffffff00000000) >> 32)>;

            static constexpr bool cf = ((result & 0xffff0000) != 0);

            using new_flags_state = set_cf<typename state::flags_t, cf>;

            using next_machine_state = machine_state<typename state::stack_t, new_flags_state, regs_state_after_edx>;
        };

        namespace tests
        {

            using mul_tests_startup_machine_state = machine_state<startup_stack, startup_flags_state, registers_state<0x00000002, 0x00000102, 0x000000ff, 0xffffffff,0,0,0,0, 0>>;

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                                  regs::to_size<regs::id_t::AL>,
                                  regs::to_size<regs::id_t::BL>>::next_machine_state::registers_state_t,
                                  regs::id_t::AX> == static_cast<uint16_t>(0x02 * 0x02)
                    ,"");

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                          regs::to_size<regs::id_t::AX>,
                          regs::to_size<regs::id_t::BX>>::next_machine_state::registers_state_t,
                          regs::id_t::AX> == static_cast<uint16_t>(0x0002 * 0x0102)
                    ,"");

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                          regs::to_size<regs::id_t::EAX>,
                          regs::to_size<regs::id_t::EDX>>::next_machine_state::registers_state_t,
                          regs::id_t::EAX> == static_cast<uint32_t>(0x00000002 * 0xffffffff)
                    ,"");

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                          regs::to_size<regs::id_t::EAX>,
                          regs::to_size<regs::id_t::EDX>>::next_machine_state::registers_state_t,
                          regs::id_t::EDX> == static_cast<uint32_t>(((static_cast<uint64_t>(0x00000002) * static_cast<uint64_t>(0xffffffff)) & 0xffffffff00000000) >> 32)
                    ,"");

        }
    }
}