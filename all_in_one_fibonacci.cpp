#include <type_traits>
#include <cstdlib>
#include <stdint.h>
#include <limits>


template <size_t val, size_t ...values>
constexpr auto is_value_in_v = ((val == values) || ...);

template <typename type, typename ...types>
constexpr auto is_type_in_v = ((std::is_same<type, types>::value) || ...);

namespace cai
{
    template <char ...chars>
    struct string
    {
        static constexpr auto size = sizeof...(chars);
    };





    namespace details
    {
        template <typename str>
        struct to_string_impl;

        template <char ...chars>
        struct to_string_impl<string<chars...>>
        {
            using type = string<chars...>;
        };
    }

    template <typename s>
    using to_string = typename details::to_string_impl<s>::type;




    namespace details
    {
        template <typename, typename, bool eq_size>
        struct string_eq_impl;


        template <char ...lchars, char ...rchars>
        struct string_eq_impl<string<lchars...>, string<rchars...>, false>
        {
            static constexpr auto value = false;
        };

        template <>
        struct string_eq_impl<string<>, string<>, true>
        {
            static constexpr auto value = true;
        };

        template <char eq_first, char ...lchars, char ...rchars>
        struct string_eq_impl<string<eq_first, lchars...>, string<eq_first, rchars...>, true>
        {
            static constexpr auto value = string_eq_impl<string<lchars...>, string<rchars...>, true>::value;
        };

        template <char lfirst, char rfirst, char ...lchars, char ...rchars>
        struct string_eq_impl<string<lfirst, lchars...>, string<rfirst, rchars...>, true>
        {
            static constexpr auto value = false;
        };
    }

    template <typename ls, typename rs>
    constexpr auto string_eq = details::string_eq_impl<ls, rs, to_string<ls>::size == to_string<rs>::size>::value;




    namespace details
    {
        template <typename>
        struct string_front_impl;

        template <char front, char ...chars>
        struct string_front_impl<string<front, chars...>>
        {
            static constexpr auto value = front;
        };
    }

    template <typename s>
    constexpr auto string_front = details::string_front_impl<s>::value;




    namespace details
    {
        template <typename, char>
        struct string_append_impl;

        template <char ...chars, char value>
        struct string_append_impl<string<chars...>, value>
        {
            using type = string<chars..., value>;
        };
    }

    template <typename s, char value>
    using string_append = typename details::string_append_impl<s, value>::type;




    namespace details
    {
        template <typename, typename>
        struct string_merge_impl;

        template <char ...chars1, char ...chars2>
        struct string_merge_impl<string<chars1...>, string<chars2...>>
        {
            using type = string<chars1..., chars2...>;
        };
    }

    template <typename s, typename s2>
    using string_merge = typename details::string_merge_impl<s, s2>::type;




    namespace details
    {
        template <typename>
        struct string_pop_front_impl;

        template <char front, char ...chars>
        struct string_pop_front_impl<string<front, chars...>>
        {
            using type = string<chars...>;
        };
    }

    template <typename s>
    using string_pop_front = typename details::string_pop_front_impl<s>::type;





    namespace details
    {
        template <typename, int sign = 1, int current_val = 0>
        struct string_to_int_impl;

        template <int sign, int current_val>
        struct string_to_int_impl<string<>, sign, current_val>
        {
            static constexpr auto value = sign * current_val;
        };

        template <int sign, int current_val, char current_char, char ...chars>
        struct string_to_int_impl<string<current_char, chars...>, sign, current_val>
        {
            static constexpr auto nex_val = current_val * 10 + (current_char - '0');
            static constexpr auto value = string_to_int_impl<string<chars...>, sign, nex_val>::value;
        };

        template <int sign, int current_val, char current_char, char ...chars>
        struct string_to_int_impl<string<'-', current_char, chars...>, sign, current_val>
        {
            static constexpr auto nex_val = current_val * 10 + (current_char - '0');
            static constexpr auto value = string_to_int_impl<string<chars...>, -1, nex_val>::value;
        };
    }

    template <typename str>
    constexpr auto string_to_int = details::string_to_int_impl<str>::value;




    namespace details
    {
        template <int value, typename sign_str, bool end = false, typename current_string = string<>>
        struct string_from_int_impl;

        template <typename sign_str, typename current_string>
        struct string_from_int_impl<0, sign_str, true, current_string>
        {
            using type = string_merge<sign_str, current_string>;
        };

        template <int value, typename sign_str, char ...chars>
        struct string_from_int_impl<value, sign_str, false, string<chars...>>
        {
            using type = typename string_from_int_impl<
                    value/10,
                    sign_str,
                    value/10 == 0,
                    string<value % 10 + '0', chars...>>::type;
        };

        template <int value>
        struct prepare_sign_and_convert
        {
            using type = typename std::conditional_t<
                    value < 0,
                    string_from_int_impl<value * -1, string<'-'>>,
                    string_from_int_impl<value, string<>>
                    >::type;
        };
    }
    template <int value>
    using string_from_int = typename details::prepare_sign_and_convert<value>::type;
}

template <typename T, T... chars>
constexpr auto operator""_s() {
    return cai::string<chars...>{};
}

namespace cai
{
    namespace tests
    {
        static_assert(string_eq<string<'a','b'>, string<'a','b'>>, "");
        static_assert(string_eq<string<'a','b'>, decltype("ab"_s)>, "");
        static_assert(!string_eq<string<'a','b'>, string<'A','b'>>, "");
        static_assert(!string_eq<string<'a','b','c'>, string<'a','b'>>, "");
        static_assert(!string_eq<string<'A','b'>, string<'a','b'>>, "");
        static_assert(!string_eq<string<'a','b'>, string<'a','b','c'>>, "");

        static_assert(string_front<decltype("abc"_s)> == 'a', "");
        static_assert(string_front<decltype(" abc"_s)> == ' ', "");

        static_assert(string_to_int<decltype("12"_s)> == 12, "");
        static_assert(string_to_int<decltype("-2"_s)> == -2, "");

        static_assert(std::is_same<string_from_int<12>, decltype("12"_s)>::value, "");
        static_assert(std::is_same<string_from_int<-2>, decltype("-2"_s)>::value, "");

    }
}

namespace cai
{
    namespace inst
    {
        enum class id_t
        {
            EXIT,
            PUSH_REG,
            POP_REG,

            JMP,
            JGE,

            ADD_REG_REG,
            ADD_REG_VAL,

            SUB_REG_REG,
            CMP_REG_REG,

            MOV_REG_REG,
            MOV_REG_VAL,


            MOV_MEM_VAL__mem_eq_reg_minus_const,
            MOV_MEM_VAL__mem_eq_reg_plus_const,

            MOV_MEM_REG__mem_eq_reg_minus_const,
            MOV_MEM_REG__mem_eq_reg_plus_const,

            MOV_REG_MEM__mem_eq_reg_minus_const,
            MOV_REG_MEM__mem_eq_reg_plus_const,


            MUL_REG_REG
        };

        template <size_t id_v>
        static constexpr auto to_id = static_cast<id_t>(id_v);

        template <id_t id_v>
        static constexpr auto to_size = static_cast<size_t>(id_v);
    }

    namespace memory
    {
        enum class id_t
        {
            s_8,
            s_16,
            s_32
        };

        template <size_t id_v>
        static constexpr auto to_id = static_cast<id_t>(id_v);

        template <id_t id_v>
        static constexpr auto to_size = static_cast<size_t>(id_v);

        namespace details
        {
            template <id_t>
            struct to_mem_type_impl;

            template <>
            struct to_mem_type_impl<id_t::s_8>
            {
                using type = uint8_t;
            };

            template <>
            struct to_mem_type_impl<id_t::s_16>
            {
                using type = uint16_t;
            };

            template <>
            struct to_mem_type_impl<id_t::s_32>
            {
                using type = uint32_t;
            };
        }

        template <id_t t>
        using to_mem_type = typename details::to_mem_type_impl<t>::type;
    }

    namespace operands
    {
        enum class id_t
        {
            REG_8
        };

        template <size_t id_v>
        static constexpr auto to_id = static_cast<id_t>(id_v);

        template <id_t id_v>
        static constexpr auto to_size = static_cast<size_t>(id_v);
    }

    namespace regs
    {
        enum class id_t
        {
            AL,
            AH,
            AX,
            EAX,
            BL,
            BH,
            BX,
            EBX,
            CL,
            CH,
            CX,
            ECX,
            DL,
            DH,
            DX,
            EDX,
            SI,
            ESI,
            DI,
            EDI,
            BP,
            EBP,
            SP,
            ESP,
            IP,
            EIP
        };


        template <size_t id_v>
        static constexpr auto to_id = static_cast<id_t>(id_v);

        template <id_t id_v>
        static constexpr auto to_size = static_cast<size_t>(id_v);


        template <id_t reg>
        struct reg_size_type_impl
        {
            using type = std::conditional_t<is_value_in_v<to_size<reg>, to_size<id_t::AL>, to_size<id_t::AH>, to_size<id_t::BL>, to_size<id_t::BH>, to_size<id_t::CL>, to_size<id_t::CH>>,
                    uint8_t,
                    std::conditional_t<is_value_in_v<to_size<reg>, to_size<id_t::AX>, to_size<id_t::BX>, to_size<id_t::CX>, to_size<id_t::DX>, to_size<id_t::SI>, to_size<id_t::DI>, to_size<id_t::BP>, to_size<id_t::SP>, to_size<id_t::IP>>,
                            uint16_t, uint32_t >>;

        };

        template <id_t reg>
        using reg_size_type = typename reg_size_type_impl<reg>::type;

        namespace tests
        {
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::AL>, uint8_t>::value, "");
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::AH>, uint8_t>::value, "");
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::CL>, uint8_t>::value, "");
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::CH>, uint8_t>::value, "");

            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::AX>, uint16_t>::value, "");
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::BX>, uint16_t>::value, "");
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::SP>, uint16_t>::value, "");
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::IP>, uint16_t>::value, "");

            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::EAX>, uint32_t>::value, "");
            static_assert(std::is_same<reg_size_type<::cai::regs::id_t::EBX>, uint32_t>::value, "");
        }
    }
}
namespace detail
{
    struct true_type {};
    struct false_type {};
    template <int test> struct converter : public true_type {};
    template <> struct converter<0> : public false_type {};
}



namespace cai
{
    template <uint32_t val>
    struct reg
    {
        static constexpr auto l = val & 0xff;
        static constexpr auto h = static_cast<uint8_t>((val & 0xff00) >> 8);
        static constexpr auto x = static_cast<uint16_t>(val & 0xffff);
        static constexpr auto ex = val;

        static constexpr auto value = val;
    };

    template <uint32_t reg_val, uint8_t new_val> static constexpr auto set_reg_l = (reg_val & 0xffffff00) | new_val;
    template <uint32_t reg_val, uint8_t new_val> static constexpr auto set_reg_h = (reg_val & 0xffff00ff) | (static_cast<uint32_t>(new_val) << 8);
    template <uint32_t reg_val, uint16_t new_val> static constexpr auto set_reg_x = (reg_val & 0xffff0000) | new_val;
    template <uint32_t reg_val, uint32_t new_val> static constexpr auto set_reg_ex = new_val;

    template <uint32_t val>
    struct simple_reg
    {
        static constexpr auto low_16 = val & 0xffff;
        static constexpr auto value = val;

        template <uint16_t new_val> using set_low = simple_reg<(val | 0xffff0000) | new_val>;
        template <uint32_t new_val> using set_val = simple_reg<new_val>;
    };

    template <uint32_t reg_val, uint16_t new_val> static constexpr auto set_simple_reg_low = (reg_val & 0xffff0000) | new_val;
    template <uint32_t reg_val, uint32_t new_val> static constexpr auto set_simple_reg = new_val;

    template <uint32_t eax_val,
            uint32_t ebx_val,
            uint32_t ecx_val,
            uint32_t edx_val,
            uint32_t esp_val,
            uint32_t ebp_val,
            uint32_t esi_val,
            uint32_t edi_val,
            uint32_t eip_val>
    struct registers_state
    {
        using eax_t = reg<eax_val>;
        using ebx_t = reg<ebx_val>;
        using ecx_t = reg<ecx_val>;
        using edx_t = reg<edx_val>;

        static constexpr eax_t eax{};
        static constexpr ebx_t ebx{};
        static constexpr ecx_t ecx{};
        static constexpr edx_t edx{};

        static constexpr simple_reg<esp_val> esp{};
        static constexpr simple_reg<ebp_val> ebp{};
        static constexpr simple_reg<esi_val> esi{};
        static constexpr simple_reg<edi_val> edi{};
        static constexpr simple_reg<eip_val> eip{};

        template <uint8_t val> using set_al = registers_state<set_reg_l<eax_val, val>, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint8_t val> using set_ah = registers_state<set_reg_h<eax_val, val>, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint16_t val> using set_ax = registers_state<set_reg_x<eax_val, val>, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint32_t val> using set_eax = registers_state<set_reg_ex<eax_val, val>, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;

        template <uint8_t val> using set_bl = registers_state<eax_val, set_reg_l<ebx_val, val>, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint8_t val> using set_bh = registers_state<eax_val, set_reg_h<ebx_val, val>, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint16_t val> using set_bx = registers_state<eax_val, set_reg_x<ebx_val, val>, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint32_t val> using set_ebx = registers_state<eax_val, set_reg_ex<ebx_val, val>, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;

        template <uint8_t val> using set_cl = registers_state<eax_val, ebx_val, set_reg_l<ecx_val, val>, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint8_t val> using set_ch = registers_state<eax_val, ebx_val, set_reg_h<ecx_val, val>, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint16_t val> using set_cx = registers_state<eax_val, ebx_val, set_reg_x<ecx_val, val>, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint32_t val> using set_ecx = registers_state<eax_val, ebx_val, set_reg_ex<ecx_val, val>, edx_val, esp_val, ebp_val, esi_val, edi_val, eip_val>;

        template <uint8_t val> using set_dl = registers_state<eax_val, ebx_val, ecx_val, set_reg_l<edx_val, val>, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint8_t val> using set_dh = registers_state<eax_val, ebx_val, ecx_val, set_reg_h<edx_val, val>, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint16_t val> using set_dx = registers_state<eax_val, ebx_val, ecx_val, set_reg_x<edx_val, val>, esp_val, ebp_val, esi_val, edi_val, eip_val>;
        template <uint32_t val> using set_edx = registers_state<eax_val, ebx_val, ecx_val, set_reg_ex<edx_val, val>, esp_val, ebp_val, esi_val, edi_val, eip_val>;

        template <uint16_t val> using set_sp = registers_state<eax_val, ebx_val, ecx_val, edx_val, set_simple_reg_low<esp_val, val>, ebp_val, esi_val, edi_val, eip_val>;
        template <uint32_t val> using set_esp = registers_state<eax_val, ebx_val, ecx_val, edx_val, set_simple_reg<esp_val, val>, ebp_val, esi_val, edi_val, eip_val>;

        template <uint16_t val> using set_bp = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, set_simple_reg_low<ebp_val, val>, esi_val, edi_val, eip_val>;
        template <uint32_t val> using set_ebp = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, set_simple_reg<ebp_val, val>, esi_val, edi_val, eip_val>;

        template <uint16_t val> using set_si = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, set_simple_reg_low<esp_val, val>, edi_val, eip_val>;
        template <uint32_t val> using set_esi = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, set_simple_reg<esp_val, val>, edi_val, eip_val>;

        template <uint16_t val> using set_di = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esp_val, set_simple_reg_low<edi_val, val>, eip_val>;
        template <uint32_t val> using set_edi = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esp_val, set_simple_reg<edi_val, val>, eip_val>;

        template <uint16_t val> using set_ip = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esp_val, edi_val, set_simple_reg_low<eip_val, val>>;
        template <uint32_t val> using set_eip = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esp_val, edi_val, set_simple_reg<eip_val, val>>;
    };

    using startup_registers_state = registers_state<
            0xaabbccdd,
            0xbada55,
            0xaabbccdd,
            0xaabbccdd,
            15,
            0xaabbccdd,
            0xaabbccdd,
            0xaabbccdd,
                    0>;


    namespace details
    {
        template <typename>
        struct to_register_state_impl;


        template <uint32_t eax_val,
                uint32_t ebx_val,
                uint32_t ecx_val,
                uint32_t edx_val,
                uint32_t esp_val,
                uint32_t ebp_val,
                uint32_t esi_val,
                uint32_t edi_val,
                uint32_t eip_val>
        struct to_register_state_impl<
                registers_state<
                        eax_val,
                        ebx_val,
                        ecx_val,
                        edx_val,
                        esp_val,
                        ebp_val,
                        esi_val,
                        edi_val,
                        eip_val>>
        {
            using type = registers_state<
                    eax_val,
                    ebx_val,
                    ecx_val,
                    edx_val,
                    esp_val,
                    ebp_val,
                    esi_val,
                    edi_val,
                    eip_val>;
        };
    }

    template <typename reg_state>
    using to_register_state = typename details::to_register_state_impl<reg_state>::type;

    namespace tests
    {
        static_assert(reg<0xddccbbaa>{}.l == 0xaa, "");
        static_assert(reg<0xddccbbaa>{}.h == 0xbb, "");
        static_assert(reg<0xddccbbaa>{}.x == 0xbbaa, "");
        static_assert(reg<0xddccbbaa>{}.ex == 0xddccbbaa, "");

        static_assert(simple_reg<0xddccbbaa>{}.low_16 == 0xbbaa, "");
        static_assert(simple_reg<0xddccbbaa>{}.value == 0xddccbbaa, "");
    }


    namespace details
    {
        template<typename state, regs::id_t id>
        struct get_reg_impl;

        template<typename state>
        struct get_reg_impl<state, regs::id_t::AL> { static constexpr auto val = to_register_state<state>{}.eax.l; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::AH> { static constexpr auto val = to_register_state<state>{}.eax.h; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::AX> { static constexpr auto val = to_register_state<state>{}.eax.x; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::EAX> { static constexpr auto val = to_register_state<state>{}.eax.ex; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::BL> { static constexpr auto val = to_register_state<state>{}.ebx.l; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::BH> { static constexpr auto val = to_register_state<state>{}.ebx.h; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::BX> { static constexpr auto val = to_register_state<state>{}.ebx.x; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::EBX> { static constexpr auto val = to_register_state<state>{}.ebx.ex; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::CL> { static constexpr auto val = to_register_state<state>{}.ecx.l; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::CH> { static constexpr auto val = to_register_state<state>{}.ecx.h; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::CX> { static constexpr auto val = to_register_state<state>{}.ecx.x; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::ECX> { static constexpr auto val = to_register_state<state>{}.ecx.ex; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::DL> { static constexpr auto val = to_register_state<state>{}.edx.l; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::DH> { static constexpr auto val = to_register_state<state>{}.edx.h; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::DX> { static constexpr auto val = to_register_state<state>{}.edx.x; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::EDX> { static constexpr auto val = to_register_state<state>{}.edx.ex; };


        template<typename state>
        struct get_reg_impl<state, regs::id_t::SI> { static constexpr auto val = to_register_state<state>{}.esi.low_16; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::DI> { static constexpr auto val = to_register_state<state>{}.edi.low_16; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::BP> { static constexpr auto val = to_register_state<state>{}.ebp.low_16; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::SP> { static constexpr auto val = to_register_state<state>{}.esp.low_16; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::IP> { static constexpr auto val = to_register_state<state>{}.eip.low_16; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::ESI> { static constexpr auto val = to_register_state<state>{}.esi.value; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::EDI> { static constexpr auto val = to_register_state<state>{}.edi.value; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::EBP> { static constexpr auto val = to_register_state<state>{}.ebp.value; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::ESP> { static constexpr auto val = to_register_state<state>{}.esp.value; };

        template<typename state>
        struct get_reg_impl<state, regs::id_t::EIP> { static constexpr auto val = to_register_state<state>{}.eip.value; };

    }

    template <typename state, regs::id_t id>
    constexpr auto get_reg = details::get_reg_impl<state, id>::val;


    namespace details
    {
        template <typename state, regs::id_t id, uint32_t val>
        struct set_reg_impl;

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::AL, val> { using type = typename to_register_state<state>::template set_al<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::AH, val> { using type = typename to_register_state<state>::template set_ah<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::AX, val> { using type = typename to_register_state<state>::template set_ax<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::EAX, val> { using type = typename to_register_state<state>::template set_eax<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::BL, val> { using type = typename to_register_state<state>::template set_bl<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::BH, val> { using type = typename to_register_state<state>::template set_bh<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::BX, val> { using type = typename to_register_state<state>::template set_bx<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::EBX, val> { using type = typename to_register_state<state>::template set_ebx<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::CL, val> { using type = typename to_register_state<state>::template set_cl<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::CH, val> { using type = typename to_register_state<state>::template set_ch<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::CX, val> { using type = typename to_register_state<state>::template set_cx<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::ECX, val> { using type = typename to_register_state<state>::template set_ecx<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::DL, val> { using type = typename to_register_state<state>::template set_dl<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::DH, val> { using type = typename to_register_state<state>::template set_dh<static_cast<uint8_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::DX, val> { using type = typename to_register_state<state>::template set_dx<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::EDX, val> { using type = typename to_register_state<state>::template set_edx<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::SP, val> { using type = typename to_register_state<state>::template set_sp<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::ESP, val> { using type = typename to_register_state<state>::template set_esp<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::BP, val> { using type = typename to_register_state<state>::template set_bp<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::EBP, val> { using type = typename to_register_state<state>::template set_ebp<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::SI, val> { using type = typename to_register_state<state>::template set_si<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::ESI, val> { using type = typename to_register_state<state>::template set_esi<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::DI, val> { using type = typename to_register_state<state>::template set_di<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::EDI, val> { using type = typename to_register_state<state>::template set_edi<val>; };


        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::IP, val> { using type = typename to_register_state<state>::template set_ip<static_cast<uint16_t>(val)>; };

        template <typename state, uint32_t val>
        struct set_reg_impl<state, regs::id_t::EIP, val> { using type = typename to_register_state<state>::template set_eip<val>; };
    }

    template <typename state, regs::id_t id, uint32_t val>
    using set_reg = typename details::set_reg_impl<state, id, val>::type;

    namespace tests
    {
        static_assert(get_reg<startup_registers_state, regs::id_t::AL> == 0xdd, "");
        static_assert(get_reg<set_reg<startup_registers_state, regs::id_t::AL, 0xab>, regs::id_t::AL> == 0xab, "");
        static_assert(get_reg<set_reg<startup_registers_state, regs::id_t::EAX, 0xddccbbaa>, regs::id_t::AL> == 0xaa, "");
    }
}


namespace cai
{
    template <typename ...args>
    struct tuple
    {};




    namespace details
    {
        template <typename, typename>
        struct tuple_append_impl;

        template <typename ...args, typename tail>
        struct tuple_append_impl<tuple<args...>, tail>
        {
            using type = tuple<args..., tail>;
        };
    }

    template <typename tup, typename tail>
    using tuple_append = typename details::tuple_append_impl<tup, tail>::type;




    namespace details
    {
        template <typename, typename>
        struct tuple_merge_impl;

        template <typename ...ltypes, typename ...rtypes>
        struct tuple_merge_impl<tuple<ltypes...>, tuple<rtypes...>>
        {
            using type = tuple<ltypes..., rtypes...>;
        };
    }

    template <typename l, typename r>
    using tuple_merge = typename details::tuple_merge_impl<l, r>::type;






    template <size_t ...values>
    struct values_container
    {};




    namespace details
    {
        template <typename, size_t>
        struct values_append_impl;

        template <size_t ...args, size_t tail>
        struct values_append_impl<values_container<args...>, tail>
        {
            using type = values_container<args..., tail>;
        };
    }

    template <typename tup, size_t tail>
    using values_append = typename details::values_append_impl<tup, tail>::type;




    namespace details
    {
        template <typename, typename>
        struct values_merge_impl;

        template <size_t ...ltypes, size_t ...rtypes>
        struct values_merge_impl<values_container<ltypes...>, values_container<rtypes...>>
        {
            using type = values_container<ltypes..., rtypes...>;
        };
    }

    template <typename l, typename r>
    using values_merge = typename details::values_merge_impl<l, r>::type;

    namespace details
    {
        template <size_t, bool, typename>
        struct values_drop_impl;

        template <size_t ...left>
        struct values_drop_impl<0, true, values_container<left...>>
        {
            using type = values_container<left...>;
        };

        template <size_t count, size_t to_drop, size_t ...left>
        struct values_drop_impl<count, false, values_container<to_drop, left...>>
        {
            using type = typename values_drop_impl<count-1,
                                          count == 1,
                                          values_container<left...>>::type;
        };
    }

    template <size_t count, typename values>
    using values_drop = typename details::values_drop_impl<count, count==0, values>::type;

}


namespace cai
{
    template <bool cf, bool zf, bool sf, bool of>
    struct flags
    {
        static constexpr auto CF = cf;
        static constexpr auto ZF = zf;
        static constexpr auto SF = sf;
        static constexpr auto OF = of;
    };

    using startup_flags_state = flags<false, false, false, false>;


    namespace details
    {
        template <typename>
        struct to_flags_impl;


        template <bool cf, bool zf, bool sf, bool of>
        struct to_flags_impl<
                flags<cf, zf, sf, of>>
        {
            using type = flags<cf, zf, sf, of>;
        };
    }

    template <typename flags_t>
    using to_flags = typename details::to_flags_impl<flags_t>::type;

    template <typename flags_t, bool value>
    using set_cf = flags<value, to_flags<flags_t>{}.ZF, to_flags<flags_t>{}.SF, to_flags<flags_t>{}.OF>;

    template <typename flags_t, bool value>
    using set_zf = flags<to_flags<flags_t>{}.CF, value, to_flags<flags_t>{}.SF, to_flags<flags_t>{}.OF>;

    template <typename flags_t, bool value>
    using set_sf = flags<to_flags<flags_t>{}.CF, to_flags<flags_t>{}.ZF, value, to_flags<flags_t>{}.OF>;

    template <typename flags_t, bool value>
    using set_of = flags<to_flags<flags_t>{}.CF, to_flags<flags_t>{}.ZF, to_flags<flags_t>{}.SF, value>;
}




namespace cai
{
    template <uint8_t ...values>
    struct stack
    {
        static constexpr auto size = sizeof...(values);
    };


    using startup_stack = stack<0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff>;

    namespace details
    {
        template <typename>
        struct to_stack_impl;


        template <uint8_t ...values>
        struct to_stack_impl<stack<values...>>
        {
            using type = stack<values...>;
        };
    }

    template <typename stack_t>
    using to_stack = typename details::to_stack_impl<stack_t>::type;




    namespace details
    {
        template <size_t count, bool end, uint8_t ...values>
        struct pop_front_impl;

        template <size_t count, bool end>
        struct pop_front_impl<count, end>
        {
            using type = startup_stack;
        };

        template <size_t count, uint8_t top, uint8_t ...values>
        struct pop_front_impl<count, true, top, values...>
        {
            using type = stack<top, values...>;
        };

        template <size_t count, uint8_t top, uint8_t ...values>
        struct pop_front_impl<count, false, top, values... >
        {
            using type = typename pop_front_impl<count-1, count == 1, values...>::type;
        };

        template <size_t s, typename v>
        struct stack_pop_impl;

        template <size_t pop_size, uint8_t ...values>
        struct stack_pop_impl<pop_size, stack<values...>>
        {
            static_assert(sizeof...(values) > pop_size, "");

            using new_stack = typename details::pop_front_impl<pop_size,
                                                               false,
                                                               values...>::type;
        };


    }

    template <typename pop_dest_t, typename stack_t>
    using stack_pop = typename details::stack_pop_impl<sizeof(pop_dest_t), stack_t>::new_stack;

    template <typename stack_t>
    using stack_pop_8 = stack_pop<uint8_t, stack_t>;

    template <typename stack_t>
    using stack_pop_16 = stack_pop<uint16_t, stack_t>;

    template <typename stack_t>
    using stack_pop_32 = stack_pop<uint32_t, stack_t>;






    namespace details
    {
        template<typename l, typename r>
        struct stack_merge_impl;

        template<uint8_t ...lvalues, uint8_t ...rvalues>
        struct stack_merge_impl<stack<lvalues...>, stack<rvalues...>>
        {
            using new_stack = stack<lvalues..., rvalues...>;
        };
    }

    template<typename l, typename r>
    using stack_merge = typename details::stack_merge_impl<l, r>::new_stack;






    template <typename val_type, uint32_t value>
    struct stack_from_value
    {
        using new_stack = typename std::conditional_t<sizeof(val_type) == 1,
                                                      stack<value & 0xff>,
                                                      typename std::conditional_t<sizeof(val_type) == 2,
                                                                                  stack<((value & 0xff00) >> 8), value & 0xff>,
                                                                                  stack<((value & 0xff000000) >> 24), ((value & 0xff0000) >> 16), ((value & 0xff00) >> 8), value & 0xff>>>;
    };






    template <typename push_t, uint32_t push_v, typename stack_t>
    struct stack_push_impl
    {
        using new_stack = stack_merge<typename stack_from_value<push_t, push_v>::new_stack,
                                      stack_t>;
    };


    template <uint32_t push_v, typename stack_t>
    using stack_push_8 = typename stack_push_impl<uint8_t, push_v, stack_t>::new_stack;

    template <uint32_t push_v, typename stack_t>
    using stack_push_16 = typename stack_push_impl<uint16_t, push_v, stack_t>::new_stack;

    template <uint32_t push_v, typename stack_t>
    using stack_push_32 = typename stack_push_impl<uint32_t, push_v, stack_t>::new_stack;



    namespace details
    {
        template <typename, uint32_t, typename>
        struct stack_push_impl;

        template<uint8_t val_to_push, uint8_t ...values>
        struct stack_push_impl<uint8_t, val_to_push, stack<values...>>
        {
            using type = stack_push_8<val_to_push, stack<values...>>;
        };

        template<uint16_t val_to_push, uint8_t ...values>
        struct stack_push_impl<uint16_t, val_to_push, stack<values...>>
        {
            using type = stack_push_16<val_to_push, stack<values...>>;
        };

        template<uint32_t val_to_push, uint8_t ...values>
        struct stack_push_impl<uint32_t, val_to_push, stack<values...>>
        {
            using type = stack_push_32<val_to_push, stack<values...>>;
        };
    }

    template <typename size_type, uint32_t val, typename stack_t>
    using stack_push = typename details::stack_push_impl<size_type, val, stack_t>::type;





    namespace details
    {
        template<typename>
        struct top_getter_8;

        template<typename>
        struct top_getter_16;

        template<typename>
        struct top_getter_32;

        template<uint8_t top, uint8_t ...values>
        struct top_getter_8<stack<top, values...>>
        {
            static constexpr uint8_t value = top;
        };

        template<uint8_t top_1, uint8_t top_0, uint8_t ...values>
        struct top_getter_16<stack<top_1, top_0, values...>>
        {
            static constexpr uint16_t value = (static_cast<uint16_t>(top_1) << 8) + static_cast<uint16_t>(top_0);
        };

        template<uint8_t top_3, uint8_t top_2, uint8_t top_1, uint8_t top_0, uint8_t ...values>
        struct top_getter_32<stack<top_3, top_2, top_1, top_0, values...>>
        {
            static constexpr uint32_t value = (static_cast<uint16_t>(top_3) << 24) + (static_cast<uint16_t>(top_2) << 16) + (static_cast<uint16_t>(top_1) << 8) + static_cast<uint16_t>(top_0);
        };
    }

    template <typename stack_t>
    constexpr uint8_t stack_top_8 = details::top_getter_8<stack_t>::value;

    template <typename stack_t>
    constexpr uint16_t stack_top_16 = details::top_getter_16<stack_t>::value;

    template <typename stack_t>
    constexpr uint32_t stack_top_32 = details::top_getter_32<stack_t>::value;



    namespace details
    {
        template <typename, typename>
        struct stack_top_impl;

        template<uint8_t ...values>
        struct stack_top_impl<uint8_t, stack<values...>>
        {
            static constexpr uint8_t val = stack_top_8<stack<values...>>;
        };

        template<uint8_t ...values>
        struct stack_top_impl<uint16_t, stack<values...>>
        {
            static constexpr uint16_t val = stack_top_16<stack<values...>>;
        };

        template<uint8_t ...values>
        struct stack_top_impl<uint32_t, stack<values...>>
        {
            static constexpr uint32_t val = stack_top_32<stack<values...>>;
        };
    }

    template <typename size_type, typename stack_t>
    constexpr auto stack_top = details::stack_top_impl<size_type, stack_t>::val;





    namespace details
    {
        template <size_t, bool, typename, typename>
        struct stack_take_impl;

        template <uint8_t ...taken, uint8_t ...left>
        struct stack_take_impl<0, true, stack<taken...>, stack<left...>>
        {
            using type = stack_take_impl;

            using taken_stack = stack<taken...>;
            using left_stack = stack<left...>;
        };

        template <size_t count, uint8_t to_take, uint8_t ...taken, uint8_t ...left>
        struct stack_take_impl<count, false, stack<taken...>, stack<to_take, left...>>
        {
            using type = stack_take_impl<count-1,
                                         count == 1,
                                         stack<taken..., to_take>,
                                         stack<left...>>;

            using taken_stack = typename type::taken_stack;
            using left_stack = typename type::left_stack;
        };
    }

    template <size_t count, typename stack_t>
    using stack_take = typename details::stack_take_impl<count, count==0, stack<>, stack_t>::type;





    namespace details
    {
        template <typename, uint32_t, size_t, typename>
        struct stack_set_impl;

        template <uint32_t val_to_set, size_t ptr, uint8_t ...values>
        struct stack_set_impl<uint8_t, val_to_set, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            using stack_after_pop = stack_pop_8<typename stack_take_t::type::left_stack>;

            using stack_after_push = stack_push_8<static_cast<uint8_t>(val_to_set), stack_after_pop>;
            using result_stack = stack_merge<typename stack_take_t::taken_stack, stack_after_push>;

        };

        template <uint32_t val_to_set, size_t ptr, uint8_t ...values>
        struct stack_set_impl<uint16_t, val_to_set, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            using stack_after_pop = stack_pop_16<typename stack_take_t::type::left_stack>;
            using stack_after_push = stack_push_16<static_cast<uint16_t>(val_to_set), stack_after_pop>;
            using result_stack = stack_merge<typename stack_take_t::taken_stack, stack_after_push>;
        };

        template <uint32_t val_to_set, size_t ptr, uint8_t ...values>
        struct stack_set_impl<uint32_t, val_to_set, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            using stack_after_pop = stack_pop_32<typename stack_take_t::type::left_stack>;
            using stack_after_push = stack_push_32<val_to_set, stack_after_pop>;
            using result_stack = stack_merge<typename stack_take_t::taken_stack, stack_after_push>;
        };
    }


    template <typename val_size_type, uint32_t val, size_t ptr, typename stack_t>
    using stack_set = typename details::stack_set_impl<val_size_type, val, ptr, stack_t>::result_stack;






    namespace details
    {
        template <typename, size_t, typename>
        struct stack_get_impl;

        template <size_t ptr, uint8_t ...values>
        struct stack_get_impl<uint8_t, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            static constexpr auto value = stack_top_8<typename stack_take_t::type::left_stack>;

        };

        template <size_t ptr, uint8_t ...values>
        struct stack_get_impl<uint16_t, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            static constexpr auto value = stack_top_16<typename stack_take_t::type::left_stack>;
        };

        template <size_t ptr, uint8_t ...values>
        struct stack_get_impl<uint32_t, ptr, stack<values...>>
        {
            using stack_t = stack<values...>;
            static constexpr auto to_take = stack_t::size - ptr - 1;

            using stack_take_t = stack_take<to_take, stack_t>;
            static constexpr auto value = stack_top_32<typename stack_take_t::type::left_stack>;
        };
    }


    template <typename val_size_type, size_t ptr, typename stack_t>
    constexpr auto stack_get = details::stack_get_impl<val_size_type, ptr, stack_t>::value;





    namespace tests
    {
        using test_stack = stack<0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77>;

        static_assert(stack_top_8<test_stack> == 0x00, "");
        static_assert(stack_top_16<test_stack> == 0x0011, "");
        static_assert(stack_top_32<test_stack> == 0x00112233, "");

        static_assert(stack_top_8<stack_pop_8<test_stack>> == 0x11, "");
        static_assert(stack_top_16<stack_pop_16<test_stack>> == 0x2233, "");
        static_assert(stack_top_32<stack_pop_32<test_stack>> == 0x44556677, "");

        static_assert(stack_top_8<stack_push_8<0xaa, test_stack>> == 0xaa, "");
        static_assert(stack_top_16<stack_push_16<0xaabb, test_stack>> == 0xaabb, "");
        static_assert(stack_top_32<stack_push_32<0xaabbccdd, test_stack>> == 0xaabbccdd, "");

        static_assert(
                std::is_same<stack_take<2, test_stack>::type::left_stack,
                             stack<0x22, 0x33, 0x44, 0x55, 0x66, 0x77>>::value
                , "");

        static_assert(
                std::is_same<stack_take<2, test_stack>::type::taken_stack ,
                             stack<0x00, 0x11>>::value
                , "");

        static_assert(
                std::is_same<stack_take<0, test_stack>::type::left_stack,
                        test_stack>::value
                , "");

        static_assert(
                std::is_same<stack_set<uint8_t, 0xff, 1, test_stack>,
                        stack<0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xff, 0x77>>::value
                , "");

    }
}

namespace cai
{
    template <typename stack_type, typename flags_type, typename registers_state_type>
    struct machine_state
    {
        using stack_t = stack_type;
        using flags_t = flags_type;
        using registers_state_t = registers_state_type;

        static constexpr to_flags<flags_t> flags{};
        static constexpr to_register_state <registers_state_t> reg_state{};
    };

    namespace details
    {
        template <typename>
        struct to_machine_state_impl;


        template <uint8_t ...stack_vals, bool ...flags_bools, uint32_t ...regs_values>
        struct to_machine_state_impl<machine_state<stack<stack_vals...>,
                flags<flags_bools...>,
                registers_state<regs_values...>>>
        {
            using type = machine_state<stack<stack_vals...>,
                    flags<flags_bools...>,
                    registers_state<regs_values...>>;
        };
    }

    template <typename state>
    using to_machine_state = typename details::to_machine_state_impl<state>::type;




    namespace details
    {
        template <typename state, typename opcodes>
        struct get_next_instruction_impl
        {
            using regs_sate = typename to_machine_state<state>::registers_state_t;
            static constexpr auto eip_val = get_reg<regs_sate, regs::id_t::EIP>;
            using type = values_drop<eip_val, opcodes>;
        };
    }

    template <typename state, typename opcodes>
    using get_next_instruction = typename details::get_next_instruction_impl<state, opcodes>::type;

    using startup_machine_state = machine_state<startup_stack, startup_flags_state, startup_registers_state>;
}




namespace cai {
    namespace execute {
        template<typename state, size_t ...opcodes>
        struct ex_instruction;
    }
}



namespace cai
{
    namespace instructions
    {
    }
}



namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg, size_t ...opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::POP_REG>, reg, opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val = stack_top<regs::reg_size_type<regs::to_id<reg>>, typename state::stack_t>;
            using new_stack = stack_pop<regs::reg_size_type<regs::to_id<reg>>, typename state::stack_t>;
            using new_regs_state_after_pop = set_reg<typename state::registers_state_t, regs::to_id<reg>, static_cast<uint32_t>(val)>;

            static constexpr auto eip_val = get_reg<new_regs_state_after_pop, regs::id_t::EIP> + 2;
            using new_regs_state = set_reg<new_regs_state_after_pop, regs::id_t::EIP, eip_val>;

            using next_machine_state = machine_state<new_stack, typename state::flags_t, new_regs_state>;
        };

        namespace tests
        {
            static_assert(get_reg<
                    ex_instruction<
                            startup_machine_state,
                            inst::to_size<inst::id_t::POP_REG>,
                            regs::to_size<regs::id_t::AL>
                    >::next_machine_state::registers_state_t,
                    regs::id_t::AL
            > == 0x00 ,"");

            static_assert(get_reg<
                    ex_instruction<
                            startup_machine_state,
                            inst::to_size<inst::id_t::POP_REG>,
                            regs::to_size<regs::id_t::AX>
                    >::next_machine_state::registers_state_t,
                    regs::id_t::AX
            > == 0x0011 ,"");

            static_assert(get_reg<
                    ex_instruction<
                            startup_machine_state,
                            inst::to_size<inst::id_t::POP_REG>,
                            regs::to_size<regs::id_t::EAX>
                    >::next_machine_state::registers_state_t,
                    regs::id_t::EAX
            > == 0x00112233 ,"");
        }
    }
}






namespace cai
{
    namespace execute
    {
        template <typename state_t, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::PUSH_REG>, reg, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            using new_stack = stack_push<regs::reg_size_type<regs::to_id<reg>>,
                        reg_val,
                        typename state::stack_t>;

            static constexpr auto eip_val = get_reg<typename state::registers_state_t, regs::id_t::EIP> + 2;
            using new_regs_state = set_reg<typename state::registers_state_t, regs::id_t::EIP, eip_val>;

            using next_machine_state = machine_state<new_stack,
                                                     typename state::flags_t,
                                                     new_regs_state>;
        };


        namespace tests
        {


            static_assert(stack_top<regs::reg_size_type<regs::id_t::AL>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AL>>::next_machine_state>::stack_t> == 0xdd, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AH>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AH>>::next_machine_state>::stack_t> == 0xcc, "");



            static_assert(stack_top<regs::reg_size_type<regs::id_t::AH>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AX>>::next_machine_state>::stack_t> == 0xcc, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AX>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::AX>>::next_machine_state>::stack_t> == 0xccdd, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AL>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::EAX>>::next_machine_state>::stack_t> == 0xaa, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::AX>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::EAX>>::next_machine_state>::stack_t> == 0xaabb, "");

            static_assert(stack_top<regs::reg_size_type<regs::id_t::EAX>,
                                    to_machine_state<typename ex_instruction<startup_machine_state,
                                                                             inst::to_size<inst::id_t::PUSH_REG>,
                                                                             regs::to_size<regs::id_t::EAX>>::next_machine_state>::stack_t> == 0xaabbccdd, "");
        }
    }
}

namespace cai
{
    namespace details
    {
        template <inst::id_t instruction_opcode>
        constexpr size_t eip_change{0};

        template <> constexpr size_t eip_change<inst::id_t::ADD_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::ADD_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::CMP_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::EXIT> = 1;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::PUSH_REG> = 2;
        template <> constexpr size_t eip_change<inst::id_t::POP_REG> = 2;
        template <> constexpr size_t eip_change<inst::id_t::JMP> = 2;
        template <> constexpr size_t eip_change<inst::id_t::JGE> = 2;
    }

    template <inst::id_t instruction>
    constexpr auto get_eip_change = details::eip_change<instruction>;
}

namespace cai
{
    namespace details
    {
        template <typename regs_state_t, inst::id_t instruction_opcode>
        struct adjust_eip_impl
        {
            static constexpr auto eip_val = get_reg<regs_state_t, regs::id_t::EIP> + eip_change<instruction_opcode>;
            using regs_state = set_reg<regs_state_t, regs::id_t::EIP, eip_val>;
        };
    }

    template <typename regs_state_t, inst::id_t instruction>
    using adjust_eip = typename details::adjust_eip_impl<regs_state_t, instruction>::regs_state;
}

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::ADD_REG_REG>, reg1, reg2, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val1 = get_reg<typename state::registers_state_t, regs::to_id<reg1>>;
            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val1 + val2)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::ADD_REG_REG>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        template<typename state_t, size_t reg1, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::ADD_REG_VAL>, reg1, val, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val1 = get_reg<typename state::registers_state_t, regs::to_id<reg1>>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val1 + val)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::ADD_REG_REG>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        namespace tests
        {
            static_assert(get_reg<ex_instruction<startup_machine_state,
                                                 inst::to_size<inst::id_t::ADD_REG_REG>,
                                                 regs::to_size<regs::id_t::AL>,
                                                 regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                                  regs::id_t::AL> == static_cast<uint8_t>(0xcc + 0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::ADD_REG_REG>,
                          regs::to_size<regs::id_t::AH>,
                          regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                          regs::id_t::AH> == static_cast<uint8_t>(0xcc + 0xcc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::ADD_REG_REG>,
                          regs::to_size<regs::id_t::AX>,
                          regs::to_size<regs::id_t::BX>>::next_machine_state::registers_state_t,
                          regs::id_t::AX> == static_cast<uint16_t>(0xccdd + static_cast<uint16_t>(0xbada55))
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::ADD_REG_REG>,
                          regs::to_size<regs::id_t::EAX>,
                          regs::to_size<regs::id_t::EBX>>::next_machine_state::registers_state_t,
                          regs::id_t::EAX> == static_cast<uint32_t>(0xaabbccdd + 0xbada55)
                    ,"");
        }
    }
}

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::SUB_REG_REG>, reg1, reg2>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val1 = get_reg<typename state::registers_state_t, regs::to_id<reg1>>;
            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val1 - val2)>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, new_regs_state>;
        };

        namespace tests
        {
            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::SUB_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                          regs::id_t::AL> == static_cast<uint8_t>(0xdd - 0xcc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::SUB_REG_REG>,
                          regs::to_size<regs::id_t::AH>,
                          regs::to_size<regs::id_t::AL>>::next_machine_state::registers_state_t,
                          regs::id_t::AH> == static_cast<uint8_t>(0xcc - 0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                                  inst::to_size<inst::id_t::SUB_REG_REG>,
                                  regs::to_size<regs::id_t::AH>,
                                  regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                                  regs::id_t::AH> == static_cast<uint8_t>(0xcc - 0xcc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                                  inst::to_size<inst::id_t::SUB_REG_REG>,
                                  regs::to_size<regs::id_t::AX>,
                                  regs::to_size<regs::id_t::BX>>::next_machine_state::registers_state_t,
                                  regs::id_t::AX> == static_cast<uint16_t>(0xccdd - static_cast<uint16_t>(0xbada55))
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                                  inst::to_size<inst::id_t::SUB_REG_REG>,
                                  regs::to_size<regs::id_t::EAX>,
                                  regs::to_size<regs::id_t::EBX>>::next_machine_state::registers_state_t,
                                  regs::id_t::EAX> == static_cast<uint32_t>(0xaabbccdd - 0xbada55)
                    ,"");
        }
    }
}

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::CMP_REG_REG>, reg1, reg2, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val1 = get_reg<typename state::registers_state_t, regs::to_id<reg1>>;
            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>;
            static constexpr int64_t result = static_cast<int64_t>(val1) - static_cast<int64_t>(val2);

            static constexpr bool zf = (val1 == val2);
            static constexpr bool cf = (static_cast<uint64_t>(val1) < static_cast<uint64_t>(val2));
            static constexpr bool sf = (result < 0);
            static constexpr bool of = (result < static_cast<int64_t>(std::numeric_limits<int32_t>::min()) ||
                                        result > static_cast<int64_t>(std::numeric_limits<int32_t>::max()));

            using new_flags_state = flags<cf, zf, sf, of>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::CMP_REG_REG>;

            using next_machine_state = machine_state<typename state::stack_t, new_flags_state, final_regs_state>;
        };

        namespace tests
        {
            using cmp_test_machine_state = machine_state<startup_stack, startup_flags_state, registers_state<0x0,0xaaaabbbb, 4, 5,0,0,0,0, 0>>;

            static_assert(
                    ex_instruction<
                            cmp_test_machine_state,
                            inst::to_size<inst::id_t::CMP_REG_REG>,
                            regs::to_size<regs::id_t::AL>,
                            regs::to_size<regs::id_t::AH>>::next_machine_state::flags.CF == false
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::AL>,
                    regs::to_size<regs::id_t::AH>>::next_machine_state::flags.ZF == true
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::BX>,
                    regs::to_size<regs::id_t::AX>>::next_machine_state::flags.CF == false
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::AX>,
                    regs::to_size<regs::id_t::BX>>::next_machine_state::flags.CF == true
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::EAX>,
                    regs::to_size<regs::id_t::EBX>>::next_machine_state::flags.CF == true
                    ,"");

            static_assert(
                    ex_instruction<
                    cmp_test_machine_state,
                    inst::to_size<inst::id_t::CMP_REG_REG>,
                    regs::to_size<regs::id_t::ECX>,
                    regs::to_size<regs::id_t::EDX>>::next_machine_state::flags.SF == true
                    ,"");
        }
    }
}

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t reg1, size_t reg2, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_REG>, reg1, reg2, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto val2 = get_reg<typename state::registers_state_t, regs::to_id<reg2>>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val2)>;
            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_REG>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        template<typename state_t, size_t reg1, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_VAL>, reg1, val, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg1>, static_cast<uint32_t>(val)>;
            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_VAL>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };



        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const>, mem_ptr_reg, mem_ptr_const, mem_size, val, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val - mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };


        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t val, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>, mem_ptr_reg, mem_ptr_const, mem_size, val, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val + mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };


        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const>, mem_ptr_reg, mem_ptr_const, mem_size, reg, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto mem_ptr = mem_reg_val - mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(reg_val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };


        template<typename state_t, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t reg, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>, mem_ptr_reg, mem_ptr_const, mem_size, reg, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto mem_ptr = mem_reg_val + mem_ptr_const;

            using stack_after_set = stack_set<memory::to_mem_type<memory::to_id<mem_size>>, static_cast<uint32_t>(reg_val), mem_ptr, typename state::stack_t>;

            using final_regs_state = adjust_eip<typename state::registers_state_t, inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>;

            using next_machine_state = machine_state<stack_after_set, typename state::flags_t, final_regs_state>;
        };


        template<typename state_t, size_t reg, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>, reg, mem_ptr_reg, mem_ptr_const, mem_size, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val - mem_ptr_const;
            static constexpr auto mem_val = stack_get<memory::to_mem_type<memory::to_id<mem_size>>, mem_ptr, typename state::stack_t>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg>, static_cast<uint32_t>(mem_val)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };


        template<typename state_t, size_t reg, size_t mem_ptr_reg, size_t mem_ptr_const, size_t mem_size, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const>, reg, mem_ptr_reg, mem_ptr_const, mem_size, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto mem_reg_val = get_reg<typename state::registers_state_t, regs::to_id<mem_ptr_reg>>;
            static constexpr auto mem_ptr = mem_reg_val + mem_ptr_const;
            static constexpr auto mem_val = stack_get<memory::to_mem_type<memory::to_id<mem_size>>, mem_ptr, typename state::stack_t>;

            using new_regs_state = set_reg<typename state::registers_state_t, regs::to_id<reg>, static_cast<uint32_t>(mem_val)>;

            using final_regs_state = adjust_eip<new_regs_state, inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };




        namespace tests
        {
            static_assert(get_reg<ex_instruction<startup_machine_state,
                                  inst::to_size<inst::id_t::MOV_REG_REG>,
                                  regs::to_size<regs::id_t::AL>,
                                  regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                                  regs::id_t::AL> == static_cast<uint8_t>(0xcc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::AX>>::next_machine_state::registers_state_t,
                          regs::id_t::AL> == static_cast<uint8_t>(0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::EAX>>::next_machine_state::registers_state_t,
                          regs::id_t::AL> == static_cast<uint8_t>(0xdd)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AL>,
                          regs::to_size<regs::id_t::AH>>::next_machine_state::registers_state_t,
                          regs::id_t::EAX> == static_cast<uint32_t>(0xaabbcccc)
                    ,"");

            static_assert(get_reg<ex_instruction<startup_machine_state,
                          inst::to_size<inst::id_t::MOV_REG_REG>,
                          regs::to_size<regs::id_t::AX>,
                          regs::to_size<regs::id_t::AL>>::next_machine_state::registers_state_t,
                          regs::id_t::AX> == static_cast<uint32_t>(0xdd)
                    ,"");

            using mov_tests_machine_state = machine_state<startup_stack, startup_flags_state, registers_state<6, 0xaabbccdd,0,0,0,0,0,0, 0>>;



            static_assert(stack_get<uint8_t,
                                    5,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_8>,
                                                   0xfa>::next_machine_state::stack_t> == 0xfa
                    ,"");


            static_assert(stack_get<uint8_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_8>,
                                                   0xfa>::next_machine_state::stack_t> == 0xfa
                    ,"");


            static_assert(stack_get<uint16_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_16>,
                                                   0xfafb>::next_machine_state::stack_t> == 0xfafb
                    ,"");


            static_assert(stack_get<uint32_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_32>,
                                                   0xfafbfcfd>::next_machine_state::stack_t> == 0xfafbfcfd
                    ,"");




            static_assert(stack_get<uint32_t,
                                    5,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_32>,
                                                   regs::to_size<regs::id_t::EBX>>::next_machine_state::stack_t> == 0xaabbccdd
                    ,"");


            static_assert(stack_get<uint32_t,
                                    7,
                                    ex_instruction<mov_tests_machine_state,
                                                   inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>,
                                                   regs::to_size<regs::id_t::EAX>,
                                                   1,
                                                   memory::to_size<memory::id_t::s_32>,
                                                   regs::to_size<regs::id_t::EBX>>::next_machine_state::stack_t> == 0xaabbccdd
                    ,"");


            static_assert(get_reg<ex_instruction<mov_tests_machine_state,
                                                 inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 1,
                                                 memory::to_size<memory::id_t::s_32>>::next_machine_state::registers_state_t,
                                                 regs::id_t::EAX> == 0xaabbccdd
                    ,"");


            static_assert(get_reg<ex_instruction<mov_tests_machine_state,
                                                 inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 regs::to_size<regs::id_t::EAX>,
                                                 1,
                                                 memory::to_size<memory::id_t::s_32>>::next_machine_state::registers_state_t,
                                                 regs::id_t::EAX> == 0x8899aabb
                    ,"");
        }
    }
}

namespace cai
{
    namespace execute
    {

        template<typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MUL_REG_REG>, regs::to_size<regs::id_t::AL>, reg>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto al_val = get_reg<typename state::registers_state_t, regs::id_t::AL>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto result = static_cast<uint16_t>(al_val) * static_cast<uint16_t>(reg_val);

            using new_regs_state = set_reg<typename state::registers_state_t, regs::id_t::AX, static_cast<uint32_t>(result)>;

            static constexpr bool cf = ((result & 0xff00) != 0);

            using new_flags_state = set_cf<typename state::flags_t, cf>;

            using next_machine_state = machine_state<typename state::stack_t, new_flags_state, new_regs_state>;
        };


        template<typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MUL_REG_REG>, regs::to_size<regs::id_t::AX>, reg>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto ax_val = get_reg<typename state::registers_state_t, regs::id_t::AX>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto result = static_cast<uint32_t>(ax_val) * static_cast<uint32_t>(reg_val);

            using regs_state_after_ax = set_reg<typename state::registers_state_t, regs::id_t::AX, result & 0xffff>;
            using regs_state_after_dx = set_reg<regs_state_after_ax, regs::id_t::DX, ((result & 0xffff0000) >> 16)>;

            static constexpr bool cf = ((result & 0xffff0000) != 0);

            using new_flags_state = set_cf<typename state::flags_t, cf>;

            using next_machine_state = machine_state<typename state::stack_t, new_flags_state, regs_state_after_dx>;
        };


        template<typename state_t, size_t reg>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::MUL_REG_REG>, regs::to_size<regs::id_t::EAX>, reg>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto eax_val = get_reg<typename state::registers_state_t, regs::id_t::EAX>;
            static constexpr auto reg_val = get_reg<typename state::registers_state_t, regs::to_id<reg>>;
            static constexpr auto result = static_cast<uint64_t>(eax_val) * static_cast<uint64_t>(reg_val);

            using regs_state_after_eax = set_reg<typename state::registers_state_t, regs::id_t::EAX, result & 0xffffffff>;
            using regs_state_after_edx = set_reg<regs_state_after_eax, regs::id_t::EDX, ((result & 0xffffffff00000000) >> 32)>;

            static constexpr bool cf = ((result & 0xffff0000) != 0);

            using new_flags_state = set_cf<typename state::flags_t, cf>;

            using next_machine_state = machine_state<typename state::stack_t, new_flags_state, regs_state_after_edx>;
        };

        namespace tests
        {

            using mul_tests_startup_machine_state = machine_state<startup_stack, startup_flags_state, registers_state<0x00000002, 0x00000102, 0x000000ff, 0xffffffff,0,0,0,0, 0>>;

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                                  regs::to_size<regs::id_t::AL>,
                                  regs::to_size<regs::id_t::BL>>::next_machine_state::registers_state_t,
                                  regs::id_t::AX> == static_cast<uint16_t>(0x02 * 0x02)
                    ,"");

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                          regs::to_size<regs::id_t::AX>,
                          regs::to_size<regs::id_t::BX>>::next_machine_state::registers_state_t,
                          regs::id_t::AX> == static_cast<uint16_t>(0x0002 * 0x0102)
                    ,"");

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                          regs::to_size<regs::id_t::EAX>,
                          regs::to_size<regs::id_t::EDX>>::next_machine_state::registers_state_t,
                          regs::id_t::EAX> == static_cast<uint32_t>(0x00000002 * 0xffffffff)
                    ,"");

            static_assert(get_reg<ex_instruction<mul_tests_startup_machine_state,
                                  inst::to_size<inst::id_t::MUL_REG_REG>,
                          regs::to_size<regs::id_t::EAX>,
                          regs::to_size<regs::id_t::EDX>>::next_machine_state::registers_state_t,
                          regs::id_t::EDX> == static_cast<uint32_t>(((static_cast<uint64_t>(0x00000002) * static_cast<uint64_t>(0xffffffff)) & 0xffffffff00000000) >> 32)
                    ,"");

        }
    }
}

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t ip, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::JMP>, ip, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            using final_regs_state = set_reg<typename state::registers_state_t, regs::id_t::EIP, static_cast<uint32_t>(ip)>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        namespace tests
        {
        }
    }
}

namespace cai
{
    namespace execute
    {
        template<typename state_t, size_t ip, size_t ...rest_of_opcodes>
        struct ex_instruction<state_t, inst::to_size<inst::id_t::JGE>, ip, rest_of_opcodes...>
        {
            using state = to_machine_state<state_t>;

            static constexpr auto flags_v = typename state::flags_t{};

            static constexpr bool should_jmp = ((static_cast<int>(flags_v.SF) ^ static_cast<int>(flags_v.OF)) == 0);
            using final_regs_state = std::conditional_t<
                    should_jmp,
                    set_reg<typename state::registers_state_t, regs::id_t::EIP, static_cast<uint32_t>(ip)>,
                    adjust_eip<typename state::registers_state_t, inst::id_t::JGE>>;

            using next_machine_state = machine_state<typename state::stack_t, typename state::flags_t, final_regs_state>;
        };

        namespace tests
        {
        }
    }
}

namespace cai
{
    namespace details
    {
        template <typename, typename>
        struct execute_instruction_impl;

        template <typename state, size_t ...opcodes>
        struct execute_instruction_impl<state, values_container<opcodes...>>
        {
            using next_machine_state = typename execute::ex_instruction<state, opcodes...>::next_machine_state;
        };
    }

    template <typename state, typename opcodes>
    using execute_instruction = typename details::execute_instruction_impl<state, opcodes>::next_machine_state;
}

namespace cai
{



    namespace details
    {
        template <typename str, typename curr_token = string<>>
        struct get_token_impl;

        template <typename current_token>
        struct get_token_impl<string<>, current_token>
        {
            using result_token = current_token;
            using rest_of_string = string<>;
        };

        template <char ...str_chars, typename current_token>
        struct get_token_impl<string<' ', str_chars...>, current_token>
        {
            using result_token = current_token;
            using rest_of_string = string<str_chars...>;
        };

        template <char curr_char, char ...str_chars, typename current_token>
        struct get_token_impl<string<curr_char, str_chars...>, current_token>
        {
            using string_without_token_char = string<str_chars...>;

            using result_t = get_token_impl<string_without_token_char, string_append<current_token, curr_char>>;

            using result_token = typename result_t::result_token;
            using rest_of_string = typename result_t::rest_of_string;
        };
    }

    template <typename s>
    using get_token = details::get_token_impl<s>;

    namespace details
    {
        template <typename s, typename current_tokens = tuple<>>
        struct tokenize_impl;

        template <typename current_tokens>
        struct tokenize_impl<string<>, current_tokens>
        {
            using tokens = current_tokens;
        };

        template <char ...str_chars, typename current_tokens>
        struct tokenize_impl<string<str_chars...>, current_tokens>
        {
            using str = string<str_chars...>;
            using get_token_t = get_token<str>;

            using next_tokens = tuple_append<current_tokens, typename get_token_t::result_token>;
            using next_string = typename get_token_t::rest_of_string;

            using tokens = typename tokenize_impl<next_string, next_tokens>::tokens;
        };
    }

    template <typename s>
    using tokenize = typename details::tokenize_impl<s>::tokens;

    namespace tests
    {
        static_assert(std::is_same<typename get_token<decltype("abc def"_s)>::result_token,
                                   decltype("abc"_s)>::value, "");

        static_assert(std::is_same<typename get_token<decltype("abc def"_s)>::rest_of_string,
                                   decltype("def"_s)>::value, "");

        static_assert(std::is_same<tokenize<decltype("abc def"_s)>,
                                   tuple<decltype("abc"_s), decltype("def"_s)>>::value, "");
    }
}

namespace cai
{
    namespace tokens
    {
        using tok_empty = decltype(""_s);

        using tok_exit = decltype("exit"_s);

        using tok_mov = decltype("mov"_s);
        using tok_add = decltype("add"_s);
        using tok_push = decltype("push"_s);
        using tok_pop = decltype("pop"_s);
        using tok_sub = decltype("sub"_s);
        using tok_mul = decltype("mul"_s);
        using tok_cmp = decltype("cmp"_s);
        using tok_jmp= decltype("jmp"_s);
        using tok_jge = decltype("jge"_s);

        using tok_eax = decltype("eax"_s);
        using tok_ebx = decltype("ebx"_s);
        using tok_ecx = decltype("ecx"_s);
        using tok_edx = decltype("edx"_s);
        using tok_esp = decltype("esp"_s);
        using tok_ebp = decltype("ebp"_s);
        using tok_edi = decltype("edi"_s);
        using tok_esi = decltype("esi"_s);
        using tok_eip = decltype("eip"_s);

        using tok_byte = decltype("BYTE"_s);
        using tok_word = decltype("WORD"_s);
        using tok_dword = decltype("DWORD"_s);
        using tok_ptr = decltype("PTR"_s);

        using tok_square_bracket_open = decltype("["_s);
        using tok_square_bracket_close = decltype("]"_s);
        using tok_minus = decltype("-"_s);
        using tok_plus = decltype("+"_s);
        using tok_comma = decltype(","_s);

        template <typename value>
        struct reg_token : to_string<value>
        {
            using str_type = to_string<value>;
        };



    }

    template <typename type>
    constexpr auto is_reg_token = is_type_in_v<type,
            tokens::tok_eax,
            tokens::tok_ebx,
            tokens::tok_ecx,
            tokens::tok_edx,
            tokens::tok_esp,
            tokens::tok_ebp,
            tokens::tok_edi,
            tokens::tok_esi,
            tokens::tok_eip>;

    namespace details
    {
        template <typename>
        struct token_to_reg_opcode_impl;

        template <> struct token_to_reg_opcode_impl<tokens::tok_eax> { static constexpr auto value = regs::to_size<regs::id_t::EAX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ebx> { static constexpr auto value = regs::to_size<regs::id_t::EBX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ecx> { static constexpr auto value = regs::to_size<regs::id_t::ECX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_edx> { static constexpr auto value = regs::to_size<regs::id_t::EDX>; };

        template <> struct token_to_reg_opcode_impl<tokens::tok_esp> { static constexpr auto value = regs::to_size<regs::id_t::ESP>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ebp> { static constexpr auto value = regs::to_size<regs::id_t::EBP>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_edi> { static constexpr auto value = regs::to_size<regs::id_t::EDI>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_esi> { static constexpr auto value = regs::to_size<regs::id_t::ESI>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_eip> { static constexpr auto value = regs::to_size<regs::id_t::EIP>; };
    }

    template <typename token>
    constexpr auto token_to_reg_opcode = details::token_to_reg_opcode_impl<token>::value;
}

namespace cai
{
    namespace details
    {
        template <typename tokens>
        struct matcher_impl;

        template <typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_exit, rest_of_tokens...>>
        {
            using instruction = values_container<inst::to_size<inst::id_t::EXIT>>;
            static constexpr auto eip_change = get_eip_change<inst::id_t::EXIT>;

            using instruction_tokens = tuple<tokens::tok_exit>;
            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;
}

namespace cai
{
    namespace details
    {
        template <typename token>
        struct reg_token_decoder
        {
            static constexpr size_t value = token_to_reg_opcode<token>;
        };

        template <typename token>
        struct const_val_token_decoder
        {
            static constexpr size_t value = string_to_int<token>;
        };

        template <typename>
        struct operand_decoder_impl;

        template <typename str>
        struct operand_decoder_impl
        {
            static constexpr size_t value =
                    std::conditional_t<
                            is_reg_token<str>,
                                reg_token_decoder<str>,
                                const_val_token_decoder<str>>::value;
        };
    }

    template <typename token>
    constexpr auto operand_decoder = details::operand_decoder_impl<token>::value;

    namespace tests
    {
        static_assert(operand_decoder<decltype("eax"_s)> == regs::to_size<regs::id_t::EAX>,"");
        static_assert(operand_decoder<decltype("2"_s)> == 2,"");
        static_assert(operand_decoder<decltype("-2"_s)> == -2,"");
    }
}

namespace cai
{
    namespace details
    {

        template <typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_push, operand, rest_of_tokens...>>
        {
            using instruction = values_container<
                    inst::to_size<inst::id_t::PUSH_REG>,
                    operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<inst::id_t::PUSH_REG>;
            using instruction_tokens = tuple<tokens::tok_push, operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_push, tokens::tok_eax, string<>, string<>>>::instruction,
                values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}

namespace cai
{
    namespace details
    {

        template <typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<tokens::tok_pop, operand, rest_of_tokens...>>
        {
            using instruction = values_container<
                    inst::to_size<inst::id_t::POP_REG>,
                    operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<inst::id_t::POP_REG>;
            using instruction_tokens = tuple<tokens::tok_pop, operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_pop, tokens::tok_eax, string<>, string<>>>::instruction,
                values_container<inst::to_size<inst::id_t::POP_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}

namespace cai
{
    namespace details
    {

        template <typename>
        struct plus_minus_decoder_impl;

        template <typename str>
        struct plus_minus_decoder_impl
        {
            static constexpr bool value = std::is_same<str, tokens::tok_plus>::value;
        };
    }

    template <typename token>
    constexpr auto is_plus = details::plus_minus_decoder_impl<token>::value;
}

namespace cai
{
    namespace details
    {

        template <typename>
        struct mem_size_decoder_impl;

        template <typename str>
        struct mem_size_decoder_impl
        {
            static constexpr auto value = std::is_same<str, tokens::tok_byte>::value ? memory::id_t::s_8
                                                                                     : std::is_same<str, tokens::tok_word>::value ? memory::id_t::s_16
                                                                                                                                  : memory::id_t::s_32;
        };
    }

    template <typename token>
    constexpr auto mem_size_decoder = details::mem_size_decoder_impl<token>::value;
}

namespace cai
{
    namespace details
    {

        template <typename reg_token, typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_mov,
                reg_token,
                tokens::tok_comma,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_reg_token<operand> ? inst::id_t::MOV_REG_REG : inst::id_t::MOV_REG_VAL;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_opcode<reg_token>,
                    operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<inst::id_t::MOV_REG_REG>;
            using instruction_tokens = tuple<
                    tokens::tok_mov,
                    reg_token,
                    tokens::tok_comma,
                    operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };


        template <typename reg_token, typename mem_size_token, typename mem_ptr_reg, typename mem_ptr_const, typename plus_minus_token, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_mov,
                reg_token,
                tokens::tok_comma,
                mem_size_token,
                tokens::tok_ptr,
                tokens::tok_square_bracket_open,
                mem_ptr_reg,
                plus_minus_token,
                mem_ptr_const,
                tokens::tok_square_bracket_close,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_plus<plus_minus_token> ? inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const
                                                                               : inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_opcode<reg_token>,
                    token_to_reg_opcode<mem_ptr_reg>,
                    string_to_int<mem_ptr_const>,
                    memory::to_size<mem_size_decoder<mem_size_token>>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_mov,
                    reg_token,
                    tokens::tok_comma,
                    mem_size_token,
                    tokens::tok_ptr,
                    tokens::tok_square_bracket_open,
                    mem_ptr_reg,
                    plus_minus_token,
                    mem_ptr_const,
                    tokens::tok_square_bracket_close>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };


        template <typename reg_or_val, typename mem_size_token, typename mem_ptr_reg, typename mem_ptr_const, typename plus_minus_token, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_mov,
                mem_size_token,
                tokens::tok_ptr,
                tokens::tok_square_bracket_open,
                mem_ptr_reg,
                plus_minus_token,
                mem_ptr_const,
                tokens::tok_square_bracket_close,
                tokens::tok_comma,
                reg_or_val,
                rest_of_tokens...>>
        {
            static constexpr bool plus = is_plus<plus_minus_token>;
            static constexpr bool is_reg = is_reg_token<reg_or_val>;


            static constexpr auto instruction_type = plus
                                                     ? is_reg
                                                       ? inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const
                                                       : inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const
                                                     : is_reg
                                                       ? inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const
                                                       : inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_opcode<mem_ptr_reg>,
                    string_to_int<mem_ptr_const>,
                    memory::to_size<mem_size_decoder<mem_size_token>>,
                    operand_decoder<reg_or_val>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_mov,
                    mem_size_token,
                    tokens::tok_ptr,
                    tokens::tok_square_bracket_open,
                    mem_ptr_reg,
                    plus_minus_token,
                    mem_ptr_const,
                    tokens::tok_square_bracket_close,
                    tokens::tok_comma,
                    reg_or_val>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_pop, tokens::tok_eax, string<>, string<>>>::instruction,
                values_container<inst::to_size<inst::id_t::POP_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");

        static_assert(std::is_same<instruction_match<tuple<
                decltype("mov"_s),
                decltype("eax"_s),
                decltype(","_s),
                decltype("2"_s)>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::MOV_REG_VAL>,
                        regs::to_size<regs::id_t::EAX>,
                        2
                >>::value, "");

        static_assert(std::is_same<instruction_match<tuple<
                decltype("mov"_s),
                decltype("eax"_s),
                decltype(","_s),
                decltype("BYTE"_s),
                decltype("PTR"_s),
                decltype("["_s),
                decltype("edx"_s),
                decltype("-"_s),
                decltype("2"_s),
                decltype("]"_s)>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const>,
                        regs::to_size<regs::id_t::EAX>,
                        regs::to_size<regs::id_t::EDX>,
                        2,
                        memory::to_size<memory::id_t::s_8>
                >>::value, "");

        static_assert(std::is_same<instruction_match<tuple<
                decltype("mov"_s),
                decltype("BYTE"_s),
                decltype("PTR"_s),
                decltype("["_s),
                decltype("edx"_s),
                decltype("-"_s),
                decltype("2"_s),
                decltype("]"_s),
                decltype(","_s),
                decltype("eax"_s)>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const>,
                        regs::to_size<regs::id_t::EDX>,
                        2,
                        memory::to_size<memory::id_t::s_8>,
                        regs::to_size<regs::id_t::EAX>
                >>::value, "");

        static_assert(std::is_same<instruction_match<tuple<
                decltype("mov"_s),
                decltype("BYTE"_s),
                decltype("PTR"_s),
                decltype("["_s),
                decltype("edx"_s),
                decltype("+"_s),
                decltype("2"_s),
                decltype("]"_s),
                decltype(","_s),
                decltype("eax"_s)>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>,
                        regs::to_size<regs::id_t::EDX>,
                        2,
                        memory::to_size<memory::id_t::s_8>,
                        regs::to_size<regs::id_t::EAX>
                >>::value, "");
    }
}

namespace cai
{
    namespace details
    {

        template <typename reg_token, typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_add,
                reg_token,
                tokens::tok_comma,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_reg_token<operand> ? inst::id_t::ADD_REG_REG : inst::id_t::ADD_REG_VAL;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_opcode<reg_token>,
                    operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_add,
                    reg_token,
                    tokens::tok_comma,
                    operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_add, tokens::tok_ebx, tokens::tok_comma, tokens::tok_eax , string<>, string<>>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::ADD_REG_REG>,
                        regs::to_size<regs::id_t::EBX>,
                        regs::to_size<regs::id_t::EAX>
                                >>::value, "");
    }
}

namespace cai
{
    namespace details
    {

        template <typename ip, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_jmp,
                ip,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JMP;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    operand_decoder<ip>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_jmp,
                    ip>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };


        template <char ...label_chars, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_jmp,
                string<'.', label_chars...>,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JMP;

            using instruction = values_container<
            inst::to_size<instruction_type>, 0>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_jmp,
                    string<'.', label_chars...>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_jmp, decltype("22"_s), string<>, string<>>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::JMP>,
                        22
                >>::value, "");
    }
}

namespace cai
{
    namespace details
    {

        template <typename ip, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_jge,
                ip,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JGE;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    operand_decoder<ip>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_add,
                    ip>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };


        template <char ...label_chars, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                            tokens::tok_jge,
                string<'.', label_chars...>,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::JGE;

            using instruction = values_container<
            inst::to_size<instruction_type>, 0>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_jge,
                    string<'.', label_chars...>>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_jge, decltype("22"_s), string<>, string<>>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::JGE>,
                        22
                >>::value, "");
    }
}

namespace cai
{
    namespace details
    {

        template <typename reg_token, typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_cmp,
                reg_token,
                tokens::tok_comma,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::CMP_REG_REG;

            using instruction = values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_opcode<reg_token>,
                    operand_decoder<operand>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_cmp,
                    reg_token,
                    tokens::tok_comma,
                    operand>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };
    }

    template <typename tokens>
    using instruction_match = typename details::matcher_impl<tokens>;

    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_cmp, tokens::tok_ebx, tokens::tok_comma, tokens::tok_eax , string<>, string<>>>::instruction,
                values_container<
                        inst::to_size<inst::id_t::CMP_REG_REG>,
                        regs::to_size<regs::id_t::EBX>,
                        regs::to_size<regs::id_t::EAX>
                >>::value, "");
    }
}

namespace cai
{
    namespace tests
    {
        static_assert(std::is_same<instruction_match<tuple<tokens::tok_push, tokens::tok_eax, string<>, string<>>>::instruction,
                values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");
    }
}

namespace cai
{




    namespace details
    {
        template <typename>
        struct get_instruction_impl;

        template <typename ...tokens>
        struct get_instruction_impl<tuple<tokens...>>
        {
            using match = instruction_match<tuple<tokens...>>;
            using instruction = typename match::instruction;
            using res_of_tokens = typename match::rest_of_tokens_t;
        };
    }

    template <typename tokens>
    using get_instruction = details::get_instruction_impl<tokens>;

    namespace details
    {
        template <typename tokens, typename instructions_t = values_container<>>
        struct assemble_impl;

        template <typename curr_instructions>
        struct assemble_impl<tuple<>, curr_instructions>
        {
            using instructions_result = curr_instructions;
        };

        template <typename tokens, typename curr_instructions>
        struct assemble_impl
        {
            using instruction = get_instruction<tokens>;

            using next_instructions = values_merge<curr_instructions, typename instruction::instruction>;
            using next_tokens = typename instruction::res_of_tokens;

            using instructions_result = typename assemble_impl<next_tokens, next_instructions>::instructions_result;
        };
    }

    template <typename tokens>
    using assemble = typename details::assemble_impl<tokens>::instructions_result;

    namespace tests
    {

        static_assert(std::is_same<assemble<tuple<tokens::tok_push, tokens::tok_eax>>,
                                   values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>>>::value,"");



        static_assert(std::is_same<assemble<tuple<tokens::tok_push, tokens::tok_eax, tokens::tok_pop, tokens::tok_ebx>>,
                                   values_container<inst::to_size<inst::id_t::PUSH_REG>, regs::to_size<regs::id_t::EAX>,
                                                    inst::to_size<inst::id_t::POP_REG>, regs::to_size<regs::id_t::EBX>>>::value,"");
    }
}

namespace cai
{
    template <typename name, size_t ip>
    struct label_metadata {};




    namespace details
    {
        template <typename, typename, size_t>
        struct label_add_impl;

        template <typename ...current_labels, typename name, size_t ip>
        struct label_add_impl<tuple<current_labels...>, name, ip>
        {
            using type = tuple<current_labels..., label_metadata<name, ip>>;
        };
    }

    template <typename current_labels, typename name, size_t ip>
    using labels_add = typename details::label_add_impl<current_labels, name, ip>::type;




    namespace details
    {
        template <typename, typename>
        struct labels_get_ip_impl;

        template <typename ...current_labels, typename name, size_t ip_v>
        struct labels_get_ip_impl<tuple<label_metadata<name, ip_v>, current_labels...>, name>
        {
            static constexpr auto ip = ip_v;
        };

        template <typename current_label, typename ...current_labels, typename name>
        struct labels_get_ip_impl<tuple<current_label, current_labels...>, name>
        {
            static constexpr auto ip = labels_get_ip_impl<tuple<current_labels...>, name>::ip;
        };
    }

    template <typename labels, typename name>
    constexpr auto labels_get_ip = details::labels_get_ip_impl<labels, name>::ip;
}

namespace cai
{



    namespace details
    {
        template <typename tokens, typename labels, typename final_tokens = tuple<>>
        struct substitute_labels_impl;

        template <typename labels, typename final_tokens>
        struct substitute_labels_impl<tuple<>, labels, final_tokens>
        {
            using tokens = final_tokens;
        };


        template <char ...label_chars, typename ...rest_of_tokens, typename labels, typename ...current_tokens>
        struct substitute_labels_impl<
                tuple<string<'.', label_chars...>, rest_of_tokens...>,
                labels,
                tuple<current_tokens...>>
        {
            static constexpr auto ip = labels_get_ip<labels, string<'.', label_chars...>>;
            using str_ip = string_from_int<ip>;

            using substitued = substitute_labels_impl<
                    tuple<rest_of_tokens...>,
                    labels,
                    tuple<current_tokens..., str_ip>>;

            using tokens = typename substitued::tokens;
        };


        template <typename current_token, typename ...rest_of_tokens, typename labels, typename ...current_tokens>
        struct substitute_labels_impl<
                tuple<current_token, rest_of_tokens...>,
                labels,
                tuple<current_tokens...>>
        {
            using substitued = substitute_labels_impl<
                    tuple<rest_of_tokens...>,
                    labels,
                    tuple<current_tokens..., current_token>>;

            using tokens = typename substitued::tokens;
        };
    }

    template <typename tokens, typename labels>
    using substutite_labels = typename details::substitute_labels_impl<tokens, labels>::tokens;

    namespace tests
    {
        static_assert(
                std::is_same<
                        substutite_labels<tuple<decltype("abc"_s), decltype("def"_s)>, tuple<>>,
                        tuple<decltype("abc"_s), decltype("def"_s)>
                >::value
                ,"");

        static_assert(
                std::is_same<
                        substutite_labels<tuple<decltype(".label"_s), decltype("abc"_s), decltype("def"_s)>, tuple<label_metadata<decltype(".label"_s), 12>>>,
                        tuple<decltype("12"_s), decltype("abc"_s), decltype("def"_s)>
                >::value
                ,"");
    }
}

namespace cai
{



    namespace details
    {
        template <typename tokens, size_t current_ip = 0, typename final_tokens = tuple<>, typename final_labels = tuple<>>
        struct extract_labels_impl;

        template <size_t ip, typename final_tokens, typename final_labels>
        struct extract_labels_impl<tuple<>, ip, final_tokens, final_labels>
        {
            using tokens = final_tokens;
            using labels = final_labels;
        };


        template <size_t current_ip, char ...label_chars, typename ...rest_of_tokens, typename ...result_tokens, typename ...result_labels>
        struct extract_labels_impl<
                tuple<string<':', label_chars...>, rest_of_tokens...>,
                current_ip,
                tuple<result_tokens...>,
                tuple<result_labels...>>
        {
            using extracted = extract_labels_impl<
                    tuple<rest_of_tokens...>,
                    current_ip,
                    tuple<result_tokens...>,
                    tuple<result_labels..., label_metadata<string<'.', label_chars...>, current_ip>>>;

            using tokens = typename extracted::tokens;
            using labels = typename extracted::labels;
        };


        template <size_t current_ip, typename current_token, typename ...rest_of_tokens, typename ...result_tokens, typename result_labels>
        struct extract_labels_impl<
                tuple<current_token, rest_of_tokens...>,
                current_ip,
                tuple<result_tokens...>,
                result_labels>
        {
            using instruction = instruction_match<tuple<current_token, rest_of_tokens...>>;
            using next_tokens = tuple_merge<tuple<result_tokens...>, typename instruction::instruction_tokens>;
            static constexpr auto nex_ip = current_ip + instruction::eip_change;

            using extracted = extract_labels_impl<
                    typename instruction::rest_of_tokens_t,
                    nex_ip,
                    next_tokens,
                    result_labels>;

            using tokens = typename extracted::tokens;
            using labels = typename extracted::labels;
        };
    }

    template <typename tokens>
    using extract_labels = details::extract_labels_impl<tokens>;

    namespace tests
    {
        static_assert(
                std::is_same<extract_labels<tuple<decltype(":label0"_s), decltype("push"_s), decltype("eax"_s)>>::tokens,
                             tuple<decltype("push"_s), decltype("eax"_s)>
                             >::value
                ,"");

        static_assert(
                std::is_same<extract_labels<tuple<decltype(":label0"_s), decltype("push"_s), decltype("eax"_s), decltype(":label1"_s)>>::labels,
                             tuple<label_metadata<decltype(".label0"_s), 0>,
                                   label_metadata<decltype(".label1"_s), 2>>
                             >::value
                ,"");
    }
}

namespace cai
{
    namespace details
    {
        template <typename, typename, typename>
        struct execute_impl;

        template <typename curr_machine_state, typename all_opcodes, size_t ...rest_of_instructions>
        struct execute_impl<curr_machine_state, all_opcodes, values_container<inst::to_size<inst::id_t::EXIT>, rest_of_instructions...>>
        {
            using regs_state = typename to_machine_state<curr_machine_state>::registers_state_t;
            static constexpr auto ret_val = get_reg<regs_state, regs::id_t::EAX>;
        };

        template <typename curr_machine_state, typename all_opcodes, typename opcodes = all_opcodes>
        struct execute_impl
        {
            using next_machine_state = execute_instruction<curr_machine_state, opcodes>;
            using next_opcodes = get_next_instruction<next_machine_state, all_opcodes>;
            static constexpr auto ret_val = execute_impl<next_machine_state, all_opcodes, next_opcodes>::ret_val;
        };
    }

    namespace details
    {
        template <typename code>
        struct prepare_and_execute
        {
            using tokens = tokenize<code>;
            using extract_labels_result = extract_labels<tokens>;
            using tokens_without_labels = typename extract_labels_result::tokens;
            using labels_metadata = typename extract_labels_result::labels;
            using tokens_after_labels_substitution = substutite_labels<tokens_without_labels, labels_metadata>;
            using opcodes = assemble<tokens_after_labels_substitution>;
            using machine_state = startup_machine_state;

            static constexpr auto ret_val = execute_impl<machine_state, opcodes>::ret_val;
        };
    }


    template <typename code>
    constexpr auto execute_code = details::prepare_and_execute<code>::ret_val;
}

using code = decltype(
    "mov ebp , esp "
    "push eax "
    "push eax "
    "push eax "
    "push eax "
    "mov DWORD PTR [ ebp + 8 ] , 0 "
    "mov DWORD PTR [ ebp + 12 ] , 1 "
    "mov DWORD PTR [ ebp + 16 ] , 1 "
    "mov DWORD PTR [ ebp + 4 ] , 1 "
":loop_label "
    "mov eax , DWORD PTR [ ebp + 4 ] "
    "mov ebx , 15 "
    "cmp eax , ebx "
    "jge .end_label "
    "mov edx , DWORD PTR [ ebp + 8 ] "
    "mov eax , DWORD PTR [ ebp + 12 ] "
    "add eax , edx "
    "mov DWORD PTR [ ebp + 16 ] , eax "
    "mov eax , DWORD PTR [ ebp + 12 ] "
    "mov DWORD PTR [ ebp + 8 ] , eax "
    "mov eax , DWORD PTR [ ebp + 16 ] "
    "mov DWORD PTR [ ebp + 12 ] , eax "
    "mov eax , DWORD PTR [ ebp + 4 ] "
    "mov ebx , 1 "
    "add eax , ebx "
    "mov DWORD PTR [ ebp + 4 ] , eax "
    "jmp .loop_label "
":end_label "
    "mov eax , DWORD PTR [ ebp + 16 ] "
    "exit"_s);

int main()
{
    return cai::execute_code<code>;
}
