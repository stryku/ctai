#pragma once

#include <cstdint>
#include "containers/values_container.hpp"

namespace ctai
{
    template <uint8_t ...values>
    using memory = values_container_n::values_container<values...>;


}
