#include "locationdata.h"

void LocationData::SetNeighborIndex(KnightMoveType knightMoveType, size_t neighborIndex)
{
    _neighbors[knightMoveType] = neighborIndex;
}
void LocationData::RemoveNeighborIndex(KnightMoveType knightMoveType)
{
    _neighbors.erase(knightMoveType);
}
std::optional<size_t> LocationData::GetNeighborIndex(KnightMoveType knightMoveType) const
{
    auto result = _neighbors.find(knightMoveType);
    if(result==_neighbors.end())
    {
        return std::nullopt;
    }
    return result->second;
}
