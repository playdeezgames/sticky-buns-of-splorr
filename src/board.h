#pragma once
#include <vector>
#include "boardcolumn.h"
class Board
{
private:
    std::vector<BoardColumn> boardColumns;
public:
    Board(int column, int rows);
    int GetColumns() const;
    BoardColumn& GetColumn(int column);
    const BoardColumn& GetColumn(int column) const;
};