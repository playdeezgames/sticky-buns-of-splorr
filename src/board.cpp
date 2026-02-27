#include "board.h"
BoardData& Board::GetBoardData()
{
    return _data.GetBoard(_index);
}
const BoardData& Board::GetBoardData() const
{
    return _data.GetBoard(_index);
}
void Board::SetLocation(size_t column, size_t row, std::optional<Location> location)
{
    if(location.has_value())
    {
        GetBoardData().SetLocationIndex(column, row, location->GetIndex());
    }
    else
    {
        GetBoardData().SetLocationIndex(column, row, std::nullopt);
    }
}
std::optional<Location> Board::GetLocation(size_t column, size_t row) const
{
    auto locationIndex = GetBoardData().GetLocationIndex(column, row);
    if(locationIndex.has_value())
    {
        return Location(_data, *locationIndex);
    }
    return std::nullopt;
}
size_t Board::GetColumns() const
{
    return GetBoardData().GetColumns();
}
size_t Board::GetRows() const
{
    return GetBoardData().GetRows();
}
