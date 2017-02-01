#pragma once


#include <cstddef>

namespace cai
{
    namespace inst
    {
        enum class id_t
        {
            PUSH_REG
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
    }
}
