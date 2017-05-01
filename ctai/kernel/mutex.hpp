#pragma once

#include "ctai/string.hpp"
#include "ctai/declare_code.hpp"

namespace ctai
{
    namespace include
    {
        using lock_mutex = decltype(
        ":lock_mutex "
                "push eax "
                "push ebx "

                "mov ebx , eax "

            ":__mutex_lock_mutex_loop "
                "sys_try_lock_mutex "
                "cmp eax , 1 "
                "jne .__mutex_lock_mutex_loop " // todo sleep till end of turn

                "pop ebx "
                "pop eax "
                "ret "_s
        );

        using unlock_mutex = decltype(
        ":unlock_mutex "
                "mov BYTE PTR [ eax ] , 0 "
                "ret "_s
        );

        using mutex = ctai::declare_code<lock_mutex,
                                         unlock_mutex>;
    }
}
