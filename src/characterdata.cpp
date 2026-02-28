#include "characterdata.h"
#include <limits>
#include <algorithm>
void CharacterData::SetStatistic(
    StatisticType statisticType, 
    std::optional<int> statisticValue)
{
    if(statisticValue)
    {
        _statistics.insert_or_assign(
            statisticType, 
            std::clamp(
                *statisticValue,
                GetStatisticMinimum(statisticType),
                GetStatisticMaximum(statisticType)));
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
        return std::clamp(
            iter->second, 
            GetStatisticMinimum(statisticType), 
            GetStatisticMaximum(statisticType));
    }
    return std::nullopt;
}
void CharacterData::SetStatisticMinimum(StatisticType statisticType, std::optional<int> statisticMinimum)
{
    if(statisticMinimum)
    {
        _statisticMinimums.insert_or_assign(statisticType, *statisticMinimum);
    }
    else
    {
        _statisticMinimums.erase(statisticType);
    }
}
void CharacterData::SetStatisticMaximum(StatisticType statisticType, std::optional<int> statisticMaximum)
{
    if(statisticMaximum)
    {
        _statisticMaximums.insert_or_assign(statisticType, *statisticMaximum);
    }
    else
    {
        _statisticMaximums.erase(statisticType);
    }
}
int CharacterData::GetStatisticMinimum(StatisticType statisticType) const
{
    auto iter = _statisticMinimums.find(statisticType);
    if(iter != _statisticMinimums.end())
    {
        return iter->second;
    }
    return std::numeric_limits<int>().min();
}
int CharacterData::GetStatisticMaximum(StatisticType statisticType) const
{
    auto iter = _statisticMaximums.find(statisticType);
    if(iter != _statisticMaximums.end())
    {
        return iter->second;
    }
    return std::numeric_limits<int>().max();
}
