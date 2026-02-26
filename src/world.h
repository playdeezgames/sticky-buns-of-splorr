#pragma once
#include "worlddata.h"
#include "board.h"
class World
{
private:
    WorldData& _data;
    World();
public:
    static constexpr int BOARD_COLUMNS = 8;
    static constexpr int BOARD_ROWS = 8;
    World(WorldData& data):_data(data){}
    void Initialize();
    Board CreateBoard();
};