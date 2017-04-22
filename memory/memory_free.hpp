#pragma once

#include "memory/memory.hpp"
#include "memory/memory_metadata.hpp"
#include "traits.hpp"

#include <cstddef>

namespace ctai
{
    namespace memory
    {
        template <typename memory_t, size_t ptr>
        using free = metadata::free_block<typename memory_t::asd, ptr>;
    }
}
