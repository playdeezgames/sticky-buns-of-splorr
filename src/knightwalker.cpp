#include "knightwalker.h"
#include <map>
static const std::map<KnightMoveType, std::tuple<int, int>> deltas = 
{
    {KnightMoveType::DDL, {-1,2}},
    {KnightMoveType::DDR, {1,2}},
    {KnightMoveType::LLD, {-2,1}},
    {KnightMoveType::LLU, {-2,-1}},
    {KnightMoveType::RRD, {2,1}},
    {KnightMoveType::RRU, {2,-1}},
    {KnightMoveType::UUL, {-1,-2}},
    {KnightMoveType::UUR, {1,-2}}
};
std::tuple<int, int> KnightWalker::Walk(int x, int y, KnightMoveType knightMoveType)
{
    auto delta = deltas.find(knightMoveType)->second;
    return {x+std::get<0>(delta), y+std::get<1>(delta)};
}
