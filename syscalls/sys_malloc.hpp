#pragma once

#include "syscalls/syscall.hpp"
#include "tokenize/syscalls_tokens.hpp"

namespace ctai
{
    namespace syscall
    {
        namespace details
        {
            template <typename machine_state_t>
            struct syscall_impl<machine_state_t, tokens::tok_sys_malloc>
            {

            };
        }
    }
}
