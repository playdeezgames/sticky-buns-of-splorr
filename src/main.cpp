#include "application.h"
#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
int main(int argc, char* argv[]) {
  Application::Initialize();
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(Application::Loop, 0, 1);
#else
  Application::Run();
#endif
    return 0;
}
