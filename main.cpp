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
}

using code = decltype(
"push ebx "
"pop eax "
"exit"_s);
constexpr auto ret_val = cai::execute_code<code>;

int main()
{
    std::cout << std::hex << ret_val;
    return 0;
}