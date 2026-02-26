#include "worlddata.h"
void WorldData::Clear()
{
    _boards.clear();    
}
size_t WorldData::CreateBoard()
{
    size_t result = _boards.size();
    _boards.push_back(BoardData());
    return result;
}
BoardData& WorldData::GetBoard(size_t index)
{
    return _boards[index];
}
const BoardData& WorldData::GetBoard(size_t index) const
{
    return _boards[index];
}
