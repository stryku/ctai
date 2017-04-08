#include "string.hpp"
#include "execute/execute_code.hpp"

#include <iostream>

using code = decltype("push eax"_s);

int main()
{
    std::cout << ctai::execute2::execute_code<code>;
    return 0;
}

#include "tests/tests_main.hpp"