#include "roomstate.h"
constexpr int CELL_WIDTH = 8;
constexpr int CELL_HEIGHT = 8;
void RoomState::Draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

std::optional<GameState> RoomState::HandleEvent(const SDL_Event& event)
{
    if(event.type == SDL_QUIT)
    {
        return std::nullopt;
    }
    return GameState::ROOM;
}
