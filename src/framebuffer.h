#pragma once
#include "framebuffercell.h"
#include <vector>
class FrameBuffer
{
private:
    std::vector<FrameBufferCell> _cells;
    size_t _columns;
    size_t _rows;
    FrameBuffer();
public:
    FrameBuffer(size_t columns, size_t rows);
    size_t GetColumns() const;
    size_t GetRows() const;
    FrameBufferCell& GetCell(size_t column, size_t row);
    const FrameBufferCell& GetCell(size_t column, size_t row) const;
};