#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"
#include "memory/memory.hpp"
#include "values/values.hpp"
#include "execute/eip_adjust.hpp"

#include <cstddef>
#include <thread/thread.hpp>

namespace ctai
{
    namespace execute2
    {
        //exit_thread
        template <typename thread_t, typename machine_state_t, size_t ...rest_of_opcodes>
        struct ex_instruction<thread_t,
                machine_state_t,
                              inst::to_size<inst::id_t::SYS_EXIT_THREAD>,
                              rest_of_opcodes...>
        {
            using result_thread = thread::thread<true,
                                                 thread_t::priority,
                                                 thread_t::id,
                                                 typename thread_t::registers,
                                                 typename thread_t::flags>;
            using result_machine_state = machine_state_t;
        };
    }
}
