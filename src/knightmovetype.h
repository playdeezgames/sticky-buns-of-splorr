#pragma once
#include <array>
enum class KnightMoveType
{
    UUR,
    RRU,
    RRD,
    DDR,
    DDL,
    LLD,
    LLU,
    UUL
};
constexpr std::array<KnightMoveType, 8> AllKnightMoveTypes = 
{
    KnightMoveType::UUR,
    KnightMoveType::RRU,
    KnightMoveType::RRD,
    KnightMoveType::DDR,
    KnightMoveType::DDL,
    KnightMoveType::LLD,
    KnightMoveType::LLU,
    KnightMoveType::UUL
};