#pragma once

#include "containers/values_container.hpp"

#include "tests/tests_macros.hpp"

namespace ctai::tests::values_container
{
    template<auto ...values>
    using con = values_container_n::values_container<values...>;

    using test_con = con<1, 2, 3>;

    namespace t0
    {
        ASSERT_EQ(test_con::size, 3);
    }

    namespace t1
    {
        using expected = con<2, 3>;
        ASSERT_EQ_T(values_container_n::pop_front<test_con>, expected);
    }

    namespace t2
    {
        using expected = con<1, 2, 3, 4>;
        using test_type = values_container_n::append<test_con, 4>;
        ASSERT_EQ_T(test_type, expected);
    }

    namespace t3
    {
        using expected = con<1, 2, 3, 4, 5>;
        using test_type = values_container_n::merge<test_con, con<4, 5>>;
        ASSERT_EQ_T(test_type, expected);
    }

    namespace t4
    {
        ASSERT_EQ(values_container_n::front<test_con>, 1);
    }
}