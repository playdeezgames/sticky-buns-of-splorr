#include "message.h"
const std::string_view& Message::GetText() const
{
    return GetMessageData().GetText();
}
FrameBufferCellColor Message::GetForeground() const
{
    return GetMessageData().GetForeground();
}
FrameBufferCellColor Message::GetBackground() const
{
    return GetMessageData().GetBackground();
}
