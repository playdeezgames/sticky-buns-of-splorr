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
