#pragma once

#include "string.hpp"

namespace ctai
{
    namespace include
    {
        using sys_write = decltype(
        ":sys_write "
                "sys_write "
                "ret "_s);

        using sys_read = decltype(
        ":sys_read "
                "sys_read "
                "ret "_s);

        using getline = decltype(
        ":getline "
                "push ecx "

                "mov ecx , 0 " //chars counter
                "mov ah , al " // move delimeter to ah

            ":__iog_read_loop "
                "sys_read "
                "cmp ah , al "
                "je .__iog_finish "
                "mov BYTE PTR [ ebx + ecx ] , al "
                "inc ecx "
                "jmp __iog_read_loop "

            ":__iog_finish "
                "mov eax , ecx "

                "pop ecx "

                "ret "_s
        );
    }
}
