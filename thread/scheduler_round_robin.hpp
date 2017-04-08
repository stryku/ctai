#pragma once

#include "containers/queue.hpp"

namespace ctai
{
    namespace thread
    {
        namespace schedule
        {
            template <typename threads_queue>
            struct next
            {
                using next_thread = queue::front<threads_queue>;
                using result_threads_queue = queue::push<queue::pop<threads_queue>, next_thread>;
            };
        }
    }
}