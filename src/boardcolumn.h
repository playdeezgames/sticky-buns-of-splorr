#pragma once
#include "boardcell.h"
#include <vector>
class BoardColumn
{
private:
    std::vector<BoardCell> _cells;
public:
    BoardColumn(int rows);
    int GetRows() const;
    BoardCell& GetCell(int row);
    const BoardCell& GetCell(int row) const;
};