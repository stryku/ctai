#pragma once

#include "string.hpp"

namespace ctai
{
    namespace include
    {
        using atoui = decltype(
            ":atoui "
                    "push ebx "
                    "push ecx "
                    "mov ecx , eax " //string ptr
                    "mov eax , 0 "

                ":__utils_atoui_convert_loop "
                    "mov bl , BYTE PTR [ ecx ] " //get current char

                    "cmp bl , 0 "
                    "je .__utils_atoui_end " //jump to end if NULL

                    "sub bl , '0' " //ascii to value
                    "add eax , bl " //add to current value

                    "inc ecx "//move to next character

                    "jmp .__utils_atoui_convert_loop "


                ":__utils_atoui_end "
                    "pop ecx "
                    "pop ebx "
                    "ret "_s
        );

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