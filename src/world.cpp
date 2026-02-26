#include "world.h"
void World::Initialize()
{
    _data.Clear();
    auto board = CreateBoard();
}
Board World::CreateBoard()
{
    return Board(_data, _data.CreateBoard());
}

