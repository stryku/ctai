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
                "mov ah , al " // move delimiter to ah

            ":__iog_read_loop "
                "sys_read "
                "cmp ah , al "
                "je .__iog_finish "
                "mov BYTE PTR [ ecx ] , al "
                "inc ecx "
                "jmp .__iog_read_loop "

            ":__iog_finish "
                "mov eax , ecx "

                "ret "_s
        );

        using read_uint = decltype(
        ":read_uint "
                "push ebx "
                "push ecx "
                "sub esp , 11 " //space for read digits

                "mov ebx , esp " //pointer to store input
                "mov al , ' ' " //getline delimiter
                "call .getline " //getline returns digits count in eax

                "mov eax , ebx " //pointer to ascii number
                "call .atoui " //value is in eax

                "add esp , 11 "
                "pop ecx "
                "pop ebx "
                "ret "_s
        );

        using io = declare_code<sys_write,
                                sys_read,
                                getline,
                                read_uint>;
    }
}
