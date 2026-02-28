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
    std::map<StatisticType, int> _statisticMinimums;
    std::map<StatisticType, int> _statisticMaximums;
    CharacterData();
public:
    CharacterData(CharacterType characterType, size_t locationIndex)
        : _characterType(characterType)
        , _locationIndex(locationIndex)
        , _statistics()
        , _statisticMinimums()
        , _statisticMaximums()
        {

        }
    size_t GetLocationIndex() const {return _locationIndex;}
    void SetLocationIndex(size_t locationIndex){_locationIndex = locationIndex;}
    CharacterType GetCharacterType() const {return _characterType;}
    void SetStatistic(StatisticType statisticType, std::optional<int> statisticValue);
    void SetStatisticMinimum(StatisticType statisticType, std::optional<int> statisticMinimum);
    void SetStatisticMaximum(StatisticType statisticType, std::optional<int> statisticMaximum);
    std::optional<int> GetStatistic(StatisticType statisticType) const;
    int GetStatisticMinimum(StatisticType statisticType) const;
    int GetStatisticMaximum(StatisticType statisticType) const;
};