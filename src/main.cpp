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
            pos.y -= 250 * GetFrameTime();
        }
        pos.y += 100 * GetFrameTime();

        DrawRectangleV(pos, {50, 50}, MAGENTA);
    }
};

int main()
{
    //WINDOW INIT
    int screenWidth = 800;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Try Hard Wings");
//    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(0);


//    Image icon = LoadImage("assets/david.png");
//    SetWindowIcon(icon);

    // GAME INIT
    BlockList blockList(10);
    Player player;

    Vector2 resolution = {(float)screenWidth, (float)screenHeight};

    Shader shader = LoadShader(0, "assets/shader.fs");
    int pointListLoc = GetShaderLocation(shader, "pointList");
    int offsetLoc = GetShaderLocation(shader, "offset");
    int resolutionLoc = GetShaderLocation(shader, "resolution");
    int upColorLoc = GetShaderLocation(shader, "upColor");
    int downColorLoc = GetShaderLocation(shader, "downColor");

    Vector4 upColor = ColorNormalize(SKYBLUE);
    Vector4 downColor = ColorNormalize(BROWN);
    SetShaderValue(shader, upColorLoc, &upColor, UNIFORM_VEC4);
    SetShaderValue(shader, downColorLoc, &downColor, UNIFORM_VEC4);
    SetShaderValue(shader, resolutionLoc, &resolution, UNIFORM_VEC2);
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    Vector2 offset = {0, (float) GetScreenHeight() / 2};
    float speed = 100;

    SetupRLImGui(true);
    while (!WindowShouldClose())
    {

        // INPUTS
        if (IsKeyPressed(KEY_KP_ADD))
        {
            speed *= 2;
        }
        if (IsKeyPressed(KEY_KP_SUBTRACT))
        {
            speed /= 2;
        }


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

        auto points = blockList.GetPointList();

        offset.x = sinf(GetTime()) * 100;
        SetShaderValueV(shader, pointListLoc, points.data(), UNIFORM_VEC2, (int)points.size());
        SetShaderValue(shader, offsetLoc, &offset, UNIFORM_VEC2);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginTextureMode(target);
        ClearBackground(SKYBLUE);
        EndTextureMode();


        BeginShaderMode(shader);
        DrawTextureRec(target.texture,
                       (Rectangle) {0, 0, (float) target.texture.width, (float) -target.texture.height},
                       (Vector2) {0, 0},
                       WHITE);
        EndShaderMode();

        for (int i = 0; i < points.size() - 1; i++)
        {
            DrawLineV(Vector2Add(points[i], offset), Vector2Add(points[i + 1], offset), DARKBLUE);
        }

        BeginRLImGui();
        ImGui::Begin("Debug");

        ImGui::Text("FPS: %i", GetFPS());
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