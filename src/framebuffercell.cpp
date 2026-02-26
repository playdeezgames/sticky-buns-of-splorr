#include "framebuffercell.h"
void FrameBufferCell::SetCharacter(unsigned char character)
{
    _character = character;
}
void FrameBufferCell::SetForeground(FrameBufferCellColor foreground)
{
    _foreground = foreground;
}
void FrameBufferCell::SetBackground(FrameBufferCellColor background)
{
    _background = background;
}
unsigned char FrameBufferCell::GetCharacter() const
{
    return _character;
}
FrameBufferCellColor FrameBufferCell::GetForeground() const
{
    return _foreground;
}
FrameBufferCellColor FrameBufferCell::GetBackground() const
{
    return _background;
}
