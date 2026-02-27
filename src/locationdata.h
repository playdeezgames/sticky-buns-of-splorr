#pragma once
#include <cstddef>
#include <map>
#include "knightmovetype.h"
#include <optional>
class LocationData
{
private:
    size_t _boardIndex;
    std::optional<size_t> _characterIndex;
    bool _light;
    std::map<KnightMoveType, size_t> _neighbors;
    LocationData();
public:
    LocationData(size_t boardIndex, bool light):_boardIndex(boardIndex),_characterIndex(std::nullopt),_light(light),_neighbors(){}
    size_t GetBoardIndex() const{return _boardIndex;}
    std::optional<size_t> GetCharacterIndex() const { return _characterIndex; }
    void SetCharacterIndex(std::optional<size_t> characterIndex) { _characterIndex = characterIndex;}
    void SetLight(bool light){_light = light;}
    bool GetLight() const {return _light;}
    void SetNeighborIndex(KnightMoveType knightMoveType, size_t neighborIndex);
    void RemoveNeighborIndex(KnightMoveType knightMoveType);
    std::optional<size_t> GetNeighborIndex(KnightMoveType knightMoveType) const;
};