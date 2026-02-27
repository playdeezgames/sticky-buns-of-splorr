#include "character.h"
#include "location.h"
CharacterData& Character::GetCharacterData()
{
    return _data.GetCharacter(GetIndex());
}
const CharacterData& Character::GetCharacterData() const
{
    return _data.GetCharacter(GetIndex());
}
Location Character::GetLocation() const
{
    return Location(_data, GetCharacterData().GetLocationIndex());
}
CharacterType Character::GetCharacterType() const
{
    return GetCharacterData().GetCharacterType();
}

