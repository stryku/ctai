#include "execute/execute_code.hpp"
#include "declare_code.hpp"
#include "kernel/thread.hpp"
#include "runtime/io.hpp"


#include <iostream>
#include <iterator>
#include "kernel/io.hpp"
#include "kernel/utils.hpp"
#include "kernel/memory.hpp"
#include "kernel/mutex.hpp"

using slave_code = decltype(
":slave_code "
        "mov ebx , DWORD PTR [ esp ] " //ebx - struct ptr
        "mov ecx , 0 "

    ":slave_loop "
        "call .get_next_task "//ax - task, cl - task id

        //if task == exit
        "cmp ah , 0 "
        "je .slave_end "

        //else task == fib
        "mov eax , al "
        "call .fibonacci "//eax - fibonacci result
        "mov esi , eax "//esi - fibonacci result

        //calcualte resuls ptr
        "mov eax , 4 "
        "mul ecx "//eax - result offset in array
        "mov ecx , eax "//ecx - result offset in array
        "call .struct_ptr_to_results "
        "add eax , ecx "//eax - result ptr

        //store fibonacci result
        "mov DWORD PTR [ eax ] , esi "

        "jmp .slave_loop "


    ":slave_end "
        "call .sys_exit_thread "_s);

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


//eax - N - number of fib elements to calculate
using calculate_structure_size = decltype(
":calculate_structure_size "
        "push ebx "
        "push ecx "
        "push edx "
        "push esi "

        "mov esi , eax "
        "mov ebx , 0 "

        //elements array
        "mov ecx , 4 "
        "mov eax , esi "
        "mul ecx "
        "add ebx , eax "

        //tasks array
        "mov eax , esi "
        "add eax , 5 " //five slave threads
        "mov ecx , 2 "
        "mul ecx "
        "add eax , 2 " //tasks mutex and current task id
        "add ebx , eax "

        //results array
        "mov eax , esi "
        "mov ecx , 4 "
        "mul ecx "
        "add eax , 1 "//results array mutex
        "add ebx , eax "

        //thread ids array
        "add ebx , 20 " //5 threads * 4 bytes each

        //metadata
        "add ebx , 17 "// 1 + 4 * 4. 1 for N, 4*4 for four pointers to arrays

        "mov eax , ebx "//return size

        "pop esi "
        "pop edx "
        "pop ecx "
        "pop ebx "
        "ret "_s
);

//eax - N - number of fib elements to calculate
//ret - ptr to allocated struct
//metadata:
//   [0] N
//   [1] ptr to elements
//   [5] ptr to tasks
//   [9] ptr to results
//  [13] ptr to threads ids
using struct_create = decltype(
":struct_create "
        "push ebx "
        "push ecx "
        "push edx "
        "push esi "
        "push edi "

        "mov esi , eax " //esi - N

        "call .calculate_structure_size "
        "call .sys_malloc "//eax - ptr to allocated struct

        "push eax "//store ptr to struct on stack

        "mov edi , eax "//edx - ptr to struct

        "mov ebx , eax "
        "add ebx , 17 "// 1 + 4 * 4. 1 for N, 4*4 for four pointers to arrays


        // store N in the struct
        "mov BYTE PTR [ edi ] , esi "

        //store pointer to elements array
        "inc edi "
        "mov DWORD PTR [ edi ] , ebx "

        //calculate pointer to tasks array
        "mov ecx , 4 "
        "mov eax , esi "
        "mul ecx "
        "add ebx , eax "//ebx - pointer to tasks array
        //store pointer to tasks array
        "add edi , 4 "
        "mov DWORD PTR [ edi ] , ebx "

        //calculate pointer to results array
        "mov eax , esi "
        "mov ecx , 4 "
        "mul ecx "
        "add eax , 2 "//results array mutex and current task id
        "add ebx , eax "//ebx - pointer to results array
        //store pointer to results array
        "add edi , 4 "
        "mov DWORD PTR [ edi ] , ebx "

        //calculate pointer to threads ids array
        "mov eax , esi "
        "add eax , 5 " //five slave threads
        "mov ecx , 2 "
        "mul ecx "
        "add eax , 1 " //tasks mutex
        "add eax , 1 " //current task id
        "add ebx , eax "//ebx - pointer to threads ids array
        "add edi , 4 "
        "mov DWORD PTR [ edi ] , ebx "

        "pop eax "//restore struct ptr from the stack

        "pop edi "
        "pop esi "
        "pop edx "
        "pop ecx "
        "pop ebx "
        "ret "_s
);

//ebx - ptr to struct
using struct_ptr_to_elements = decltype(
":struct_ptr_to_elements "
        "mov eax , ebx "
        "add eax , 1 "
        "ret "_s
);

//ebx - ptr to struct
using struct_ptr_to_tasks = decltype(
":struct_ptr_to_tasks "
        "mov eax , ebx "
        "add eax , 5 "
        "ret "_s
);

//ebx - ptr to struct
using struct_ptr_to_results = decltype(
":struct_ptr_to_results "
        "mov eax , ebx "
        "add eax , 9 "
        "ret "_s
);

//ebx - ptr to struct
using struct_ptr_to_ids = decltype(
":struct_ptr_to_ids "
        "mov eax , ebx "
        "add eax , 13 "
        "ret "_s
);

//ebx - ptr to struct
//invalidates - eax
using struct_lock_tasks = decltype(
":struct_lock_tasks "
        "call .struct_ptr_to_tasks "//eax - ptr to tasks
        "call .lock_mutex "
        "ret "_s
);

//ebx - ptr to struct
//invalidates - eax
using struct_unlock_tasks = decltype(
":struct_unlock_tasks "
        "call .struct_ptr_to_tasks "//eax - ptr to tasks
        "call .unlock_mutex "
        "ret "_s
);

//ebx - ptr to struct
using get_current_task_ptr = decltype(
":get_current_task_ptr "
        "push esi "
        "push edi "
        "push ebp "

        "call .struct_ptr_to_tasks "//eax - ptr to tasks
        "mov ebp , eax "//ebp - ptr to tasks

        "mov esi , BYTE PTR [ ebp + 1 ] "//esi - current task id

        //calculate current task ptr
        "mov eax , 2 " //eax - task size
        "mul esi " //eax - current task offset
        "add eax , 2 "//omit tasks metadata
        "add eax , ebp "//eax - ptr to current task

        "pop ebp "
        "pop edi "
        "pop esi "
        "ret "_s
);

//ebx - ptr to struct
//returns
//  al - task
//  cx - task id
//IMPORTANT: it will increase current task id
using get_next_task = decltype(
":get_next_task "
        "push ecx "
        "push edx "
        "push ebp "

        "call .get_current_task_ptr " //eax - current task ptr
        "mov cx , WORD PTR [ eax ] "//cx - task

        "call .struct_ptr_to_tasks "//eax - ptr to tasks
        "mov ebp , eax "//ebp - ptr to tasks

        "mov al , BYTE PTR [ ebp + 1 ] "//esi - current task id
        "mov dl , al "
        "inc al "
        "mov BYTE PTR [ ebp + 1 ] , al "//store next task id

        "mov ax , cx "//return task
        "mov cl , dl "//return task id

        "pop ebp "
        "pop edx "
        "pop ecx "
        "ret "_s
);

//ebx - ptr to struct
//cx - task
//edx - already created tasks
// IMPORTANT: this function will inc edx!!!
using add_task = decltype(
":add_task "
        "push eax "
        "push esi "
        "push edi "
        "push ebp "

        "push edx "

        "call .struct_ptr_to_tasks "//eax - ptr to tasks
        "mov ebp , eax "//ebp - ptr to tasks

        "mov esi , edx "//esi - current task id

        //calculate current task ptr
        "mov eax , 2 " //eax - task size
        "mul esi " //eax - current task offset
        "add eax , 2 "//omit tasks metadata
        "add eax , ebp "//eax - ptr to current task

        //finally store task
        "mov WORD PTR [ eax ] , cx "

        "pop edx "
        "inc edx "

        "pop ebp "
        "pop edi "
        "pop esi "
        "pop eax "
        "ret "_s
);

//eax - fibonacci element
//ebx - ptr to struct
using add_fibonacci_task = decltype(
":add_fibonacci_task "
        "push ecx "

        "mov ch , 1 "
        "mov cl , al "
        "call .add_task "

        "pop ecx "
        "ret "_s
);

//ebx - ptr to struct
using add_exit_task = decltype(
":add_exit_task "
        "push ecx "

        "mov ch , 0 "
        "mov cl , al "
        "call .add_task "

        "pop ecx "
        "ret "_s
);

using create_slave_thread = decltype(
":create_slave_thread "

        "mov ebx , .slave_code " //start point
        "mov ecx , 10 "//priority
        "mov edx , esp "//pointer to args

        "ret "_s
);

//eax - fibonacci element
//ecx - result
using write_result = decltype(
":write_result "
        "push ebx "

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
        "mov BYTE PTR [ eax + 4 ] , 0 "
        "mov eax , ebp "
        "call .write_string "


        //write result
        //convert to string
        "mov ebx , ebp "//ebx - buffer ptr
        "mov eax , ecx "//eax - fibonacci result
        "call .uitoa "

        //write to stdout
        "mov eax , ebp "//eax - buffer ptr
        "call .write_string "

        "add esp , 30 "

        "pop ebx "
        "ret "_s
);

using main2 = decltype(
":main "

        "mov edx , 0 "

        "call .read_uint " //eax - N elements to calculate
        "mov esi , eax "//esi - N
        "mov ebp , esi "//ebp - N copy

        "call .struct_create "//eax - pointer to allocated structure
        "mov ebx , eax "//ebx - pointer to structure

        "call .struct_ptr_to_elements "//eax - ptr to elements array
        "mov ecx , eax "//ecx - ptr to elements array



        //create fibonacii tasks
    ":read_elements_loop "
        //read element from stdin
        "call .read_uint "//eax (in fact al) - fibonacci element
        "call .add_fibonacci_task "

        //store element in array
        "mov DWORD PTR [ ecx ] , eax "
        "add ecx , 4 "

        //loop
        "dec esi "
        "cmp esi , 0 "
        "jne .read_elements_loop "



        //create slave exit tasks
        "mov esi , 5 "//5 slaves

    ":add_slave_exit_task_loop "
        "call .add_exit_task "

        //loop
        "dec esi "
        "cmp esi , 0 "
        "jne .add_slave_exit_task_loop "



        //tasks ready. finally create slave threads
        "call .struct_ptr_to_ids "//eax - ptr to threads ids array
        "mov ecx , eax "//ecx - ptr to threads ids array

        "mov esi , 1 "//5 slaves
        "mov ebx , .slave_code " //slave start point
        "mov ecx , 10 "//priority
        "mov edx , ebx "//pointer to args - struct ptr

    ":create_slaves_loop "
        "call .sys_create_thread "//eax - thread id

        //store id in in array
        "mov DWORD PTR [ ecx ] , eax "
        "add ecx , 4 "

        //loop
        "dec esi "
        "cmp esi , 0 "
        "jne .read_elements_loop "


        //and join the slaves
        "mov esi , 1 "//5 slaves
        "call .struct_ptr_to_ids "//eax - ptr to threads ids array
        "mov ecx , eax "//ecx - ptr to threads ids array

/*    ":join_slaves_loop "
        "mov eax , DWORD PTR [ ecx ] " //eax - next slave id
        "add ecx , 4 "

        "call .join_thread "

        //loop
        "dec esi "
        "cmp esi , 0 "
        "jne .join_slaves_loop "


        //everything ready. Now you have some time, go make a tea or a time machine. In fact, the second one will probably take less time than the above shit.


        //after join, write results to stdout
        "mov edx , 0 "
        "call .struct_ptr_to_results "//eax - ptr to results array
        "mov esi , eax "//eax - ptr to results array

        "call .struct_ptr_to_elements "//eax - ptr to elements array
        "mov edi , eax "//edi - ptr to elements array

    ":write_results_loop "
        "mov eax , DWORD PTR [ edi ] "//eax - next element
        "add edi , 4 "

        "mov ecx , DWORD PTR [ esi ] "//ecx - next result
        "add esi , 4 "

        "call .write_result "

        //loop
        "inc esi "
        "cmp esi , ebp "
        "jne .write_results_loop "

        //free structure
        "mov eax , ebx "
        "call .sys_free "
*/

        "call .sys_exit_thread"_s);

using code2 = ctai::declare_code<ctai::include::thread,
                                 ctai::include::memory,
                                 ctai::include::io,
                                 ctai::include::utils,
                                 ctai::include::mutex,
                                 fibonacci,
                                 calculate_structure_size,
                                 struct_create,
                                 struct_ptr_to_elements,
                                 struct_ptr_to_tasks,
                                 struct_ptr_to_results,
                                 struct_ptr_to_ids,
                                 struct_lock_tasks,
                                 struct_unlock_tasks,
                                 add_task,
                                 add_fibonacci_task,
                                 add_exit_task,
                                 get_current_task_ptr,
                                 get_next_task,
                                 slave_code,
                                 create_slave_thread,
                                 write_result,
                                 main2>;



using execution_result = ctai::execute2::execute_code<code2>;

int main()
{
    constexpr auto output = ctai::runtime::io::make_runtime_output<execution_result::output>();

    std::cout << "Return value: " << execution_result::ret_val << "\n";

    std::cout << "execution output: \n";

    for(auto c : output)
        std::cout<<c;

    return 0;
}

#include "tests/tests_main.hpp"