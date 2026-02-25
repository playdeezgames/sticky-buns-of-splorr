#pragma once
#include "state.h"
#include <vector>
#include "board.h"
enum class CellState {FLOOR, WALL, N00B};
class RoomState: public State
{
private:
    Board& _board;
    RoomState();
    std::optional<GameState> HandleKeyEvent(const SDL_KeyboardEvent& key);
    std::optional<GameState> Move(int deltaX, int deltaY);
    int GetX() const;
    int GetY() const;
public:
    RoomState(Board& board): _board(board) {}
    void Draw(SDL_Renderer* renderer) override;
    std::optional<GameState> HandleEvent(const SDL_Event& event) override;
};