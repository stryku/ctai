#pragma once

#include "instructions/ids_vaules.hpp"
#include "static_warning.hpp"

#include <cstdint>

namespace ctai
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

        template <uint16_t val> using set_si = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, set_simple_reg_low<esi_val, val>, edi_val, eip_val>;
        template <uint32_t val> using set_esi = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, set_simple_reg<esi_val, val>, edi_val, eip_val>;

        template <uint16_t val> using set_di = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, set_simple_reg_low<edi_val, val>, eip_val>;
        template <uint32_t val> using set_edi = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, set_simple_reg<edi_val, val>, eip_val>;

        template <uint16_t val> using set_ip = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, set_simple_reg_low<eip_val, val>>;
        template <uint32_t val> using set_eip = registers_state<eax_val, ebx_val, ecx_val, edx_val, esp_val, ebp_val, esi_val, edi_val, set_simple_reg<eip_val, val>>;
    };

    using startup_registers_state = registers_state<
            0xaabbccdd, //eax
            0xbada55,   //ebx
            0xaabbccdd, //ecx
            0xaabbccdd, //edx
            15, //esp
            0xaabbccdd,
            0xaabbccdd,
            0xaabbccdd,
                    0>;//eip


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
        static_assert(get_reg<set_reg<startup_registers_state, regs::id_t::ESI, 0xddccbbaa>, regs::id_t::SI> == 0xbbaa, "");
    }
}
