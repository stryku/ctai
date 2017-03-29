#pragma once

#include <cstdint>
#include <cstddef>
#include "containers/values_container.hpp"

namespace ctai
{
    template <uint8_t ...values>
    using memory_block = values_container_n::values_container<values...>;

    template <size_t size>
    struct memory_metadata
    {
        using bytes_metadata = values_container_n::create<bool, size>;
    };

    template <size_t size>
    struct memory
    {
        using mem = memory_block<size>;
        using metadata = memory_metadata<size>;
    };

    template <size_t size>
    using memory_create = memory<size>;
}
