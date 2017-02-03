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