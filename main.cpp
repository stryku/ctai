#include "execute/execute_code.hpp"
#include "declare_code.hpp"
#include "runtime/io.hpp"
#include "kernel/thread.hpp"
#include "kernel/io.hpp"
#include "kernel/utils.hpp"
#include "kernel/memory.hpp"
#include "kernel/mutex.hpp"

#include <iostream>

//eax - element to calculate
using fibonacci = decltype(
":fibonacci "
        "push esi "
        "push ecx "
        "push edx "

        "mov ecx , eax "
        "cmp eax , 0 "
        "je .fib_ret_0 "

        "mov edx , 0 "
        "mov esi , 1 "

    ":fib_loop "
        "mov eax , esi "
        "add edx , eax "
        "mov esi , edx "
        "mov edx , eax "
        "dec ecx "
        "cmp ecx , 0 "
        "jne .fib_loop "
        "jmp .fib_end "

    ":fib_ret_0 "
        "mov eax , 0 "

    ":fib_end "
        "pop edx "
        "pop ecx "
        "pop esi "
        "ret "_s
);

//eax - fibonacci element
//ecx - result
using write_result = decltype(
":write_result "
        "push ebx "
        "push edi "
        "push ebp "

        "sub esp , 30 "//esp - buffer ptr
        "mov edi , eax "//edi - fibonacci element
        "mov ebp , esp "//ebp - buffer ptr

        //we will write something like this:
        //fib(15) = 610
        //and the new line

        //this is ugly but ctai doesn't support static data definitions yet

        //write fib(
        "mov BYTE PTR [ ebp ] , 'f' "
        "mov BYTE PTR [ ebp + 1 ] , 'i' "
        "mov BYTE PTR [ ebp + 2 ] , 'b' "
        "mov BYTE PTR [ ebp + 3 ] , '(' "
        "mov BYTE PTR [ ebp + 4 ] , 0 "
        "mov eax , ebp "
        "call .write_string "


        //write element
        //convert to string
        "mov ebx , ebp "//ebx - buffer ptr
        "mov eax , edi "//eax - fibonacci element
        "call .uitoa "

        //write to stdout
        "mov eax , ebp "//eax - buffer ptr
        "call .write_string "


        //write ) =
        "mov BYTE PTR [ eax ] , ')' "
        "mov BYTE PTR [ eax + 1 ] , ' ' "
        "mov BYTE PTR [ eax + 2 ] , '=' "
        "mov BYTE PTR [ eax + 3 ] , ' ' "
        "mov BYTE PTR [ eax + 3 ] , ' ' "
        "mov BYTE PTR [ eax + 4 ] , 0 "
        "mov eax , ebp "
        "call .write_string "


        //write result
        //convert to string
        "mov ebx , ebp "//ebx - buffer ptr
        "mov eax , ecx "//eax - fibonacci result
        "call .uitoa "

        //write result to stdout
        "mov eax , ebp "//eax - buffer ptr
        "call .write_string "

        //write new line
        "mov al , 10 "
        "call .sys_write "

        "add esp , 30 "

        "pop ebp "
        "pop edi "
        "pop ebx "
        "ret "_s
);

using slave_code = decltype(
":slave_code "
        "call .read_uint " //eax - element to calculate
        "mov edx , eax "//edx - fib element to calculate

        //calculate fibonacci element
        "mov eax , edx "//eax - fibonacci element to calculate
        "call .fibonacci "//eax - calculated fibonacci element

        "mov ecx , eax " //ecx - calculated fibonacci element
        "mov eax , edx "//eax - fibonacci element to calculate
        "call .write_result "

        "call .sys_exit_thread "_s
);

using main_code = decltype(
":main "
        //prepare arguments to create slaves threads
        "mov ebx , .slave_code "//slave thread entry point
        "mov ecx , 50 " //slave thread priority
        "mov edx , 0 " //pointer to args

        //create two slaves
        "call .sys_create_thread " //eax - slave 1 id
        "mov edi , eax "//edi slave 1 id

        "call .sys_create_thread " //eax - slave 2 id

        //join the slaves
        "call .join_thread "//join thread 2

        "mov eax , edi "//eax - slave 1 id
        "call .join_thread "//join thread 1

        "call .sys_exit_thread "_s
);

using code = ctai::declare_code<ctai::include::thread,
                                ctai::include::io,
                                ctai::include::utils,
                                ctai::include::mutex,
                                fibonacci,
                                slave_code,
                                write_result,
                                main_code>;


//program input - two fibonacci elements to calculate
using input_t = decltype(
"15 10 "_s
);

using execution_result = ctai::execute2::execute_code<code, input_t>;

int main()
{
    constexpr auto output = ctai::runtime::io::make_runtime_output<execution_result::output>();

    std::cout << "Executed instructions: " << execution_result::ret_val << "\n";

    std::cout << "execution output: \n";

    for(auto c : output)
        std::cout<<c;

    return 0;
}
