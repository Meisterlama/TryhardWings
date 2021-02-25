#include "raylib.h"
#include "rlgl.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "Block.hpp"

struct Player
{
    Vector2 pos{150, 0};

    void Update()
    {
        if (IsKeyDown(KEY_SPACE))
        {
            pos.y-= 250 * GetFrameTime();
        }
        pos.y += 100 * GetFrameTime();

        DrawRectangleV(pos, {50,50}, MAGENTA);
    }
};

int main()
{
    //WINDOW INIT
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);

    InitWindow(screenWidth, screenHeight, "Try Hard Wings");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(240);


    Image icon = LoadImage("assets/david.png");
    SetWindowIcon(icon);

    // GAME INIT
    BlockList blockList;
    Player player;

    Vector2 offset = {0, (float) GetScreenHeight() / 2};
    float speed = 100;

    while (!WindowShouldClose()) {

        // INPUTS
        if (IsKeyPressed(KEY_KP_ADD))
        {
            speed *= 2;
        }
        if (IsKeyPressed(KEY_KP_SUBTRACT))
        {
            speed /= 2;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        //DRAWING
        blockList.Draw(offset);

        offset.x -= speed * GetFrameTime();

        // BLOCK SHIFTING
        while (offset.x <  -blockList.BlockStep)
        {
            offset.x += blockList.BlockStep;
            blockList.Shift();
        }
        player.Update();

        // UI
        DrawFPS(10, 10);

        DrawText(TextFormat("Offset: %f", offset.x), 10, 48, 24, DARKGREEN);
        DrawText(TextFormat("Speed: %f", speed), 10, 72, 24, DARKGREEN);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}