#pragma once
#include "framebuffercellcolor.h"
class FrameBufferCell
{
private:
    FrameBufferCellColor _foreground;
    FrameBufferCellColor _background;
    unsigned char _character;
public:
    void SetCharacter(unsigned char character);
    void SetForeground(FrameBufferCellColor foreground);
    void SetBackground(FrameBufferCellColor background);
    unsigned char GetCharacter() const;
    FrameBufferCellColor GetForeground() const;
    FrameBufferCellColor GetBackground() const;
};