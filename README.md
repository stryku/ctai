# compile-time (simplified) x86 assembly interpreter

Hello World example:

```
#include "string.hpp"
#include "execute.hpp"

#include <iostream>

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
```

`cai::execute_code` will parse, tokenize, execute code and return value from eax register.
Startup `ebx` value is `0xbada55`.
Above program will push ebx to stack and pop from stack to eax. So eax will eventually contain `0xbada55`.

## Supported features
1. Multithreading
2. Dynamic memory
3. input/output
4. Couple of syscalls and stdlib functions
5. Asm instructions

Of course awesomeness comes by default.

## Examples
1. [Multithread fibonacci without synchronization](https://github.com/stryku/ctai/blob/master/examples/v2.0/multithread_fib_without_sync.cpp)

![alt text](https://github.com/stryku/ctai/blob/master/images/multithread_fib_without_sync.png "Compiled program output")

2. [Multithread fibonacci with synchronization](https://github.com/stryku/ctai/blob/master/examples/v2.0/multithread_fib_with_sync.cpp)

![alt text](https://github.com/stryku/ctai/blob/master/images/multithread_fib_with_sync.png "Compiled program output")

3. [Unrealized dream](https://github.com/stryku/ctai/blob/master/examples/v2.0/unrealized_dream.cpp)

Didn't managed to compile this beast.

## Documentation
In `doc` folder you can find syscalls and stdlib funcitons documentations. For now there is no documentation with supported asm instructions.
