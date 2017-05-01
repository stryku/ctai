#pragma once

#include "string.hpp"

namespace ctai
{
    namespace details
    {
        template <typename current_code, typename ...rest_of_functions>
        struct declare_code_impl;

        template <typename current_code, typename function, typename ...rest_of_functions>
        struct declare_code_impl<current_code, function, rest_of_functions...>
        {
            using next_code = string_merge<current_code, function>;
            using result = typename declare_code_impl<next_code, rest_of_functions...>::result;
        };

        template <typename current_code>
        struct declare_code_impl<current_code>
        {
            using result = current_code;
        };
    }

    template<typename ...functions>
    using declare_code = typename details::declare_code_impl<string<>, functions...>::result;
}
