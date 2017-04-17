#pragma once

#include "string.hpp"

namespace ctai
{
    namespace tokens
    {
        using tok_empty = decltype(""_s);

        using tok_exit = decltype("exit"_s);

        using tok_mov = decltype("mov"_s);
        using tok_add = decltype("add"_s);
        using tok_push = decltype("push"_s);
        using tok_pusha = decltype("pusha"_s);
        using tok_pop = decltype("pop"_s);
        using tok_popa = decltype("popa"_s);
        using tok_sub = decltype("sub"_s);
        using tok_mul = decltype("mul"_s);
        using tok_cmp = decltype("cmp"_s);
        using tok_jmp= decltype("jmp"_s);
        using tok_jge = decltype("jge"_s);
        using tok_jg = decltype("jg"_s);
        using tok_jl = decltype("jl"_s);
        using tok_jne = decltype("jne"_s);
        using tok_call = decltype("call"_s);
        using tok_ret = decltype("ret"_s);

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

        //using tok_eax = reg_token<decltype("eax"_s)>;
        //using tok_ebx = reg_token<decltype("ebx"_s)>;
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

    //
    //is_label_token
    //
    namespace details
    {
        template <typename token>
        struct is_label_token_impl
        {
            static constexpr auto result = false;
        };

        template <char ...chars>
        struct is_label_token_impl<ctai::string<'.', chars...>>
        {
            static constexpr auto result = false;
        };
    }

    template <typename token>
    constexpr auto is_label_token = details::is_label_token_impl<token>::result;

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