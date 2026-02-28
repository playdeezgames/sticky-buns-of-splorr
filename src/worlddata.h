#pragma once
#include <vector>
#include <set>
#include "boarddata.h"
#include "locationdata.h"
#include "characterdata.h"
class WorldData
{
private:
    std::vector<BoardData> _boards;
    std::vector<LocationData> _locations;
    std::vector<CharacterData> _characters;
    std::set<size_t> _recycledCharacters;
    std::optional<size_t> _avatarIndex;
public:
    void Clear();
    size_t CreateBoard(size_t columns, size_t rows);
    BoardData& GetBoard(size_t index);
    const BoardData& GetBoard(size_t index) const;
    size_t CreateLocation(size_t boardIndex, bool light);
    LocationData& GetLocation(size_t index);
    const LocationData& GetLocation(size_t index) const;
    size_t CreateCharacter(CharacterType characterType,size_t locationIndex);
    CharacterData& GetCharacter(size_t index);
    const CharacterData& GetCharacter(size_t index) const;
    void RecycleCharacter(size_t index);
    void SetAvatarIndex(std::optional<size_t> avatarIndex);
    std::optional<size_t> GetAvatarIndex() const;
};