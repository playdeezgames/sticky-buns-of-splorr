#pragma once
#include "state.h"
#include <vector>
class RoomState: public State
{
private:
public:
    void Draw(SDL_Renderer* renderer) override;
    std::optional<GameState> HandleEvent(const SDL_Event& event) override;
};