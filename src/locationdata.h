#pragma once
#include <cstddef>
class LocationData
{
private:
    size_t _boardIndex;
    std::optional<size_t> _characterIndex;
    bool _light;
    LocationData();
public:
    LocationData(size_t boardIndex, bool light):_boardIndex(boardIndex),_characterIndex(std::nullopt),_light(light){}
    size_t GetBoardIndex() const{return _boardIndex;}
    std::optional<size_t> GetCharacterIndex() const { return _characterIndex; }
    void SetCharacterIndex(std::optional<size_t> characterIndex) { _characterIndex = characterIndex;}
    void SetLight(bool light){_light = light;}
    bool GetLight() const {return _light;}
};