compile-time (simplified) x86 assembly interpreter

Hello World example:

```
#include "string.hpp"
#include "execute.hpp"

#include <iostream>

using code = decltype("push ebx pop eax exit"_s);
constexpr auto ret_val = cai::execute_code<code>;

int main()
{
    std::cout << std::hex << ret_val;
    return 0;
}
```

`cai::execute_code` will parse, tokenize, execute code and return value from eax register.
Startup `ebx` value is `0xbada55`.
Above program will push ebx to stack and pop from stack to eax. So eax will eventually contain `0xbada55`.


Fibonacci example:
ctai is still able to compile very (vary very) simplified x86 assembly, but it's able to interpret such code, which compute 15th element of the Fibonacci
```
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
    std::cout << "15th element of fibonacci series is: " << cai::execute_code<code>;
    return 0;
}
```

output: `15th element of fibonacci series is: 610`
