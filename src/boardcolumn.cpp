#include "boardcolumn.h"
BoardColumn::BoardColumn(int rows)
    :_cells()
{
    while(_cells.size() < rows)
    {
        _cells.push_back(BoardCell(CellType::FLOOR));
    }
}
int BoardColumn::GetRows() const
{
    return _cells.size();
}
BoardCell& BoardColumn::GetCell(int row)
{
    return _cells[row];
}
const BoardCell& BoardColumn::GetCell(int row) const
{
    return _cells[row];
}
