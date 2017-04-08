#pragma once

#include "containers/values_container.hpp"
#include "register.hpp"

#include <cstddef>

namespace ctai
{
    namespace thread
    {
        template <bool finished_v,
                  size_t id_v,
                  size_t priority_v,
                  typename registers_t,
                  typename flags_t>
        struct thread
        {
            static constexpr auto id = id_v;
            static constexpr auto priority = priority_v;
            static constexpr auto finished = finished_v;
            using registers = registers_t;
            using flags = flags_t;
        };

        //
        //get_next_opcodes
        //
        namespace details
        {
            template <typename thread_t, typename all_opcodes>
            struct get_next_opcodes_impl
            {
                static constexpr auto eip = get_reg<typename thread_t::registers, regs::id_t::EIP>;

                using result = values_container_n::drop_front<all_opcodes, eip>;
            };
        }

        template <typename thread_t, typename all_opcodes>
        using get_next_opcodes = typename details::get_next_opcodes_impl<thread_t, all_opcodes>::result
    }
}
