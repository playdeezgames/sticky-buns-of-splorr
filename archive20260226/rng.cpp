#include "rng.h"
#include <random>
static std::random_device rd; 
static std::mt19937 gen(rd()); 
size_t RNG::FromRange(size_t low, size_t high)
{
    std::uniform_int_distribution<size_t> distrib(low, high);
    return distrib(gen);
}
