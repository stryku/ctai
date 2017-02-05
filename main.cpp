#include "string.hpp"
#include "execute.hpp"

#include <iostream>

namespace tests
{
    static_assert(
            cai::execute_code<
                    decltype(
                    "mov eax , 6 "
                            "exit"_s
                    )
            > == 6
            ,"");

    static_assert(
            cai::execute_code<
                    decltype("mov ebx , -6 "
                            "push ebx "
                            "pop eax "
                            "exit"_s
                    )
            > == -6
            ,"");

    static_assert(
            cai::execute_code<
                    decltype("mov ebp , esp "
                            "push ebx " // declare variable
                            "mov ebx , -6 "
                            "mov DWORD PTR [ ebp + 4 ] , ebx " //move ebx to the variable
                            "mov eax , DWORD PTR [ ebp + 4 ] " // move variable to eax
                            "exit"_s
                    )
            > == -6
            ,"");

    static_assert(
            cai::execute_code<
                    decltype("mov ebp , esp "
                            "push ebx " // declare variable
                            "mov eax , 22 " // declare variable
                            "mov ebx , 11 " // declare variable
                            "add ebx , eax "
                            "mov DWORD PTR [ ebp + 4 ] , ebx " //move 1 to the variable
                            "mov eax , DWORD PTR [ ebp + 4 ] " // move variable to eax
                            "exit"_s
                    )
            > == 33
            ,"");

    static_assert(
            cai::execute_code<
                    decltype(
                            "jmp .jump_here "
                            "mov eax , 12 "
                        ":jump_here "
                            "mov eax , 6 "
                            "exit"_s
                    )
            > == 6
            ,"");

    static_assert(
            cai::execute_code<
                    decltype(
                    "mov ecx , 10 "
                    "mov eax , 22 "
                            "mov ebx , 1 "
                            "mov edx , 10 "
                            ":loop "
                            "add edx , 1 "
                            "add eax , ebx "
                            "cmp ecx , edx "
                            "jge .loop "
                            "mov DWORD PTR [ esp + 0 ] , eax "
                            "mov eax , DWORD PTR [ esp + 0 ] "
                            "exit"_s
                    )
            > == 23
            ,"");

    static_assert(
            cai::execute_code<
                    decltype(
                    "mov ebp , esp "
                            "push eax " // declare four variables
                            "push eax "
                            "push eax "
                            "push eax "
                            "mov DWORD PTR [ ebp + 8 ] , 0 "
                            "mov DWORD PTR [ ebp + 12 ] , 1 "
                            "mov DWORD PTR [ ebp + 16 ] , 1 "
                            "mov DWORD PTR [ ebp + 4 ] , 1 "
                            ":loop_label "
                            "mov eax , DWORD PTR [ ebp + 4 ] "
                            "mov ebx , 6 "
                            "cmp eax , ebx "
                            "jge .end_label "
                            "mov edx , DWORD PTR [ ebp + 8 ] "
                            "mov eax , DWORD PTR [ ebp + 12 ] "
                            "add eax , edx "
                            "mov DWORD PTR [ ebp + 16 ] , eax "
                            "mov eax , DWORD PTR [ ebp + 12 ] "
                            "mov DWORD PTR [ ebp + 8 ] , eax "
                            "mov eax , DWORD PTR [ ebp + 16 ] "
                            "mov DWORD PTR [ ebp + 12 ] , eax "
                            "mov eax , DWORD PTR [ ebp + 4 ] "
                            "mov ebx , 1 "
                            "add eax , ebx "
                            "mov DWORD PTR [ ebp + 4 ] , eax "
                            "jmp .loop_label "
                            ":end_label "
                            "mov eax , DWORD PTR [ ebp + 16 ] "
                            "exit"_s
                    )
            > == 8
            ,"");
}

using code = decltype(
    "mov ebp , esp "
    "push eax " // declare four variables
    "push eax "
    "push eax "
    "push eax "
    "mov DWORD PTR [ ebp + 8 ] , 0 "
    "mov DWORD PTR [ ebp + 12 ] , 1 "
    "mov DWORD PTR [ ebp + 16 ] , 1 "
    "mov DWORD PTR [ ebp + 4 ] , 1 "
":loop_label "
    "mov eax , DWORD PTR [ ebp + 4 ] "
    "mov ebx , 10 " //we want to get 10th fibonacci
    "cmp eax , ebx "
    "jge .end_label "
    "mov edx , DWORD PTR [ ebp + 8 ] "
    "mov eax , DWORD PTR [ ebp + 12 ] "
    "add eax , edx "
    "mov DWORD PTR [ ebp + 16 ] , eax "
    "mov eax , DWORD PTR [ ebp + 12 ] "
    "mov DWORD PTR [ ebp + 8 ] , eax "
    "mov eax , DWORD PTR [ ebp + 16 ] "
    "mov DWORD PTR [ ebp + 12 ] , eax "
    "mov eax , DWORD PTR [ ebp + 4 ] "
    "mov ebx , 1 "
    "add eax , ebx "
    "mov DWORD PTR [ ebp + 4 ] , eax "
    "jmp .loop_label "
":end_label "
    "mov eax , DWORD PTR [ ebp + 16 ] "
    "exit"_s);
constexpr auto ret_val = cai::execute_code<code>;

int main()
{
    std::cout << ret_val;
    return 0;
}