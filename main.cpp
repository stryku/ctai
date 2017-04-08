#include "execute/execute_code.hpp"
#include "declare_code.hpp"
#include "kernel/thread.hpp"

#include <iostream>

using main_code = decltype(
":main " // deklarujemy poczatek programu
        "push eax " //jakis syf
        "call .sys_exit_thread"_s); //wywolanie syscalla do wyjscia z watku (na razie jest tylko jeden, glowny

using code = ctai::declare_code<ctai::include::exit_thread, //includujemy kod funkcji exit_thread
                                main_code>;// no i naszego maina


int main()
{
    std::cout << ctai::execute2::execute_code<code>; //wykonujemy to gowno!!!!!!!! execute code zwraca jak na razie ilosc wykonanych instrukcji
    return 0;
}

#include "tests/tests_main.hpp"