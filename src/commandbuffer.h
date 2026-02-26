#pragma once
#include "commandtype.h"
#include <queue>
#include <optional>
class CommandBuffer
{
private:
    std::queue<CommandType> _queue;
public:
    void Write(CommandType command);
    std::optional<CommandType> Read();
};