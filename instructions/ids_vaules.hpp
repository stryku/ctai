#pragma once

#include "traits.hpp"

#include <type_traits>

#include <cstddef>
#include <cstdint>

namespace ctai
{
    namespace inst
    {
        enum class id_t
        {
            SYS_EXIT_THREAD,
            SYS_CREATE_THREAD,

            EXIT,
            PUSH_REG,
            PUSHA,
            POP_REG,
            POPA,

            JMP,
            JGE,

            ADD_REG_REG,
            ADD_REG_VAL = 10,

            SUB_REG_REG,
            CMP_REG_REG,

            MOV_REG_REG,
            MOV_REG_VAL,


            MOV_MEM_VAL__mem_eq_reg_minus_const,
            MOV_MEM_VAL__mem_eq_reg_plus_const,

            MOV_MEM_REG__mem_eq_reg_minus_const,
            MOV_MEM_REG__mem_eq_reg_plus_const,

            MOV_REG_MEM__mem_eq_reg_minus_const,
            MOV_REG_MEM__mem_eq_reg_plus_const = 20,

            MUL_REG_REG,

            CALL_VAL,
            RET,
            SYS_WRITE

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
            using type = std::conditional_t<is_value_in_v<to_size<reg>, to_size<id_t::AL>, to_size<id_t::AH>, to_size<id_t::BL>, to_size<id_t::BH>, to_size<id_t::CL>, to_size<id_t::CH>>, // check if regex is 1 byte long
                    uint8_t,
                    std::conditional_t<is_value_in_v<to_size<reg>, to_size<id_t::AX>, to_size<id_t::BX>, to_size<id_t::CX>, to_size<id_t::DX>, to_size<id_t::SI>, to_size<id_t::DI>, to_size<id_t::BP>, to_size<id_t::SP>, to_size<id_t::IP>>, // check if register is 2 bytes long
                            uint16_t, uint32_t >>;

        };

        template <id_t reg>
        using reg_size_type = typename reg_size_type_impl<reg>::type;

        namespace tests
        {
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::AL>, uint8_t>::value, "");
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::AH>, uint8_t>::value, "");
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::CL>, uint8_t>::value, "");
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::CH>, uint8_t>::value, "");

            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::AX>, uint16_t>::value, "");
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::BX>, uint16_t>::value, "");
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::SP>, uint16_t>::value, "");
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::IP>, uint16_t>::value, "");

            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::EAX>, uint32_t>::value, "");
            static_assert(std::is_same<reg_size_type<::ctai::regs::id_t::EBX>, uint32_t>::value, "");
        }
    }
}
