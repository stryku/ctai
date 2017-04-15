#include "execute/execute_code.hpp"
#include "declare_code.hpp"
#include "kernel/thread.hpp"

#include <iostream>

using main_code = decltype(
":main "
        //push parameters
        "mov eax , 0 "
        "push eax "
        "mov eax , 19 "
        "push eax "
        "mov eax , .thread_code "
        "push eax "

        "call .sys_create_thread "

        "call .sys_exit_thread"_s);

using thread_code = decltype(
":thread_code "
        "call .sys_exit_thread "_s);

using code = ctai::declare_code<ctai::include::thread ,
                                thread_code,
                                main_code>;


int main()
{
    std::cout << ctai::execute2::execute_code<code>;
    return 0;
}

#include "tests/tests_main.hpp"