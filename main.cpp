#include "string.hpp"
#include "execute.hpp"

#include <iostream>

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
    "mov ebx , 15 " //we want to get 15th fibonacci element
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

int main()
{
    std::cout << "15th element of fibonacci series is: " << ctai::execute_code<code>;
    return 0;
}

#include "tests/tests_main.hpp"