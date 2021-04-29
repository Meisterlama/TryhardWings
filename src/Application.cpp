#include "Application.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Utils.hpp"

#include "imgui.h"
#include "rlImGui.h"

Application::Application(Vector2 screenSize)
: blockList(20)
, terrainShader(TextFormat(TextFormat("assets/terrain%i.fs", GLSL_VERSION)), DARKBLUE, DARKBROWN)
{
    SetShaderValue(terrainShader.shader,
                   terrainShader.resolutionLoc,
                   &screenSize,
                   UNIFORM_VEC2);

    origin = {screenSize.x, screenSize.y / 2};
    gameConfig.targetHeight = origin.y;
    SetupRLImGui(true);
}

void Application::Update()
{
    // WINDOW RESIZING
    if (IsWindowResized())
    {
        screenSize = Vector2{(float) GetScreenWidth(), (float) GetScreenHeight()};
        origin = {screenSize.x, screenSize.y / 2};
        shouldRefreshTarget = true;
        lastTimeSinceResize = GetTime();
    }
    else if (shouldRefreshTarget && GetTime() - lastTimeSinceResize > 0.1f)
    {
        UnloadRenderTexture(target);
        target = LoadRenderTexture((int) screenSize.x, (int) screenSize.y);
        SetShaderValue(terrainShader.shader, terrainShader.resolutionLoc, &screenSize, UNIFORM_VEC2);
        shouldRefreshTarget = false;
    }

    // INPUTS
    if (IsKeyDown(KEY_KP_ADD))
    {
        player.velocity.x += 2;
    }
    if (IsKeyDown(KEY_KP_SUBTRACT))
    {
        player.velocity.x -= 2;
    }

    if(IsKeyPressed(KEY_F1))
    {
        blockList.Shift();
        player.position.x -= blockList.GetFirstPoint().x;
        blockList.ResetOffset();
    }

//    offset.y = Lerp(offset.y,gameConfig.targetHeight - blockList.GetFirstPoint().y,
//                                gameConfig.lerpSpeed);

    player.Update();

    scale.x = logf(player.velocity.x/scaleSpeed + 1) + 1;

    offset.y = origin.y - player.position.y;

    while (player.position.x > blockList.GetFirstBlockWidth())
    {
        blockList.Shift();
        player.position.x -= blockList.GetFirstPoint().x;
        blockList.ResetOffset();
    }

    auto virtualPoints = blockList.GetPointList();
    std::vector<Vector2> realPoints = GetShaderReadyPoints({{player.position.x, 0}, scale}, virtualPoints);

    float heightUnderPlayer = GetPointFromFunction( player.position.x + 15 * scale.x, virtualPoints) * scale.y;
    if (player.position.y > heightUnderPlayer)
    {
        player.velocity.y = 0;
        gameConfig.targetHeight = origin.y + (player.position.y - offset.y);
//        player.velocity.y -= (player.position.y - heightUnderPlayer);
//        player.position.y = heightUnderPlayer;
    }


    SetShaderValueV(terrainShader.shader, terrainShader.pointListLoc, realPoints.data(), UNIFORM_VEC2, (int) realPoints.size());

    Vector2 realOffset = {0, gameConfig.targetHeight - 2 * player.position.y};
    SetShaderValue(terrainShader.shader, terrainShader.offsetLoc, &offset, UNIFORM_VEC2);

    BeginDrawing();
    DrawGameState(heightUnderPlayer);
    DrawDebug(virtualPoints, realPoints);
    DrawCircle(30, origin.y, 5, MAGENTA);
    DrawCircle( 30,  heightUnderPlayer + origin.y - player.position.y, 3, LIME);
    EndDrawing();

}

Application::~Application()
{
    ShutdownRLImGui();
    CloseWindow();
}
void Application::DrawGameState(float heightUnderPlayer)
{
    ClearBackground(WHITE);
    BeginShaderMode(terrainShader.shader);
    BeginTextureMode(target);
    DrawTextureRec(target.texture,
                   Rectangle {0, 0, (float) target.texture.width, (float) target.texture.height},
                   Vector2 {0, 0},
                   WHITE);
    EndShaderMode();
    EndTextureMode();

    DrawTextureRec(target.texture,
                   Rectangle {0, 0, (float) target.texture.width, (float) -target.texture.height},
                   Vector2 {0, 0},
                   WHITE);

}
void Application::DrawDebug(const std::vector<Vector2>& virtualPoints, const std::vector<Vector2>& realPoints)
{
    Vector2 virtualPointsOffset = {offset.x, offset.y + 3.f};
    Vector2 realPointsOffset = {0, 0};

    DrawDebugPoints(realPoints, GREEN, {0,origin.y}, {1, 1});
    DrawDebugPoints(virtualPoints, RED, {0, origin.y + 3.f}, {1, scale.y});

    float lineHeight = 0.5 * gameConfig.targetHeight + origin.y - player.position.y;
    DrawLine(0, lineHeight,
             screenSize.x, lineHeight, GRAY);
    DrawLine(30, 0,
             30, screenSize.y, GRAY);

    BeginRLImGui();
    static bool isDebugOpen = true;
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("FPS: %i", GetFPS());
        ImGui::Separator();
        if(ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Open Debug"))
                isDebugOpen = true;
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    if (isDebugOpen)
    {
        ImGui::SetNextWindowPos({4.0f/5.0f * screenSize.x, 30});
        ImGui::SetNextWindowSize({1.0f/5.0f * screenSize.x, 4.0f/5.0f * screenSize.y});
        ImGui::Begin("Debug", &isDebugOpen);
        ImGui::DragFloat("Speed", &player.velocity.x);
        ImGui::DragFloat("scaleSpeed", &scaleSpeed, 1, 0, FLT_MAX, "%.3f");
        ImGui::DragFloat("targetHeight", &gameConfig.targetHeight);
        ImGui::DragFloat2("Offset", (float*)&offset);
        ImGui::DragFloat2("PlayerPosition", (float*)&player.position);
        ImGui::DragFloat2("Origin", (float*)&origin);
        ImGui::DragFloat2("Scale", (float*)&scale, 0.01, 0);
        ImGui::Text("Player: %.2f,\t%.2f", player.position.x , gameConfig.targetHeight - 2 * player.position.y);
        ImGui::Text("Velocity: %.2f,\t%.2f", player.velocity.x, player.velocity.y);
        ImGui::Text("lineHeight: %.2f", lineHeight);
        ImGui::End();
    }
    EndRLImGui();
}
