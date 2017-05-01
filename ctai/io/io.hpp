#pragma once

#include "io/output.hpp"

namespace ctai
{
    namespace io
    {
        template <typename buffer, char c>
        using write = output::append<buffer, c>;
    }
}
