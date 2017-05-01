#pragma once

#include "instructions/ids_vaules.hpp"
#include "ex_call.hpp"

#include <numeric>
#include <limits>
#include <thread/thread.hpp>

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <typename thread_t,
                      typename machine_state_t,
                      size_t val1,
                      size_t val2>
            struct ex_cmp_impl
            {
                using registers = typename thread_t::registers;
                static constexpr int64_t result = static_cast<int64_t>(val1) - static_cast<int64_t>(val2);

                static constexpr bool zf = (val1 == val2);
                static constexpr bool cf = (static_cast<uint64_t>(val1) < static_cast<uint64_t>(val2)); //set if borrow
                static constexpr bool sf = (result < 0);
                static constexpr bool of = (result < static_cast<int64_t>(std::numeric_limits<int32_t>::min()) ||
                                            result > static_cast<int64_t>(std::numeric_limits<int32_t>::max()));

                using new_flags_state = flags<cf, zf, sf, of>;

                using final_regs_state = adjust_eip<registers, inst::id_t::CMP_REG_VAL>;
                using thread_after_registers = thread::set_registers<thread_t, final_regs_state>;
                using result_thread = thread::set_flags<thread_after_registers, new_flags_state>;

                using result_machine_state = machine_state_t;
            };
        }

        //cmp reg, reg
        template<typename thread_t,
                 typename machine_state_t, size_t reg, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::CMP_REG_REG>, reg, reg2, rest_of_opcodes...>
        {
            using registers = typename thread_t::registers;
            using cmp_result = details::ex_cmp_impl<thread_t,
                                                    machine_state_t,
                                                    get_reg<registers, regs::to_id<reg>>,
                                                    get_reg<registers, regs::to_id<reg2>>>;

            using result_thread = typename cmp_result::result_thread;
            using result_machine_state = typename cmp_result::result_machine_state;
        };

        //cmp reg, val
        template<typename thread_t,
                 typename machine_state_t, size_t reg, size_t value_v, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t, machine_state_t, inst::to_size<inst::id_t::CMP_REG_VAL>, reg, value_v, rest_of_opcodes...>
        {
            using cmp_result = details::ex_cmp_impl<thread_t,
                                                    machine_state_t,
                                                    get_reg<typename thread_t::registers, regs::to_id<reg>>,
                                                    value_v>;

            using result_thread = typename cmp_result::result_thread;
            using result_machine_state = typename cmp_result::result_machine_state;
        };
    }
}