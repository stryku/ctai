#pragma once

#include "string.hpp"

namespace ctai
{
    namespace include
    {
        using is_digit = decltype(
        ":is_digit "
                "cmp al , '0' "
                "jl .__uid_no "

                "cmp al , '9' "
                "jg .__uid_no "

                "mov al , 1 "
                "jmp .__uid_end "

                ":__uid_no "
                "mov al , 0 "

                ":__uid_end "
                "ret "_s
        );
    }
}