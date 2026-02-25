#pragma once
#include <SDL2/SDL.h>
#include "state.h"
#include <memory>
#include <map>
#include "gamestate.h"
#include "board.h"
class Application
{
private:
    static bool quit;
    static SDL_Renderer* renderer;
    static SDL_Window* window;
    static std::map<GameState, std::unique_ptr<State>> states;
    static GameState gameState;
    static Board board;
    static void CleanUp();
public:
    static void Initialize();
    static void Loop();
    static void Run();
};