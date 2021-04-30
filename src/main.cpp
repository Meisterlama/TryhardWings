#include "raylib.h"
#include "rlgl.h"

#if !defined(PLATFORM_DESKTOP) && !defined(PLATFORM_ANDROID)
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

//
#include "Block.hpp"
#include "Structs.hpp"
#include "Application.hpp"

#if defined(PLATFORM_DESKTOP)
#define START_SCREEN_WIDTH 1000
#define START_SCREEN_HEIGHT 600
#else
#define START_SCREEN_WIDTH 500
#define START_SCREEN_HEIGHT 300
#endif
//
void UpdateApplication()
{
    static Application application({START_SCREEN_WIDTH, START_SCREEN_HEIGHT});
    application.Update();
}

int main()
{
    InitWindow(START_SCREEN_WIDTH, START_SCREEN_HEIGHT, "Try Hard Wings");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(0);

#if defined(PLATFORM_DESKTOP)

    Application application({START_SCREEN_WIDTH, START_SCREEN_HEIGHT});
    while (!WindowShouldClose())
    {
        application.Update();
    }
#elif defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateApplication, 0, 1);
#endif
    return 0;
}
