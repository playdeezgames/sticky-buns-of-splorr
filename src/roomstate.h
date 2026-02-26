#pragma once
#include "state.h"
#include <vector>
class RoomState: public State
{
private:
    RoomState();
public:
    RoomState(FrameBuffer& frameBuffer, CommandBuffer& commandBuffer):State(frameBuffer, commandBuffer){}
    void Draw() override;
    std::optional<GameState> Update() override;
};