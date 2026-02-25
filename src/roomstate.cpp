#include "roomstate.h"
constexpr int CELL_WIDTH = 8;
constexpr int CELL_HEIGHT = 8;
void RoomState::Draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for(auto column = 0; column < _board.GetColumns(); ++column)
    {
        const auto& boardColumn = _board.GetColumn(column);
        for(auto row = 0; row < boardColumn.GetRows(); ++row)
        {
            const auto& cell = boardColumn.GetCell(row);
            switch(cell.GetCellType())
            {
                case CellType::FLOOR:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    break;
                case CellType::WALL:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    break;
                case CellType::N00B:
                    SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
                    break;
                default:
                    throw "dint work";
            }
            SDL_Rect rect = {column * CELL_WIDTH, row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

std::optional<GameState> RoomState::HandleEvent(const SDL_Event& event)
{
    if(event.type == SDL_QUIT)
    {
        return std::nullopt;
    }
    if(event.type == SDL_KEYDOWN)
    {
        return HandleKeyEvent(event.key);
    }
    return GameState::ROOM;
}
std::optional<GameState> RoomState::HandleKeyEvent(const SDL_KeyboardEvent& key)
{
    switch(key.keysym.sym)
    {
        case SDLK_w:
            return Move(0,-1);
        case SDLK_s:
            return Move(0,1);
        case SDLK_a:
            return Move(-1,0);
        case SDLK_d:
            return Move(1, 0);
    }
    return GameState::ROOM;
}
std::optional<GameState> RoomState::Move(int deltaX, int deltaY)
{
    int x = GetX();
    int y = GetY();
    int nextX = x + deltaX;
    int nextY = y + deltaY;
    if(
        (nextX>=0) && 
        (nextY>=0) && 
        (nextX<=_board.GetColumns()-1) && 
        (nextY<=_board.GetColumn(nextX).GetRows()-1) &&
        (_board.GetColumn(nextX).GetCell(nextY).GetCellType() == CellType::FLOOR))
    {
        _board.GetColumn(nextX).GetCell(nextY).SetCellType(CellType::N00B);
        _board.GetColumn(x).GetCell(y).SetCellType(CellType::FLOOR);
    }
    return GameState::ROOM;
}
int RoomState::GetX() const
{
    for(int x = 0; x<_board.GetColumns();++x)
    {
        for(int y = 0; y<_board.GetColumn(x).GetRows();++y)
        {
            if(_board.GetColumn(x).GetCell(y).GetCellType()==CellType::N00B)
            {
                return x;
            }
        }
    }
    throw "dint work";
}
int RoomState::GetY() const
{
    for(int x = 0; x<_board.GetColumns();++x)
    {
        for(int y = 0; y<_board.GetColumn(x).GetRows();++y)
        {
            if(_board.GetColumn(x).GetCell(y).GetCellType()==CellType::N00B)
            {
                return y;
            }
        }
    }
    throw "dint work";
}

