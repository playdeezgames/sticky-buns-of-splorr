#include "framebuffer.h"
FrameBuffer::FrameBuffer(size_t columns, size_t rows)
    : _cells(columns * rows)
    , _columns(columns)
    , _rows(rows)
{
    while(_cells.size() <_columns * rows)
    {
        _cells.push_back(FrameBufferCell());
    }
}
size_t FrameBuffer::GetColumns() const
{
    return _columns;
}
size_t FrameBuffer::GetRows() const
{
    return _rows;
}
FrameBufferCell& FrameBuffer::GetCell(size_t column, size_t row)
{
    return _cells[column + row * _columns];
}
const FrameBufferCell& FrameBuffer::GetCell(size_t column, size_t row) const
{
    return _cells[column + row * _columns];
}
void FrameBuffer::SetCell(
    size_t column, 
    size_t row, 
    std::optional<unsigned char> character, 
    std::optional<FrameBufferCellColor> foreground, 
    std::optional<FrameBufferCellColor> background)
{
    auto& cell = GetCell(column, row);
    if(character)
    {
        cell.SetCharacter(*character);
    }
    if(foreground)
    {
        cell.SetForeground(*foreground);
    }
    if(background)
    {
        cell.SetBackground(*background);
    }
}
void FrameBuffer::Fill(
    size_t column, 
    size_t row, 
    size_t columns, 
    size_t rows, 
    std::optional<unsigned char> character, 
    std::optional<FrameBufferCellColor> foreground, 
    std::optional<FrameBufferCellColor> background)
{
    for(size_t c = column; c < column + columns; ++c)
    {
        for(size_t r = row; r < row + rows; ++r)
        {
            SetCell(c, r, character, foreground, background);
        }
    }    
}
void FrameBuffer::WriteText(
    size_t column, 
    size_t row, 
    const std::string& text, 
    std::optional<FrameBufferCellColor> foreground, 
    std::optional<FrameBufferCellColor> background)
{
    for(auto character: text)
    {
        SetCell(
            column++,
            row,
            character,
            foreground,
            background);
    }
}
