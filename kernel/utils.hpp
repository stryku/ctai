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
                    "push edx "
                    "push esi "

                    "mov ecx , eax " //string ptr
                    "mov eax , 10 "
                    "mov esi , 0 " //current value will be in esi

                ":__utils_atoui_convert_loop "
                    "mov bl , BYTE PTR [ ecx ] " //get current char

                    "cmp bl , 0 "
                    "je .__utils_atoui_end " //jump to end if NULL

                    "sub bl , '0' " //ascii to value

                    "cmp esi , 0 " //if current value is > 0 we need to mul value by 10
                    "je .__utils_atoui_no_mul " //if it's 0 don't mul
                    "mul esi "
                    "mov esi , eax "
                    "mov eax , 10 "

                ":__utils_atoui_no_mul "
                    "add esi , bl " //add to current value

                    "inc ecx "//move to next character

                    "jmp .__utils_atoui_convert_loop "

                ":__utils_atoui_end "
                    "mov eax , esi "

                    "pop esi "
                    "pop edx "
                    "pop ecx "
                    "pop ebx "
                    "ret "_s
        );

        using uitoa = decltype(
        ":uitoa "
                ""
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