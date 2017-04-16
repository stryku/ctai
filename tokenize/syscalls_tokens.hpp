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
    }
}