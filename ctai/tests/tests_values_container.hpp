#pragma once

#include "ctai/containers/values_container.hpp"

#include "tests/tests_macros.hpp"

namespace ctai::tests::values_container
{
    template<auto ...values>
    using vc = values_container_n::values_container<values...>;

    using test_con = vc<6, 7, 8>;

    namespace test_size
    {
        ASSERT_EQ(test_con::size, 3);
    }

    namespace test_pop_front
    {
        using expected = vc<7, 8>;
        ASSERT_EQ_T(values_container_n::pop_front<test_con>, expected);
    }

    namespace test_append
    {
        using expected = vc<6, 7, 8, 9>;
        using test_type = values_container_n::append<test_con, 9>;
        ASSERT_EQ_T(test_type, expected);
    }

    namespace test_merge
    {
        namespace merge_empty
        {
            using expected = test_con;

            using test_type = values_container_n::merge<test_con, vc<>>;
            ASSERT_EQ_T(test_type, expected);

            using test_type2 = values_container_n::merge<vc<>, test_con>;
            ASSERT_EQ_T(test_type2, expected);
        }

        namespace merge_some
        {
            using expected = vc<6, 7, 8, 9, 0>;
            using test_type = values_container_n::merge<test_con, vc<9, 0>>;
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
            using expected_taken = vc<>;
            using test_type = values_container_n::take<0, test_con>;
            ASSERT_EQ_T(test_type::taken_container, expected_taken);
            ASSERT_EQ_T(test_type::rest_container, expected_rest);
        }

        namespace take_some
        {
            using expected_rest = vc<8>;
            using expected_taken = vc<6, 7>;
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
                using expected = vc<3, 7, 8>;
                using test_type = values_container_n::set<0, test_con, 3>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t2
            {
                using expected = vc<3, 4, 8>;
                using test_type = values_container_n::set<0, test_con, 3, 4>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t3
            {
                using expected = vc<3, 4, 5>;
                using test_type = values_container_n::set<0, test_con, 3, 4, 5>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t4
            {
                using expected = vc<6, 4, 5>;
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
            using test_type = values_container_n::drop_front<test_con, 0>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace drop_front_some
        {
            namespace t1
            {
                using expected = vc<7, 8>;
                using test_type = values_container_n::drop_front<test_con, 1>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t2
            {
                using expected = vc<8>;
                using test_type = values_container_n::drop_front<test_con, 2>;
                ASSERT_EQ_T(test_type, expected);
            }

            namespace t3
            {
                using expected = vc<>;
                using test_type = values_container_n::drop_front<test_con, 3>;
                ASSERT_EQ_T(test_type, expected);
            }
        }
    }

    namespace test_get
    {
        namespace get_1
        {
            namespace t1
            {
                constexpr auto expected = 6;
                constexpr auto result = values_container_n::get_1<test_con, 0>;
                ASSERT_EQ(result, expected);
            }

            namespace t2
            {
                constexpr auto expected = 7;
                constexpr auto result = values_container_n::get_1<test_con, 1>;
                ASSERT_EQ(result, expected);
            }

            namespace t3
            {
                constexpr auto expected = 8;
                constexpr auto result = values_container_n::get_1<test_con, 2>;
                ASSERT_EQ(result, expected);
            }
        }

        namespace get_2
        {
            namespace t1
            {
                using expected = vc<6, 7>;
                using result = values_container_n::get_2<test_con, 0>;
                ASSERT_EQ_T(result, expected);
            }

            namespace t2
            {
                using expected = vc<7, 8>;
                using result = values_container_n::get_2<test_con, 1>;
                ASSERT_EQ_T(result, expected);
            }
        }

        namespace get_4
        {
            using test_vc_get_4 = vc<1,2,3,4,5,6>;

            namespace t1
            {
                using expected = vc<1,2,3,4>;
                using result = values_container_n::get_4<test_vc_get_4, 0>;
                ASSERT_EQ_T(result, expected);
            }

            namespace t2
            {
                using expected = vc<2,3,4,5>;
                using result = values_container_n::get_4<test_vc_get_4, 1>;
                ASSERT_EQ_T(result, expected);
            }

            namespace t3
            {
                using expected = vc<3,4,5,6>;
                using result = values_container_n::get_4<test_vc_get_4, 2>;
                ASSERT_EQ_T(result, expected);
            }
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
            using expected = vc<>;
            using test_type = values_container_n::create_with_val<0, 11>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace create_with_val_one_size
        {
            using expected = vc<3>;
            using test_type = values_container_n::create_with_val<1, 3>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace create_with_val_some_size
        {
            using expected = vc<11, 11, 11>;
            using test_type = values_container_n::create_with_val<3, 11>;
            ASSERT_EQ_T(test_type, expected);
        }

        namespace create_with_val_bool
        {
            using expected = vc<true, true, true>;
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
            using expected = vc<3, 7,8>;
            using test_result = values_container_n::set_val<test_con, 0, 1, 3>;
            ASSERT_EQ_T(test_result, expected);
        }
    }

    namespace tests_bytes_to_value
    {
        using test_vc_bytes_to_value = vc<1,2,3,4,5,6>;

        namespace bytes_to_value_1
        {
            constexpr uint8_t expected = 1;
            constexpr auto result = values_container_n::bytes_to_value_1<test_vc_bytes_to_value>;

            ASSERT_EQ(expected, result);
        }

        namespace bytes_to_value_2
        {
            constexpr uint16_t expected = 0x0102;
            constexpr auto result = values_container_n::bytes_to_value_2<test_vc_bytes_to_value>;

            ASSERT_EQ(expected, result);
        }

        namespace bytes_to_value_4
        {
            constexpr uint32_t expected = 0x01020304;
            constexpr auto result = values_container_n::bytes_to_value_4<test_vc_bytes_to_value>;

            ASSERT_EQ(expected, result);
        }
    }
}
