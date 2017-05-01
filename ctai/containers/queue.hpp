#pragma once

#include "tuple/tuple.hpp"

namespace ctai
{
    namespace queue
    {
        template <typename ...types>
        using queue = tuple_n::tuple<types...>;

        template<typename queue_t>
        using front = tuple_n::front<queue_t>;

        template <typename queue_t>
        using pop = tuple_n::drop_front<queue_t, 1>;

        template <typename queue_t, typename type>
        using push = tuple_n::append<queue_t, type>;
    }
}
