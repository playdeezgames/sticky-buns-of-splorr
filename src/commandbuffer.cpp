#include "commandbuffer.h"
void CommandBuffer::Write(CommandType command)
{
    _queue.push(command);
}
std::optional<CommandType> CommandBuffer::Read()
{
    if(_queue.empty())
    {
        return std::nullopt;
    }
    auto result = _queue.front();
    _queue.pop();
    return result;
}
