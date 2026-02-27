#pragma once
#include "knightmovetype.h"
#include <tuple>
class KnightWalker
{
private:
public:
    static std::tuple<int, int> Walk(int x, int y, KnightMoveType knightMoveType);
};