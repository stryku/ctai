#pragma once

#include <cstddef>

namespace ctai
{
    namespace execute2
    {
        template <typename output_t, size_t ret_val_v>
        struct execution_result
        {
            using output = output_t;
            size_t ret_val = ret_val_v;
        };
    }
}
