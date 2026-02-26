#include "roomstate.h"
constexpr int CELL_WIDTH = 8;
constexpr int CELL_HEIGHT = 8;
void RoomState::Draw()
{
    for(int column=0;column<24;++column)
    {
        for(int row = 0; row<24;++row)
        {
            _frameBuffer.GetCell(column, row).SetBackground((((column/3)+(row/3))%2==0)?(FrameBufferCellColor::LIGHT_GRAY):(FrameBufferCellColor::DARK_GRAY));
        }
    }
}

std::optional<GameState> RoomState::Update()
{
    return GameState::ROOM;
}
