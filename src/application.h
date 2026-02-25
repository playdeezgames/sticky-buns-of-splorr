#pragma once
#include <SDL2/SDL.h>
#include "state.h"
#include <memory>
#include <map>
#include "gamestate.h"
#include <vector>
class Application
{
private:
    static bool quit;
    static SDL_Renderer* renderer;
    static SDL_Window* window;
    static SDL_Texture* texture;
    static std::map<GameState, std::unique_ptr<State>> states;
    static GameState gameState;
    static std::vector<SDL_Rect> src_rects;
    static std::vector<SDL_Rect> dst_rects;
    static std::vector<SDL_Color> palette;
    static void CleanUp();
public:
    static void Initialize();
    static void Loop();
    static void Run();
};