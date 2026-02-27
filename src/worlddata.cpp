#include "worlddata.h"
void WorldData::Clear()
{
    _boards.clear();    
}
size_t WorldData::CreateBoard(size_t columns, size_t rows)
{
    size_t result = _boards.size();
    _boards.push_back(BoardData(columns, rows));
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
size_t WorldData::CreateLocation(size_t boardIndex, bool light)
{
    size_t result = _locations.size();
    _locations.push_back(LocationData(boardIndex, light));
    return result;
}
LocationData& WorldData::GetLocation(size_t index)
{
    return _locations[index];
}
const LocationData& WorldData::GetLocation(size_t index) const
{
    return _locations[index];
}
size_t WorldData::CreateCharacter(CharacterType characterType,size_t locationIndex)
{
    size_t result = _characters.size();
    _characters.push_back(CharacterData(characterType, locationIndex));
    return result;
}
CharacterData& WorldData::GetCharacter(size_t index)
{
    return _characters[index];
}
const CharacterData& WorldData::GetCharacter(size_t index) const
{
    return _characters[index];
}
void WorldData::SetAvatarIndex(std::optional<size_t> avatarIndex)
{
    _avatarIndex = avatarIndex;
}
std::optional<size_t> WorldData::GetAvatarIndex() const
{
    return _avatarIndex;
}
