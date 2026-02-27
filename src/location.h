#pragma once
#include "worlddata.h"
#include "character.h"
class Board;
class Location
{
private:
    WorldData& _data;
    size_t _index;
    Location();
    LocationData& GetLocationData();
    const LocationData& GetLocationData() const;
public:
    Location(WorldData& data, size_t index):_data(data), _index(index){}
    size_t GetIndex() const{return _index;}
    std::optional<Character> GetCharacter() const;
    void SetCharacter(std::optional<Character> character);
    Board GetBoard() const;
    bool GetLight() const;
};