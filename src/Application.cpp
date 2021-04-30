#include "Application.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Utils.hpp"

#include "imgui.h"
#include "rlImGui.h"

void Application::SetupEditorStyle()
{
    ImGuiStyle& editorStyle = ImGui::GetStyle();
    editorStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.5f);
    editorStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);
    editorStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);
    editorStyle.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);
    editorStyle.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.8f);
    editorStyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);
    editorStyle.Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.8f);
    editorStyle.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);

    editorStyle.FrameRounding = 5;
    editorStyle.WindowRounding = 5;
}

Application::Application(Vector2 screenSize)
: blockList(30)
, terrainShader(TextFormat(TextFormat("assets/terrain%i.fs", GLSL_VERSION)), DARKBLUE, DARKBROWN)
, screenSize(screenSize)
{
    SetShaderValue(terrainShader.shader,
                   terrainShader.resolutionLoc,
                   &screenSize,
                   UNIFORM_VEC2);

    origin = {screenSize.x, screenSize.y / 2};
    SetupRLImGui(true);

    auto virtualPoints = blockList.GetPointList();
    player.position.y = GetPointFromFunction( player.position.x + 15.f * scale.x, virtualPoints) * scale.y;
    gameConfig.targetHeight = player.position.y;
    gameConfig.highScore = 0.f;
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

    player.Update();

    scale.x = logf(player.velocity.x/scaleSpeed + 1) + 1;
    yOffset = origin.y - player.position.y;

    while (player.position.x > blockList.GetFirstBlockWidth())
    {
        blockList.Shift();
        player.position.x -= blockList.GetFirstPoint(0).x;
        blockList.ResetOffset();
    }

    auto virtualPoints = blockList.GetPointList();
    int lastIndex = 0;
    std::vector<Vector2> realPoints = GetShaderReadyPoints(
            Transform2D{
                {player.position.x, 0},
                Vector2Multiply(scale, {screenSize.x / 1000.f, 1.0f})
                        },
                        virtualPoints);

    float heightUnderPlayer = GetPointFromFunction( player.position.x + 15.f * scale.x, virtualPoints) * scale.y;

    if (player.position.y > heightUnderPlayer)
    {
        player.velocity.y -= player.velocity.y * GetFrameTime();
        float derivativeUnderPlayer = blockList.GetDerivativeAt((player.position.x + 15.0f) * scale.x);
        gameConfig.targetHeight = origin.y + (player.position.y - yOffset);

        if (derivativeUnderPlayer > 0)
        {
            player.velocity.x +=  100 * derivativeUnderPlayer * GetFrameTime();
        }
        else
        {
            player.velocity.x +=  200 * derivativeUnderPlayer * GetFrameTime();
            player.velocity.y += player.velocity.x * 10 * derivativeUnderPlayer * GetFrameTime();
        }
        if (player.velocity.x < 20)
            player.velocity.x = 20;

        player.position.y = heightUnderPlayer;
    }

    float playerHeight = 0.5 * gameConfig.targetHeight - player.position.y;
    gameConfig.highScore = (gameConfig.highScore < playerHeight) ? playerHeight : gameConfig.highScore;
    gameConfig.currentScore = playerHeight;

    SetShaderValueV(terrainShader.shader, terrainShader.pointListLoc, realPoints.data(), UNIFORM_VEC2, (int) realPoints.size());

    Vector2 realOffset = {0, gameConfig.targetHeight - 2 * player.position.y};
    SetShaderValue(terrainShader.shader, terrainShader.offsetLoc, &yOffset, UNIFORM_FLOAT);

    BeginDrawing();
    DrawGameState(heightUnderPlayer);
    DrawDebug(virtualPoints, realPoints);
    EndDrawing();

}

Application::~Application()
{
    UnloadRenderTexture(target);
    ShutdownRLImGui();
    CloseWindow();
}
void Application::DrawGameState(float heightUnderPlayer)
{
    ClearBackground(WHITE);
    BeginShaderMode(terrainShader.shader);
    BeginTextureMode(target);
    ClearBackground(BLANK);
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
    DrawCircle(30.f, origin.y, 5, MAGENTA);

    DrawText(TextFormat("Press F1 (or touch with 3 fingers) to open help"), 20, 10, 10, WHITE);
    DrawText(TextFormat("High Score: %.2f", gameConfig.highScore), 20, 30, 20, WHITE);
    DrawText(TextFormat("Current Score: %.2f", gameConfig.currentScore), 20, 50, 20, WHITE);
}
void Application::DrawDebug(const std::vector<Vector2>& virtualPoints, const std::vector<Vector2>& realPoints)
{
    BeginRLImGui();
    SetupEditorStyle();

    static bool isMenuOpen = false;
    static bool isMetricsOpen = false;
    static bool isHelpOpen = false;


#ifndef NDEBUG
    static bool isDebugOpen = true;
    static bool drawDebugGizmo = true;
#else
    static bool isDebugOpen = false;
    static bool drawDebugGizmo = false;
#endif

    //ImGui::ShowDemoWindow();

    if (IsKeyPressed(KEY_F1))
        isMenuOpen = !isMenuOpen;
    if (GetTouchPointsCount() == 3)
        isMenuOpen = true;
    if (IsKeyPressed(KEY_F2))
        isDebugOpen = !isDebugOpen;
    if (isMenuOpen)
    {
        ImGui::Begin("Help Menu", &isMenuOpen, ImGuiWindowFlags_MenuBar);
        if(ImGui::BeginMenuBar())
        {
            ImGui::Text("FPS: %i", GetFPS());
            ImGui::Separator();
            if (ImGui::BeginMenu("Windows"))
            {
                if (ImGui::MenuItem("Open Debug", nullptr, isDebugOpen))
                    isDebugOpen = true;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::Text("Select Generation Profile");
        ImGui::Indent();
        #define GENERATE_MENU_ITEM(function) if(ImGui::MenuItem(#function, nullptr, blockList.profile == BlockList::Profile::function)) \
                                        blockList.profile = BlockList::Profile::function;
        GENERATE_MENU_ITEM(SIN);
        GENERATE_MENU_ITEM(TAN);
        GENERATE_MENU_ITEM(ELLIPTIC);
        GENERATE_MENU_ITEM(POLYNOMIAL);
        GENERATE_MENU_ITEM(RANDOM);
        #undef GENERATE_MENU_ITEM
        ImGui::Unindent();

        ImGui::Text("Inputs");
        ImGui::Indent();
        ImGui::Text("Desktop:");
        ImGui::Indent();
        ImGui::Text("Space: up");
        ImGui::Text("+/D/->: Speed up");
        ImGui::Text("-/A/<-: Slow Down");
        ImGui::Unindent();
        ImGui::Unindent();
        ImGui::Indent();
        ImGui::Text("Mobile:");
        ImGui::Indent();
        ImGui::Text("One touch : Up");
        ImGui::Text("Two touch : Speed up if touching on rightside, else slow down");
        ImGui::Unindent();

        ImGui::End();
    }

    std::vector<Vector2> derivatives = blockList.GetPointDerivativeList();
    if (isDebugOpen)
    {
        ImGui::Begin("Debug", &isDebugOpen);
        ImGui::Checkbox("Show Debug Gizmos", &drawDebugGizmo);
        ImGui::DragFloat("Speed", &player.velocity.x);
        ImGui::DragFloat2("PlayerPosition", (float*)&player.position);
        ImGui::DragFloat2("Origin", (float*)&origin);
        ImGui::Text("Derivative at player pos : %.2f", blockList.GetDerivativeAt(player.position.x));
        ImGui::Text("y Offset: %.2f", yOffset);
        ImGui::Text("Scale %.2f, %.2f", scale.x, scale.y);
        ImGui::Text("Player: %.2f,\t%.2f", player.position.x , player.position.y);
        ImGui::Text("targetHeight: %.2f", gameConfig.targetHeight * 0.5f);
        ImGui::Text("Velocity: %.2f,\t%.2f", player.velocity.x, player.velocity.y);
        ImGui::Text("ScreenSize: %.2f,\t%.2f", screenSize.x, screenSize.y);
        ImGui::End();
    }

    if (drawDebugGizmo)
    {
        float heightUnderPlayer = GetPointFromFunction( player.position.x + 15.f * scale.x, virtualPoints) * scale.y;
        Vector2 collisionPointUnderPlayer = {30.f, heightUnderPlayer + origin.y - player.position.y};

        DrawDebugPoints(realPoints, GREEN, {0,origin.y}, {1, 1});
        DrawDebugPoints(virtualPoints, RED, {0, origin.y + 3.f}, {1, scale.y});
        DrawDebugPoints(derivatives, BLUE, {0, origin.y}, {1, scale.y});

        DrawLine(0, gameConfig.currentScore + origin.y,
                 screenSize.x, gameConfig.currentScore + origin.y, ColorAlpha(GRAY, 0.5));
        DrawLine(30, 0,
                 30, screenSize.y, ColorAlpha(GRAY, 0.5));
        DrawCircle( collisionPointUnderPlayer.x, collisionPointUnderPlayer.y, 3, LIME);

        DrawLineV(collisionPointUnderPlayer, Vector2Add(collisionPointUnderPlayer, {0, blockList.GetDerivativeAt((player.position.x + 15.0f) * scale.x) * 25}), PINK);

    }
    EndRLImGui();
}

