#pragma once

#include "tuple.hpp"
#include "tokenize/tokens.hpp"
#include "instructions/ids_vaules.hpp"
#include "instructions/matchers/operand_decoder.hpp"
#include "instructions/matchers/plus_minus_decoder.hpp"
#include "instructions/matchers/mem_size_decoder.hpp"
#include "containers/values_container.hpp"

namespace ctai
{
    namespace details
    {
        //mov reg , reg/val
        template <typename reg_token, typename operand, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_mov,
                reg_token,
                tokens::tok_comma,
                operand,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_reg_token<operand> ? inst::id_t::MOV_REG_REG : inst::id_t::MOV_REG_VAL;

            using instruction = values_container_n::values_container<
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

        //mov reg , BYTE/WORD/DWORD PTR [ reg ]
        template <typename reg_token, typename mem_size_token, typename mem_ptr_reg, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_mov,
                reg_token,
                tokens::tok_comma,
                mem_size_token,
                tokens::tok_ptr,
                tokens::tok_square_bracket_open,
                mem_ptr_reg,
                tokens::tok_square_bracket_close,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = inst::id_t::MOV_REG_MEM__mem_eq_reg;

            using instruction = values_container_n::values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_opcode<reg_token>,
                    token_to_reg_opcode<mem_ptr_reg>,
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
                    tokens::tok_square_bracket_close>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };

        //mov reg , BYTE/WORD/DWORD PTR [ reg +/- val ]
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

            using instruction = values_container_n::values_container<
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

        //mov BYTE/WORD/DWORD PTR [ reg ], reg/val
        template <typename reg_or_val, typename mem_size_token, typename mem_ptr_reg, typename ...rest_of_tokens>
        struct matcher_impl<tuple<
                tokens::tok_mov,
                mem_size_token,
                tokens::tok_ptr,
                tokens::tok_square_bracket_open,
                mem_ptr_reg,
                tokens::tok_square_bracket_close,
                tokens::tok_comma,
                reg_or_val,
                rest_of_tokens...>>
        {
            static constexpr auto instruction_type = is_reg_token<reg_or_val> ? inst::id_t::MOV_MEM_REG__mem_eq_reg
                                                                              : inst::id_t::MOV_MEM_VAL__mem_eq_reg;

            using instruction = values_container_n::values_container<
                    inst::to_size<instruction_type>,
                    token_to_reg_opcode<mem_ptr_reg>,
                    memory::to_size<mem_size_decoder<mem_size_token>>,
                    operand_decoder<reg_or_val>>;

            static constexpr auto eip_change = get_eip_change<instruction_type>;
            using instruction_tokens = tuple<
                    tokens::tok_mov,
                    mem_size_token,
                    tokens::tok_ptr,
                    tokens::tok_square_bracket_open,
                    mem_ptr_reg,
                    tokens::tok_square_bracket_close,
                    tokens::tok_comma,
                    reg_or_val>;

            using rest_of_tokens_t = tuple<rest_of_tokens...>;
        };

        //mov BYTE/WORD/DWORD PTR [ reg +/- val ], reg/val
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

            using instruction = values_container_n::values_container<
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
                                   values_container_n::values_container<inst::to_size<inst::id_t::POP_REG>, regs::to_size<regs::id_t::EAX>>>::value, "");

        static_assert(std::is_same<instruction_match<tuple<decltype("mov"_s),
                                                           decltype("eax"_s),
                                                           decltype(","_s),
                                                           decltype("2"_s)>>::instruction,
                                   values_container_n::values_container<inst::to_size<inst::id_t::MOV_REG_VAL>,
                                                                        regs::to_size<regs::id_t::EAX>,
                                                                        static_cast<size_t>(2)>>::value, "");

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
                                   values_container_n::values_container<
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
                                   values_container_n::values_container<
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
                                   values_container_n::values_container<
                        inst::to_size<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const>,
                        regs::to_size<regs::id_t::EDX>,
                        2,
                        memory::to_size<memory::id_t::s_8>,
                        regs::to_size<regs::id_t::EAX>
                >>::value, "");
    }
}