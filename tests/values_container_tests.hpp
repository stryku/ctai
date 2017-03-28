#pragma once

#include "containers/values_container.hpp"

#include "tests/tests_macros.hpp"

namespace ctai::tests::values_container
{
    template<auto ...values>
    using con = values_container_n::values_container<values...>;

    using test_con = con<6, 7, 8>;

    namespace t0
    {
        ASSERT_EQ(test_con::size, 3);
    }

    namespace t1
    {
        using expected = con<7, 8>;
        ASSERT_EQ_T(values_container_n::pop_front<test_con>, expected);
    }

    namespace t2
    {
        using expected = con<6, 7, 8, 9>;
        using test_type = values_container_n::append<test_con, 9>;
        ASSERT_EQ_T(test_type, expected);
    }

    namespace t3
    {
        using expected = con<6, 7, 8, 9, 0>;
        using test_type = values_container_n::merge<test_con, con<9, 0>>;
        ASSERT_EQ_T(test_type, expected);
    }

    namespace t4
    {
        ASSERT_EQ(values_container_n::front<test_con>, 6);
    }

    namespace t5
    {
        using expected_rest = con<8>;
        using expected_taken = con<6, 7>;
        using test_type = values_container_n::take<2, test_con>;
        ASSERT_EQ_T(test_type::taken_container, expected_taken);
        ASSERT_EQ_T(test_type::rest_container, expected_rest);
    }
}