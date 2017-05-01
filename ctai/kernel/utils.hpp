#pragma once

#include "ctai/string.hpp"

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
                "push ebx "
                "push ecx "
                "push edx "
                "push esi "
                "push edi "

                //ebx - pointer to buffer

                "mov ecx , eax " //ecx - value to convert
                "mov esi , 10 " //divide by 10 to get chars
                "mov edi , 0 " //count digits

            ":__utils_uitoa_convert_loop "
                "mov edx , 0 " //prepare edx do div
                "div esi "
                //now eax = current_value / 10
                //now edx = current_value % 10

                "add dl , '0' " //value to ascii
                "mov BYTE PTR [ ebx ] , dl " //store in buffer
                "inc ebx "
                "inc edi "

                //check if it's end of conversion
                "cmp eax , 0 "
                "jne .__utils_uitoa_convert_loop " //jump if not

                //if we are here, it's end of the conversion
                "mov BYTE PTR [ ebx ] , 0 " //add NULL at the end



                //no we need to reverse the string
                "mov eax , ebx "
                "sub eax , edi " // eax - pointer to string
                "mov ebx , edi "
                "dec ebx " //ebx - digits count
                "call .mem_reverse "

                "pop edi "
                "pop esi "
                "pop edx "
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

        using mem_reverse = decltype(
        ":mem_reverse "
                "push eax "
                "push ebx "
                "push ecx "

                "add ebx , eax " //ebx - pointer to end

            ":__utils_mem_reverse_loop "
                "mov cl , BYTE PTR [ eax ] "
                "mov ch , BYTE PTR [ ebx ] "

                "mov BYTE PTR [ eax ] , ch "
                "mov BYTE PTR [ ebx ] , cl "

                "inc eax "
                "dec ebx "

                "cmp eax , ebx "
                "jl .__utils_mem_reverse_loop "


                "pop ecx "
                "pop ebx "
                "pop eax "
                "ret "_s
        );

        using utils = declare_code<atoui,
                                   uitoa,
                                   is_digit,
                                   mem_reverse>;
    }
}