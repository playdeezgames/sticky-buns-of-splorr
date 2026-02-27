#include "boarddata.h"
BoardData::BoardData(size_t columns, size_t rows)
    : _locationIndices(columns * rows)
    , _columns(columns)
    , _rows(rows)
{
    while(_locationIndices.size() < columns * rows)
    {
        _locationIndices.push_back(std::nullopt);
    }
}
void BoardData::SetLocationIndex(size_t column, size_t row, std::optional<size_t> locationIndex)
{
    _locationIndices[column + row * _columns] = locationIndex;
}
std::optional<size_t> BoardData::GetLocationIndex(size_t column, size_t row) const
{
    return _locationIndices[column + row * _columns];
}
