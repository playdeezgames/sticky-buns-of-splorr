#pragma once
#include <string>
#include "framebuffercellcolor.h"
class MessageData
{
private:
    std::string _text;
    FrameBufferCellColor _foreground;
    FrameBufferCellColor _background;
    MessageData();
public:
    MessageData(
        std::string text, 
        FrameBufferCellColor foreground, 
        FrameBufferCellColor background)
        : _text(text)
        , _foreground(foreground)
        , _background(background)
        {
        }
    const std::string& GetText() const {return _text;}
    FrameBufferCellColor GetForeground() const {return _foreground;}
    FrameBufferCellColor GetBackground() const {return _background;}
};