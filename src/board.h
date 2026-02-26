#pragma once
#include "worlddata.h"
class Board
{
private:
    Board();
    WorldData& _data;
    size_t _index;
public:
    Board(WorldData& data, size_t index):_data(data),_index(index){}
};