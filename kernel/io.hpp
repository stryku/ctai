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
                "mov ah , al " // move delimiter to ah

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

        using read_uint = decltype(
        ":read_uint "
                "push ebx "
                "push ecx "

                "sub esp , 11 " //space for read digits

                "mov ebx , esp " //pointer to store input
                "mov al , ' ' " //delimiter
                "call .getline "

                "mov ecx , eax " //move digits count
                "dec ecx " //omit NULL
                "mov eax , 0 "

            ":__io_rui_convert_loop "
                "cmp" //here. implement atoi

            ":__io_rui_finish "

                "add esp , 11 "

                "pop ecx "
                "pop ebx "
                "ret "_s
        )
    }
}
