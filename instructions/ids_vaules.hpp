#pragma once

#include "traits.hpp"

#include <type_traits>

#include <cstddef>

namespace cai
{
    namespace inst
    {
        enum class id_t
        {
            PUSH_REG,
            POP_REG,
            ADD_REG_REG,
            SUB_REG_REG,
            CMP_REG_REG,
            MOV_REG_REG,
            MOV_MEM_VAL_mem_eq_reg_minus_const,
            MUL_REG_REG
        };

        template <size_t id_v>
        static constexpr auto to_id = static_cast<id_t>(id_v);

        template <id_t id_v>
        static constexpr auto to_size = static_cast<size_t>(id_v);
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
            using type = std::conditional_t<is_in<to_size<reg>, to_size<id_t::AL>, to_size<id_t::AH>, to_size<id_t::BL>, to_size<id_t::BH>, to_size<id_t::CL>, to_size<id_t::CH>>, // check if regex is 1 byte long
                    uint8_t,
                    std::conditional_t<is_in<to_size<reg>, to_size<id_t::AX>, to_size<id_t::BX>, to_size<id_t::CX>, to_size<id_t::DX>, to_size<id_t::SI>, to_size<id_t::DI>, to_size<id_t::BP>, to_size<id_t::SP>, to_size<id_t::IP>>, // check if register is 2 bytes long
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
