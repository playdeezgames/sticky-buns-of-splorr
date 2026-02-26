#pragma once
#include <vector>
#include <map>
#include <numeric>
class RNG
{
private:
public:
    static size_t FromRange(size_t low, size_t high);
    template <typename T>
    static T FromGenerator(const std::map<T, size_t>& generator)
    {
        size_t total = std::accumulate(
            generator.begin(), 
            generator.end(), 
            0UL, 
            [](size_t acc, const auto& v) 
            {
                return acc + v.second;
            });
        size_t generated = RNG::FromRange(0UL, total-1);
        for(const auto& entry : generator)
        {
            if(entry.second > generated)
            {
                return entry.first;
            }
            else
            {
                generated -= entry.second;
            }
        }
        throw "dint work!";
    }
};