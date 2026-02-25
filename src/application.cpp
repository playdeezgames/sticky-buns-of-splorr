#include "application.h"
#include <string>
#include "roomstate.h"
#include <ranges>
#include <SDL2/SDL_image.h>
#include <cstdlib>
constexpr int SCALE = 4;
constexpr int SCREEN_WIDTH = VIEW_WIDTH * SCALE;
constexpr int SCREEN_HEIGHT = VIEW_HEIGHT * SCALE;
constexpr std::string_view GAME_TITLE = "The Blue Room";
constexpr std::string_view TEXTURE_FILENAME = "romfont8x8.png";
SDL_Texture* Application::texture = nullptr;
bool Application::quit = false;
SDL_Renderer* Application::renderer = nullptr;
SDL_Window* Application::window = nullptr;
std::map<GameState, std::unique_ptr<State>> Application::states;
GameState Application::gameState = GameState::ROOM;
std::vector<SDL_Rect> Application::src_rects;
std::vector<SDL_Rect> Application::dst_rects;
constexpr int TEXTURE_ROWS = 16;
constexpr int TEXTURE_COLUMNS = 16;
constexpr int TEXTURE_CELL_WIDTH = 8;
constexpr int TEXTURE_CELL_HEIGHT = 8;
constexpr int VIEW_COLUMNS = VIEW_WIDTH / TEXTURE_CELL_WIDTH;
constexpr int VIEW_ROWS = VIEW_HEIGHT / TEXTURE_CELL_HEIGHT;
std::vector<SDL_Color> Application::palette =
{
  {0,0,0,255},
  {0,0,170,255},
  {0,170,0,255},
  {0,170,170,255},
  {170,0,0,255},
  {170,0,170,255},
  {170,85,0,255},
  {170,170,170,255},
  {85,85,85,255},
  {85,85,255,255},
  {85,255,85,255},
  {85,255,255,255},
  {255,85,85,255},
  {255,85,255,255},
  {255,255,85,255},
  {255,255,255,255}
};
void Application::Initialize()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    src_rects.clear();
    for(int row = 0; row < TEXTURE_ROWS; ++row)
    {
      for(int column = 0; column < TEXTURE_COLUMNS; ++column)
      {
        src_rects.push_back({column * TEXTURE_CELL_WIDTH, row * TEXTURE_CELL_HEIGHT, TEXTURE_CELL_WIDTH, TEXTURE_CELL_HEIGHT});
      }
    }

    dst_rects.clear();
    for(int row = 0; row < VIEW_ROWS; ++row)
    {
      for(int column = 0; column < VIEW_COLUMNS; ++column)
      {
        dst_rects.push_back({column * TEXTURE_CELL_WIDTH, row * TEXTURE_CELL_HEIGHT, TEXTURE_CELL_WIDTH, TEXTURE_CELL_HEIGHT});
      }
    }

    window = SDL_CreateWindow(
        GAME_TITLE.data(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, 
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = IMG_LoadTexture(renderer, TEXTURE_FILENAME.data());
    SDL_RenderSetLogicalSize(renderer, VIEW_WIDTH, VIEW_HEIGHT);

    states.emplace(GameState::ROOM, std::make_unique<RoomState>());

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
    //states[gameState]->Draw(renderer);
    //SDL_SetTextureColorMod(texture, 255, 0, 255);
    for(auto dst_rect : dst_rects)
    {
      SDL_Rect src_rect = src_rects[219];
      SDL_Color color = palette[std::rand() % palette.size()];
      SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
      SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
      src_rect = src_rects[std::rand() % src_rects.size()];
      color = palette[std::rand() % palette.size()];
      SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
      SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
    }
    SDL_RenderPresent(renderer);
  }
}
void Application::CleanUp()
{
  if(texture)
  {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
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
  IMG_Quit();
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
