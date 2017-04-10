#include "execute/execute_code.hpp"
#include "declare_code.hpp"
#include "kernel/thread.hpp"

#include <iostream>

using main_code = decltype(
":main "
        "call .sys_exit_thread"_s);

using code = ctai::declare_code<ctai::include::exit_thread,
                                main_code>;


int main()
{
    //std::cout << ctai::execute2::execute_code<code>;
    return 0;
}

#include "tests/tests_main.hpp"