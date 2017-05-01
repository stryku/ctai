#pragma once

#include "string.hpp"
#include "declare_code.hpp"

namespace ctai
{
    namespace include
    {
        using sys_malloc = decltype(
        ":sys_malloc "
                "sys_malloc "
                "ret "_s
        );

        using sys_free = decltype(
        ":sys_free "
                "sys_free "
                "ret "_s
        );

        using memory = ctai::declare_code<sys_malloc,
                                          sys_free>;
    }
}
