#pragma once
#include <cstddef>
#include "charactertype.h"
class CharacterData
{
private:
    CharacterType _characterType;
    size_t _locationIndex;
    CharacterData();
public:
    CharacterData(CharacterType characterType, size_t locationIndex):_characterType(characterType),_locationIndex(locationIndex){}
    size_t GetLocationIndex() const {return _locationIndex;}
    CharacterType GetCharacterType() const {return _characterType;}
};