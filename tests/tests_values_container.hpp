#pragma once

#include "containers/values_container.hpp"

#include "tests/tests_macros.hpp"

namespace ctai::tests::values_container
{
    template<auto ...values>
    using con = values_container_n::values_container<values...>;

    using test_con = con<6, 7, 8>;

    namespace test_size
    {
        ASSERT_EQ(test_con::size, 3);
    }

    namespace test_pop_front
    {
        using expected = con<7, 8>;
        ASSERT_EQ_T(values_container_n::pop_front<test_con>, expected);
    }

    namespace test_append
    {
        using expected = con<6, 7, 8, 9>;
        using test_type = values_container_n::append<test_con, 9>;
        ASSERT_EQ_T(test_type, expected);
    }

    namespace test_merge
    {
        namespace merge_empty
        {
            using expected = test_con;

            using test_type = values_container_n::merge<test_con, con<>>;
            ASSERT_EQ_T(test_type, expected);

            using test_type2 = values_container_n::merge<con<>, test_con>;
            ASSERT_EQ_T(test_type2, expected);
        }

        namespace merge_some
        {
            using expected = con<6, 7, 8, 9, 0>;
            using test_type = values_container_n::merge<test_con, con<9, 0>>;
            ASSERT_EQ_T(test_type, expected);
        }
    }

    namespace test_front
    {
        ASSERT_EQ(values_container_n::front<test_con>, 6);
    }

    namespace test_take
    {
        namespace take_0
        {
            using expected_rest = test_con;
            using expected_taken = con<>;
            using test_type = values_container_n::take<0, test_con>;
            ASSERT_EQ_T(test_type::taken_container, expected_taken);
            ASSERT_EQ_T(test_type::rest_container, expected_rest);
        }

        namespace take_some
        {
            using expected_rest = con<8>;
            using expected_taken = con<6, 7>;
            using test_type = values_container_n::take<2, test_con>;
            ASSERT_EQ_T(test_type::taken_container, expected_taken);
            ASSERT_EQ_T(test_type::rest_container, expected_rest);
        }
    }

    namespace test_set
    {
        namespace set_0
        {
            using expected = test_con;
            using test_type = values_container_n::set<0, test_con>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace set_some
        {
            namespace t1
            {
                using expected = con<3, 7, 8>;
                using test_type = values_container_n::set<0, test_con, 3>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t2
            {
                using expected = con<3, 4, 8>;
                using test_type = values_container_n::set<0, test_con, 3, 4>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t3
            {
                using expected = con<3, 4, 5>;
                using test_type = values_container_n::set<0, test_con, 3, 4, 5>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t4
            {
                using expected = con<6, 4, 5>;
                using test_type = values_container_n::set<1, test_con, 4, 5>;
                ASSERT_EQ_T(test_type, expected);
            }
        }
    }

    namespace test_drop_front
    {
        namespace drop_front_0
        {
            using expected = test_con;
            using test_type = values_container_n::drop_front<0, test_con>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace drop_front_some
        {
            namespace t1
            {
                using expected = con<7, 8>;
                using test_type = values_container_n::drop_front<1, test_con>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t2
            {
                using expected = con<8>;
                using test_type = values_container_n::drop_front<2, test_con>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t3
            {
                using expected = con<>;
                using test_type = values_container_n::drop_front<3, test_con>;
                ASSERT_EQ_T(test_type, expected);
            }
        }
    }

    namespace test_get
    {
        namespace t1
        {
            constexpr auto expected = 6;
            constexpr auto result = values_container_n::get<0, test_con>;
            ASSERT_EQ(result, expected);
        }

        namespace t2
        {
            constexpr auto expected = 7;
            constexpr auto result = values_container_n::get<1, test_con>;
            ASSERT_EQ(result, expected);
        }

        namespace t3
        {
            constexpr auto expected = 8;
            constexpr auto result = values_container_n::get<2, test_con>;
            ASSERT_EQ(result, expected);
        }
    }

    namespace test_create
    {
        namespace t1
        {
            constexpr auto expected = 0;
            using result = values_container_n::create<uint8_t, 0>;
            ASSERT_EQ(result::size, expected);
        }

        namespace t2
        {
            constexpr auto expected = 22;
            using result = values_container_n::create<uint8_t, 22>;
            ASSERT_EQ(result::size, expected);
        }
    }

    namespace test_create_with_val
    {
        namespace create_with_val_zero_size
        {
            using expected = con<>;
            using test_type = values_container_n::create_with_val<0, 11>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace create_with_val_one_size
        {
            using expected = con<3>;
            using test_type = values_container_n::create_with_val<1, 3>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace create_with_val_some_size
        {
            using expected = con<11, 11, 11>;
            using test_type = values_container_n::create_with_val<3, 11>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace create_with_val_bool
        {
            using expected = con<true, true, true>;
            using test_type = values_container_n::create_with_val<3, true>;
            ASSERT_EQ_T(test_type, expected);
        }
    }

    namespace test_set_val
    {

        namespace set_val_zero_count
        {
            using expected = test_con;
            using test_result = values_container_n::set_val<test_con, 0, 0, 0>;
            ASSERT_EQ_T(test_result, expected);
        }

        namespace set_val_one_count
        {
            using expected = con<3, 7,8>;
            using test_result = values_container_n::set_val<test_con, 0, 1, 3>;
            ASSERT_EQ_T(test_result, expected);
        }
    }
}
