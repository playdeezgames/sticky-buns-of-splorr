#pragma once
#include <cstddef>
#include "charactertype.h"
#include "statistictype.h"
#include <optional>
#include <map>
class CharacterData
{
private:
    CharacterType _characterType;
    size_t _locationIndex;
    std::map<StatisticType, int> _statistics;
    CharacterData();
public:
    CharacterData(CharacterType characterType, size_t locationIndex)
        : _characterType(characterType)
        , _locationIndex(locationIndex)
        , _statistics()
        {

        }
    size_t GetLocationIndex() const {return _locationIndex;}
    void SetLocationIndex(size_t locationIndex){_locationIndex = locationIndex;}
    CharacterType GetCharacterType() const {return _characterType;}
    void SetStatistic(StatisticType statisticType, std::optional<int> statisticValue);
    std::optional<int> GetStatistic(StatisticType statisticType) const;
};