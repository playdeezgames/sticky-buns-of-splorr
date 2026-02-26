#pragma once
#include <SDL2/SDL.h>
#include "gamestate.h"
#include <optional>
#include "framebuffer.h"
#include "commandbuffer.h"
constexpr int VIEW_WIDTH = 320;
constexpr int VIEW_HEIGHT = 200;
class State
{
protected:
    FrameBuffer& _frameBuffer;
    CommandBuffer& _commandBuffer;
    State();
public:
    State(FrameBuffer& frameBuffer, CommandBuffer& commandBuffer)
        : _frameBuffer(frameBuffer)
        , _commandBuffer(commandBuffer)
    {
    }
    virtual void Draw() = 0;
    virtual std::optional<GameState> Update() = 0;
    virtual ~State() {}
};