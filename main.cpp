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