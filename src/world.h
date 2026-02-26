#pragma once
#include "worlddata.h"
class World
{
private:
    WorldData& _data;
    World();
public:
    World(WorldData& data):_data(data){}
};