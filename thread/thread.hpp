#pragma once

#include "containers/values_container.hpp"
#include "register.hpp"
#include "flags.hpp"

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
        using get_next_opcodes = typename details::get_next_opcodes_impl<thread_t, all_opcodes>::result;

        //
        //create
        //
        namespace details
        {
            template <size_t priority, size_t id, uint32_t eip, uint32_t esp>
            struct create_impl
            {
                using registers = registers_state<0,   //eax
                                                  0,   //ebx
                                                  0,   //ecx
                                                  0,   //edx
                                                  esp, //esp
                                                  0,
                                                  0,
                                                  0,
                                                  eip>;//eip

                using flags_t = startup_flags_state;

                using result = thread<false,
                                      id,
                                      priority,
                                      registers,
                                      flags_t>;
            };
        }

        template <size_t priority, size_t id, uint32_t eip, uint32_t esp>
        using create = typename details::create_impl<priority, id, eip, esp>::result;

        template <typename thread_t, typename registers>
        using set_registers = thread<thread_t::finished,
                                     thread_t::id,
                                     thread_t::priority,
                                     registers,
                                     typename thread_t::flags>;

        template <typename thread_t, typename flags_t>
        using set_flags = thread<thread_t::finished,
                thread_t::id,
                thread_t::priority,
                typename thread_t::registers,
                flags_t>;
    }
}
