#include "location.h"
#include "board.h"
LocationData& Location::GetLocationData()
{
    return _data.GetLocation(GetIndex());
}
const LocationData& Location::GetLocationData() const
{
    return _data.GetLocation(GetIndex());
}
std::optional<Character> Location::GetCharacter() const
{
    auto characterIndex = GetLocationData().GetCharacterIndex();
    if(characterIndex.has_value())
    {
        return Character(_data, *characterIndex);
    }
    return std::nullopt;
}
void Location::SetCharacter(std::optional<Character> character)
{
    if(character.has_value())
    {
        GetLocationData().SetCharacterIndex(character->GetIndex());
    }
    else
    {
        GetLocationData().SetCharacterIndex(std::nullopt);
    }
}
Board Location::GetBoard() const
{
    return Board(_data, GetLocationData().GetBoardIndex());
}
bool Location::GetLight() const
{
    return GetLocationData().GetLight();
}
