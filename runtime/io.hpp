#pragma once

#include "io/output.hpp"

#include <array>

namespace ctai
{
    namespace runtime
    {
        namespace io
        {
            namespace details
            {
                template <char ...chars>
                constexpr auto make_runtime_output_impl(ctai::io::output::buffer<chars...>)
                {
                    return std::array<char, sizeof...(chars)>{chars...};
                }
            }

            template <typename buffer>
            constexpr auto make_runtime_output()
            {
                return details::make_runtime_output_impl(buffer{});
            }
        }
    }
}
