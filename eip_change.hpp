#pragma once

#include "instructions/ids_vaules.hpp"
#include "register.hpp"

#include <cstddef>
#include <tuple.hpp>

namespace ctai
{
    namespace details
    {
        template <inst::id_t instruction_opcode>
        constexpr size_t eip_change{0};

        template <> constexpr size_t eip_change<inst::id_t::ADD_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::ADD_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::SUB_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::SUB_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::INC> = 2;
        template <> constexpr size_t eip_change<inst::id_t::CMP_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::CMP_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::EXIT> = 1;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_REG__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_REG__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_MEM__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_MEM__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_VAL__mem_eq_reg_minus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_VAL__mem_eq_reg_plus_const> = 5;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_MEM__mem_eq_reg> = 4;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_REG__mem_eq_reg> = 4;
        template <> constexpr size_t eip_change<inst::id_t::MOV_MEM_VAL__mem_eq_reg> = 4;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_VAL> = 3;
        template <> constexpr size_t eip_change<inst::id_t::MOV_REG_REG> = 3;
        template <> constexpr size_t eip_change<inst::id_t::PUSH_REG> = 2;
        template <> constexpr size_t eip_change<inst::id_t::PUSHA> = 1;
        template <> constexpr size_t eip_change<inst::id_t::POP_REG> = 2;
        template <> constexpr size_t eip_change<inst::id_t::POPA> = 1;
        template <> constexpr size_t eip_change<inst::id_t::JMP> = 2;
        template <> constexpr size_t eip_change<inst::id_t::JNE> = 2;
        template <> constexpr size_t eip_change<inst::id_t::JGE> = 2;
        template <> constexpr size_t eip_change<inst::id_t::JG> = 2;
        template <> constexpr size_t eip_change<inst::id_t::JL> = 2;
        template <> constexpr size_t eip_change<inst::id_t::JE> = 2;
        template <> constexpr size_t eip_change<inst::id_t::MUL_REG> = 2;

        template <> constexpr size_t eip_change<inst::id_t::CALL_VAL> = 2;
        template <> constexpr size_t eip_change<inst::id_t::RET> = 1;

        template <> constexpr size_t eip_change<inst::id_t::SYS_EXIT_THREAD> = 1;
        template <> constexpr size_t eip_change<inst::id_t::SYS_CREATE_THREAD> = 1;
        template <> constexpr size_t eip_change<inst::id_t::SYS_WRITE> = 1;
        template <> constexpr size_t eip_change<inst::id_t::SYS_READ> = 1;
        template <> constexpr size_t eip_change<inst::id_t::SYS_IS_THREAD_RUNNING> = 1;
        template <> constexpr size_t eip_change<inst::id_t::SYS_MALLOC> = 1;
        template <> constexpr size_t eip_change<inst::id_t::SYS_FREE> = 1;
    }

    template <inst::id_t instruction>
    constexpr auto get_eip_change = details::eip_change<instruction>;
}
