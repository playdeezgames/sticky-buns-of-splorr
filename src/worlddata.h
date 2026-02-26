#pragma once
#include <vector>
#include "boarddata.h"
class WorldData
{
private:
    std::vector<BoardData> _boards;
public:
    void Clear();
    size_t CreateBoard();
    BoardData& GetBoard(size_t index);
    const BoardData& GetBoard(size_t index) const;
};