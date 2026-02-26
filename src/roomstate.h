#pragma once
#include "state.h"
#include <vector>
class RoomState: public State
{
private:
    RoomState();
    size_t x;
    size_t y;
    bool HandleCommand();
public:
    RoomState(FrameBuffer& frameBuffer, CommandBuffer& commandBuffer)
        : State(frameBuffer, commandBuffer)
        , x(0)
        , y(0)
        {

        }
    void Draw() override;
    std::optional<GameState> Update() override;
};