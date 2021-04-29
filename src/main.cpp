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

void UpdateApplication()
{
    static Application application({1000.f, 600.f});
    application.Update();
}

int main()
{
    InitWindow((int) 1000, (int) 600, "Try Hard Wings");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(0);

#if defined(PLATFORM_DESKTOP)

    Application application({1000.f, 600.f});
    while (!WindowShouldClose())
    {
        application.Update();
    }
#elif defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateApplication, 60, 1);
#endif
    return 0;
}
