#pragma once
#include <string_view>
#include "framebuffercellcolor.h"
class MessageData
{
private:
    std::string_view _text;
    FrameBufferCellColor _foreground;
    FrameBufferCellColor _background;
    MessageData();
public:
    MessageData(
        std::string_view text, 
        FrameBufferCellColor foreground, 
        FrameBufferCellColor background)
        : _text(text)
        , _foreground(foreground)
        , _background(background)
        {
        }
    const std::string_view& GetText() const {return _text;}
    FrameBufferCellColor GetForeground() const {return _foreground;}
    FrameBufferCellColor GetBackground() const {return _background;}
};