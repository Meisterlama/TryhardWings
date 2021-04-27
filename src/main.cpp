#include "raylib.h"
#include "rlgl.h"
#include "imgui.h"
#include "rlImGui.h"

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
    SetTargetFPS(0);


    Image icon = LoadImage("assets/david.png");
    SetWindowIcon(icon);

    // GAME INIT
//    BlockList blockList;
    Player player;

    Vector2 offset = {0, (float) GetScreenHeight() / 2};
    float speed = 100;

    SetupRLImGui(true);
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
        BeginRLImGui();

// TODO: Block drawing
//
//        //DRAWING
//        blockList.Draw(offset);
//
//        offset.x -= speed * GetFrameTime();
//
//        // BLOCK SHIFTING
//        while (offset.x <  -blockList.BlockStep)
//        {
//            offset.x += blockList.BlockStep;
//            blockList.Shift();
//        }
        player.Update();

        // UI
        DrawFPS(10, 10);

        ImGui::Begin("Debug");

        ImGui::Text("Offset: %f", offset.x);
        ImGui::DragFloat("Speed", &speed);

        ImGui::End();

        EndRLImGui();

        EndDrawing();
    }
    ShutdownRLImGui();
    CloseWindow();
    return 0;
}