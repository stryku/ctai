#pragma once

#include "string.hpp"

namespace ctai
{
    namespace io
    {
        namespace output
        {
            template <char ...chars>
            using buffer = ctai::string<chars...>;

            template <typename buffer_t, char c>
            using append = ctai::string_append<buffer_t, c>;
        }
    }
}
