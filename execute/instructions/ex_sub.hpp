#pragma once

#include "thread/thread.hpp"
#include "execute/eip_adjuster.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template<typename thread_t,
                     typename machine_state_t, size_t reg1, size_t val>
            struct ex_sub_impl
            {
                using registers = typename thread_t::registers;

                static constexpr auto val1 = get_reg<registers, regs::to_id<reg1>>;

                using new_regs_state = set_reg<registers, regs::to_id<reg1>, static_cast<uint32_t>(val1 - val)>;

                using final_regs_state = adjust_eip<new_regs_state, inst::id_t::SUB_REG_VAL>;

                using result_thread = thread::set_registers<thread_t, final_regs_state>;
                using result_machine_state = machine_state_t;
            };
        }

        template<typename thread_t,
                 typename machine_state_t, size_t reg1, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::SUB_REG_VAL>, reg1, val, rest_of_opcodes...>
        {
            using registers = typename thread_t::registers;

            using sub_result = details::ex_sub_impl<thread_t,
                                                    machine_state_t,
                                                    reg1,
                                                    val>;

            using result_thread = typename sub_result::result_thread;
            using result_machine_state = typename sub_result::result_machine_state;
        };

        template<typename thread_t,
                 typename machine_state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::SUB_REG_REG>, reg1, reg2, rest_of_opcodes...>
        {
            using registers = typename thread_t::registers;

            using sub_result = details::ex_sub_impl<thread_t,
                                                    machine_state_t,
                                                    reg1,
                                                    get_reg<registers, regs::to_id<reg2>>>;

            using result_thread = typename sub_result::result_thread;
            using result_machine_state = typename sub_result::result_machine_state;
        };
    }
}
