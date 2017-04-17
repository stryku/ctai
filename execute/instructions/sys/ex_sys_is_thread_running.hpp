#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "execute/eip_adjuster.hpp"
#include "thread/thread.hpp"
#include "machine/machine_state.hpp"
#include "io/output.hpp"

#include <cstddef>
#include <io/io.hpp>
#include "utils/empty_type.hpp"

namespace ctai
{
    namespace execute2
    {
        namespace details
        {
            template <size_t id>
            struct find_thread_predicate
            {
                template <typename thread_t>
                static constexpr auto value = (thread_t::id == id);
            };
        }

        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                inst::to_size<inst::id_t::SYS_IS_THREAD_RUNNING>,
                rest_of_opcodes...>
        {
        //using asd = typename machine_state_t::threads::qqqqqq;

//        using asd = typename thread_t::qqqqqq;


        static constexpr auto id = get_reg<typename thread_t::registers, regs::id_t::EAX>;
            using find_result = tuple_n::find_if<typename machine_state_t::threads,
                                                 details::find_thread_predicate<id>>;
            static constexpr auto is_running = !(std::is_same<find_result, ctai::utils::empty_type>::value);

            using retisters_after_eax = set_reg<typename thread_t::registers,
                                                regs::id_t::EAX,
                                                static_cast<uint32_t>(is_running)>;

            using final_registers = adjust_eip<retisters_after_eax, inst::id_t::SYS_IS_THREAD_RUNNING>;

            using result_thread = thread::set_registers<thread_t, final_registers>;

//                using asd = typename result_thread::qqqqqq;

        using result_machine_state = machine_state_t;
        };
    }
}
