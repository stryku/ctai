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
