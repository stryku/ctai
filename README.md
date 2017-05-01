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
1. [Multithread fibonacci without synchronization](https://github.com/stryku/ctai/blob/master/ctai/examples/v2.0/multithread_fib_without_sync.cpp)

Output:

![alt text](https://github.com/stryku/ctai/blob/master/images/multithread_fib_without_sync.png "Compiled program output")

2. [Multithread fibonacci with synchronization](https://github.com/stryku/ctai/blob/master/ctai/examples/v2.0/multithread_fib_with_sync.cpp)

Execution flow:
```
MASTER THREAD:
create two slave threads
join these threads
exit thread

SLAVE THREAD:
lock input mutex
read element to calculate from stdin
unlock input mutex
calculate fibonacci for this element
lock output mutex
write result to stdout
unlock output mutex
exit thread
```

Output:

![alt text](https://github.com/stryku/ctai/blob/master/images/multithread_fib_with_sync.png "Compiled program output")

3. [Unrealized dream](https://github.com/stryku/ctai/blob/master/ctai/examples/v2.0/unrealized_dream.cpp)

Didn't managed to compile this beast. My machine doesn't have enough amount of RAM, but here's the flow:
```
MASTER THREAD
read N from input - number of elements to calculate
create structure that will fit
	-N-element array of uint8_t for elements
	-N-element array of uint32_t for results
	-(N + 5)-element array of uint16_t for tasks. + 5 because five tasks to end slave thread
	-5-element array of uint32_t for slaves ids
create tasks
	-N tasks to calculate fibonacci element
	-five tasks to end slave thread
create five slave threads. Pass pointer to the structure in `args` argument
join slaves
write results to stdout
exit thread


SLAVE THREAD
while true
	lock tasks array mutex
	get next task
	unlock tasks array mutex
	if next task is fibonacci
		calculate fibonacci
		lock results array mutex
		write result to array
		unlock results array mutex
	else if task is exit
		exit thread
```


## Documentation
In `doc` folder you can find syscalls and stdlib funcitons documentations. For now there is no documentation with supported asm instructions.
