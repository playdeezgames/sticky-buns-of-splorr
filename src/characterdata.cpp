#include "characterdata.h"
void CharacterData::SetStatistic(
    StatisticType statisticType, 
    std::optional<int> statisticValue)
{
    if(statisticValue.has_value())
    {
        _statistics[statisticType] = *statisticValue;
    }
    else
    {
        _statistics.erase(statisticType);
    }
}
std::optional<int> CharacterData::GetStatistic(StatisticType statisticType) const
{
    auto iter = _statistics.find(statisticType);
    if(iter != _statistics.end())
    {
        return iter->second;
    }
    return std::nullopt;
}
