#pragma once

#include "string.hpp"

namespace ctai
{
    namespace tokens
    {
        using tok_sys_malloc = decltype("sys_malloc"_s);
        using tok_sys_free = decltype("sys_free"_s);
        using tok_sys_exit_thread = decltype("sys_exit_thread"_s);
        using tok_sys_create_thread = decltype("sys_create_thread"_s);
        using tok_sys_write = decltype("sys_write"_s);
        using tok_sys_read = decltype("sys_read"_s);
        using tok_sys_is_thread_running = decltype("sys_is_thread_running"_s);
        using tok_sys_try_lock_mutex = decltype("sys_try_lock_mutex"_s);
    }
}