#pragma once

#include "string.hpp"

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

        using tok_eax = decltype("eax"_s);
        using tok_ebx = decltype("ebx"_s);
        using tok_ecx = decltype("ecx"_s);
        using tok_edx = decltype("edx"_s);

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

        //using tok_eax = reg_token<decltype("eax"_s)>;
        //using tok_ebx = reg_token<decltype("ebx"_s)>;
    }

    template <typename type>
    constexpr auto is_reg_token = is_in_t<type, tokens::tok_eax , tokens::tok_ebx>;

    namespace details
    {
        template <typename>
        struct token_to_reg_val_impl;

        template <> struct token_to_reg_val_impl<tokens::tok_eax> { static constexpr auto value = regs::to_size<regs::id_t::EAX>; };
        template <> struct token_to_reg_val_impl<tokens::tok_ebx> { static constexpr auto value = regs::to_size<regs::id_t::EBX>; };
        template <> struct token_to_reg_val_impl<tokens::tok_ecx> { static constexpr auto value = regs::to_size<regs::id_t::ECX>; };
        template <> struct token_to_reg_val_impl<tokens::tok_edx> { static constexpr auto value = regs::to_size<regs::id_t::EDX>; };
    }

    template <typename token>
    constexpr auto token_to_reg_val = details::token_to_reg_val_impl<token>::value;
}