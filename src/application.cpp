#include "application.h"
#include <string>
#include "roomstate.h"
#include <ranges>
constexpr int SCALE = 4;
constexpr int SCREEN_WIDTH = VIEW_WIDTH * SCALE;
constexpr int SCREEN_HEIGHT = VIEW_HEIGHT * SCALE;
constexpr int BOARD_COLUMNS = 40;
constexpr int BOARD_ROWS = 22;
constexpr std::string_view GAME_TITLE = "The Blue Room";
bool Application::quit = false;
SDL_Renderer* Application::renderer = nullptr;
SDL_Window* Application::window = nullptr;
std::map<GameState, std::unique_ptr<State>> Application::states;
GameState Application::gameState = GameState::ROOM;
Board Application::board(BOARD_COLUMNS, BOARD_ROWS);
void Application::Initialize()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        GAME_TITLE.data(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, 
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, VIEW_WIDTH, VIEW_HEIGHT);
    states.emplace(GameState::ROOM, std::make_unique<RoomState>(board));
    for(auto column = 0; column < BOARD_COLUMNS; ++column)
    {
      for(auto row = 0; row < BOARD_ROWS; ++row)
      {
        board.GetColumn(column).GetCell(row).SetCellType(((column == 0) || (row == 0) || (column == BOARD_COLUMNS - 1) || (row == BOARD_ROWS - 1))?(CellType::WALL):(CellType::FLOOR));
      }
    }
    board.GetColumn(1).GetCell(1).SetCellType(CellType::N00B);
}
void Application::Loop()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    auto nextState = states[gameState]->HandleEvent(event);
    if(nextState.has_value())
    {
      gameState = *nextState;
    }
    else
    {
      quit = true;
    }
  }
  if(!quit)
  {
    states[gameState]->Draw(renderer);
    SDL_RenderPresent(renderer);
  }
}
void Application::CleanUp()
{
    if(renderer)
    {
      SDL_DestroyRenderer(renderer);
      renderer = nullptr;
    }
    if(window)
    {
      SDL_DestroyWindow(window);
      window = nullptr;
    }
    SDL_Quit();
}
void Application::Run()
{
    for(;!quit;)
    {
      Loop();
    }
    CleanUp();
}
