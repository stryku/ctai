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
        namespace details
        {
            template <typename thread_t, typename memory_t,
        }

        //create_thread
        template <typename thread_t, typename memory_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                              memory_t,
                              inst::to_size<inst::id_t::SYS_CREATE_THREAD>,
                              rest_of_opcodes...>
        {
            using result_thread = thread::thread<true,
                                                 thread_t::priority,
                                                 thread_t::id,
                                                 typename thread_t::registers,
                                                 typename thread_t::flags>;
            using result_memory = memory_t;
        };
    }
}
