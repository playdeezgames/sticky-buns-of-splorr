#pragma once
#include "state.h"
#include <vector>
#include "world.h"
class RoomState: public State
{
private:
    RoomState();
    World& _world;
    size_t x;
    size_t y;
    bool HandleCommand();
    void AttemptMove();
    void Move(Location fromLocation, Location toLocation);
    void RemoveBlocks();
    void ConsumeStickyBuns(Character& character, Character& otherCharacter);
    void CheckButthole(Character& character, Character& otherCharacter);
    void DrawBoard();
    void DrawCursorFrame();
    void DrawStats();
    void DrawStatusBar();
    static bool IsValidMoveDestination(Location location);
public:
    RoomState(
        FrameBuffer& frameBuffer, 
        CommandBuffer& commandBuffer,
        World& world)
        : State(frameBuffer, commandBuffer)
        , _world(world)
        , x(0)
        , y(0)
        {

        }
    void Draw() override;
    std::optional<GameState> Update() override;
};