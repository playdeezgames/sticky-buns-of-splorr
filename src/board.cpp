#include "board.h"
Board::Board(int columns, int rows)
    :boardColumns()
{
    while(boardColumns.size() < columns)
    {
        boardColumns.push_back(BoardColumn(rows));
    }
}
int Board::GetColumns() const
{
    return boardColumns.size();
}
BoardColumn& Board::GetColumn(int column)
{
    return boardColumns[column];
}
const BoardColumn& Board::GetColumn(int column) const
{
    return boardColumns[column];
}
