#pragma once
#include "worlddata.h"
#include "location.h"
class Board
{
private:
    Board();
    WorldData& _data;
    size_t _index;
    BoardData& GetBoardData();
    const BoardData& GetBoardData() const;
public:
    Board(WorldData& data, size_t index):_data(data),_index(index){}
    void SetLocation(size_t column, size_t row, std::optional<Location> location);
    std::optional<Location> GetLocation(size_t column, size_t row) const;
    size_t GetIndex() const {return _index;}
    size_t GetColumns() const;
    size_t GetRows() const;
};