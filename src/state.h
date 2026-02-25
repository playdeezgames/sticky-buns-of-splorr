#pragma once
#include <SDL2/SDL.h>
#include "gamestate.h"
#include <optional>
constexpr int VIEW_WIDTH = 320;
constexpr int VIEW_HEIGHT = 180;
class State
{
private:
public:
    virtual void Draw(SDL_Renderer* renderer) = 0;
    virtual std::optional<GameState> HandleEvent(const SDL_Event& event) = 0;
};