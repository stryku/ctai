#pragma once

#include "ctai/containers/queue.hpp"

namespace ctai
{
    namespace thread
    {
        namespace schedule
        {
            template <typename threads_queue>
            struct next
            {
                using result_thread = queue::front<threads_queue>;
                using result_threads_queue = queue::pop<threads_queue>;
            };
        }
    }
}