#pragma once
#include <vector>
#include <optional>
class BoardData
{
private:
    std::vector<std::optional<size_t>> _locationIndices;
    size_t _columns;
    size_t _rows;
    BoardData();
public:
    BoardData(size_t columns, size_t rows);
    void SetLocationIndex(size_t column, size_t row, std::optional<size_t> locationIndex);
    std::optional<size_t> GetLocationIndex(size_t column, size_t row) const;
    size_t GetColumns() const {return _columns;}
    size_t GetRows() const {return _rows;}
};