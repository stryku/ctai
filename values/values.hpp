#pragma once

#include <cstdint>
#include <cstddef>
#include "containers/values_container.hpp"

namespace ctai
{
    namespace values
    {
        //
        //split_to_byte_values_container
        //
        namespace details
        {
            template <uint32_t value, size_t  value_size>
            struct split_to_byte_values_container_impl<uint8_t, value>
            {
                template <auto ...values>
                using vc = values_container_n::values_container<values...>;

                using result = typename std::conditional_t<value_size == 1,
                                                           vc<value & 0xff>,
                                                           typename std::conditional_t<value_size == 2,
                                                                                       vc<((value & 0xff00) >> 8), value & 0xff>,
                                                                                       vc<((value & 0xff000000) >> 24), ((value & 0xff0000) >> 16), ((value & 0xff00) >> 8), value & 0xff>>>;
            };
        }

        template <auto value>
        using split_to_byte_values_container = typename details::split_to_byte_values_container_impl<value, sizeof(value)>::result;

    }
}
