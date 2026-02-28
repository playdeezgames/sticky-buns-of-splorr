#pragma once
#include "worlddata.h"
#include "characterdata.h"
#include "charactertype.h"
class Location;
class Board;
class Character
{
private:
    WorldData& _data;
    size_t _index;
    Character();
    CharacterData& GetCharacterData();
    const CharacterData& GetCharacterData() const;
public:
    Character(WorldData& data, size_t index): _data(data), _index(index) { }
    size_t GetIndex() const {return _index;}
    Location GetLocation() const;
    Board GetBoard() const;
    void SetLocation(Location location);
    CharacterType GetCharacterType() const;
    void Recycle();
};