#pragma once
#include <cstddef>
class LocationData
{
private:
    size_t _boardIndex;
    std::optional<size_t> _characterIndex;
    LocationData();
public:
    LocationData(size_t boardIndex):_boardIndex(boardIndex),_characterIndex(std::nullopt){}
    size_t GetBoardIndex() const{return _boardIndex;}
    std::optional<size_t> GetCharacterIndex() const { return _characterIndex; }
    void SetCharacterIndex(std::optional<size_t> characterIndex) { _characterIndex = characterIndex;}
};