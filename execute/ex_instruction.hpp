#pragma once

#include <cstddef>

namespace cai
{
    namespace execute
    {
        template <typename state, size_t ...operands>
        struct ex_instruction;
    }
}
