#pragma once

#include "ctai/string.hpp"

namespace ctai
{
    namespace tokens
    {
        using tok_empty = decltype(""_s);

        using tok_exit = decltype("exit"_s);

        using tok_mov = decltype("mov"_s);
        using tok_add = decltype("add"_s);
        using tok_sub = decltype("sub"_s);
        using tok_push = decltype("push"_s);
        using tok_pusha = decltype("pusha"_s);
        using tok_pop = decltype("pop"_s);
        using tok_popa = decltype("popa"_s);
        using tok_sub = decltype("sub"_s);
        using tok_mul = decltype("mul"_s);
        using tok_div = decltype("div"_s);
        using tok_cmp = decltype("cmp"_s);
        using tok_inc= decltype("inc"_s);
        using tok_dec= decltype("dec"_s);
        using tok_jmp= decltype("jmp"_s);
        using tok_jge = decltype("jge"_s);
        using tok_jg = decltype("jg"_s);
        using tok_je = decltype("je"_s);
        using tok_jl = decltype("jl"_s);
        using tok_jne = decltype("jne"_s);
        using tok_call = decltype("call"_s);
        using tok_ret = decltype("ret"_s);

        using tok_al = decltype("al"_s);
        using tok_ah = decltype("ah"_s);
        using tok_ax = decltype("ax"_s);
        using tok_eax = decltype("eax"_s);
        using tok_bl = decltype("bl"_s);
        using tok_bh = decltype("bh"_s);
        using tok_bx = decltype("bx"_s);
        using tok_ebx = decltype("ebx"_s);
        using tok_cl = decltype("cl"_s);
        using tok_ch = decltype("ch"_s);
        using tok_cx = decltype("cx"_s);
        using tok_ecx = decltype("ecx"_s);
        using tok_dl = decltype("dl"_s);
        using tok_dh = decltype("dh"_s);
        using tok_dx = decltype("dx"_s);
        using tok_edx = decltype("edx"_s);
        using tok_sp = decltype("sp"_s);
        using tok_esp = decltype("esp"_s);
        using tok_bp = decltype("bp"_s);
        using tok_ebp = decltype("ebp"_s);
        using tok_di = decltype("di"_s);
        using tok_edi = decltype("edi"_s);
        using tok_si = decltype("si"_s);
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
                                               tokens::tok_al,
                                               tokens::tok_ah,
                                               tokens::tok_ax,
                                               tokens::tok_eax,
                                               tokens::tok_bl,
                                               tokens::tok_bh,
                                               tokens::tok_bx,
                                               tokens::tok_ebx,
                                               tokens::tok_cl,
                                               tokens::tok_ch,
                                               tokens::tok_cx,
                                               tokens::tok_ecx,
                                               tokens::tok_dl,
                                               tokens::tok_dh,
                                               tokens::tok_dx,
                                               tokens::tok_edx,
                                               tokens::tok_sp,
                                               tokens::tok_esp,
                                               tokens::tok_bp,
                                               tokens::tok_ebp,
                                               tokens::tok_di,
                                               tokens::tok_edi,
                                               tokens::tok_si,
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

        //TODO add rest of registers
        template <> struct token_to_reg_opcode_impl<tokens::tok_al> { static constexpr auto value = regs::to_size<regs::id_t::AL>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ah> { static constexpr auto value = regs::to_size<regs::id_t::AH>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ax> { static constexpr auto value = regs::to_size<regs::id_t::AX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_eax> { static constexpr auto value = regs::to_size<regs::id_t::EAX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_bl> { static constexpr auto value = regs::to_size<regs::id_t::BL>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_bh> { static constexpr auto value = regs::to_size<regs::id_t::BH>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_bx> { static constexpr auto value = regs::to_size<regs::id_t::BX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ebx> { static constexpr auto value = regs::to_size<regs::id_t::EBX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_cl> { static constexpr auto value = regs::to_size<regs::id_t::CL>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ch> { static constexpr auto value = regs::to_size<regs::id_t::CH>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_cx> { static constexpr auto value = regs::to_size<regs::id_t::CX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ecx> { static constexpr auto value = regs::to_size<regs::id_t::ECX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_dl> { static constexpr auto value = regs::to_size<regs::id_t::DL>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_dh> { static constexpr auto value = regs::to_size<regs::id_t::DH>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_dx> { static constexpr auto value = regs::to_size<regs::id_t::DX>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_edx> { static constexpr auto value = regs::to_size<regs::id_t::EDX>; };

        template <> struct token_to_reg_opcode_impl<tokens::tok_sp> { static constexpr auto value = regs::to_size<regs::id_t::SP>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_esp> { static constexpr auto value = regs::to_size<regs::id_t::ESP>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_bp> { static constexpr auto value = regs::to_size<regs::id_t::BP>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_ebp> { static constexpr auto value = regs::to_size<regs::id_t::EBP>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_di> { static constexpr auto value = regs::to_size<regs::id_t::DI>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_edi> { static constexpr auto value = regs::to_size<regs::id_t::EDI>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_si> { static constexpr auto value = regs::to_size<regs::id_t::SI>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_esi> { static constexpr auto value = regs::to_size<regs::id_t::ESI>; };
        template <> struct token_to_reg_opcode_impl<tokens::tok_eip> { static constexpr auto value = regs::to_size<regs::id_t::EIP>; };
    }

    template <typename token>
    constexpr auto token_to_reg_opcode = details::token_to_reg_opcode_impl<token>::value;
}