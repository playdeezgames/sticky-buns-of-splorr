#include "character.h"
#include "location.h"
#include "board.h"
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
void Character::SetLocation(Location location)
{
    GetCharacterData().SetLocationIndex(location.GetIndex());
}
void Character::Recycle()
{
    _data.RecycleCharacter(GetIndex());
}
Board Character::GetBoard() const
{
    return GetLocation().GetBoard();
}
void Character::SetStatistic(StatisticType statisticType, std::optional<int> statisticValue)
{
    GetCharacterData().SetStatistic(statisticType, statisticValue);
}
std::optional<int> Character::GetStatistic(StatisticType statisticType) const
{
    return GetCharacterData().GetStatistic(statisticType);
}


