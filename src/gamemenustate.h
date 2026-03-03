#pragma once
#include "state.h"
#include "world.h"
class GameMenuState: public State
{
private:
    GameMenuState();
    World& _world;
    bool HandleCommand(GameState& gameState);
public:
    GameMenuState(
        FrameBuffer& frameBuffer, 
        CommandBuffer& commandBuffer,
        World& world)
        : State(frameBuffer, commandBuffer)
        , _world(world)
        {

        }
    void Draw() override;
    std::optional<GameState> Update() override;
};