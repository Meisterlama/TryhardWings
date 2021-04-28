#include "Application.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Utils.hpp"

#if 1//defined(PLATFORM_DESKTOP)
#include "imgui.h"
#include "rlImGui.h"
#endif

Application::Application(Vector2 screenSize)
: blockList(15)
, terrainShader(TextFormat(TextFormat("assets/terrain%i.fs", GLSL_VERSION)), DARKBLUE, DARKBROWN)
{
    SetShaderValue(terrainShader.shader,
                   terrainShader.resolutionLoc,
                   &screenSize,
                   UNIFORM_VEC2);
    transform =
            {
                    {0,   (float) GetScreenHeight() / 2},
                    {1.0, 1.0},
            };
    miniMap = LoadRenderTexture(200, 200);
#if 1//defined(PLATFORM_DESKTOP)
    SetupRLImGui(true);
#endif
}

void Application::Update()
{
    // WINDOW RESIZING
    if (IsWindowResized())
    {
        screenSize = Vector2{(float) GetScreenWidth(), (float) GetScreenHeight()};
        shouldRefreshTarget = true;
        lastTimeSinceResize = GetTime();
    }
    else if (shouldRefreshTarget && GetTime() - lastTimeSinceResize > 0.1f)
    {
        TraceLog(LOG_INFO, "ScreenSize: %f, %f", screenSize.x, screenSize.y);
        UnloadRenderTexture(target);
        target = LoadRenderTexture((int) screenSize.x, (int) screenSize.y);
        SetShaderValue(terrainShader.shader, terrainShader.resolutionLoc, &screenSize, UNIFORM_VEC2);
        shouldRefreshTarget = false;
    }

    // INPUTS
    if (IsKeyPressed(KEY_KP_ADD))
    {
        gameConfig.speed *= 2;
    }
    if (IsKeyPressed(KEY_KP_SUBTRACT))
    {
        gameConfig.speed /= 2;
    }

    transform.position.x += GetFrameTime() * gameConfig.speed;
    transform.position.y = Lerp(transform.position.y, screenSize.y - gameConfig.targetHeight - blockList.GetFirstPoint().y,
                                gameConfig.lerpSpeed);

//    player.Update();

    while (transform.position.x > blockList.GetFirstBlockWidth())
    {
        blockList.Shift();

        if (transform.position.x > 10000.f)
        {
            transform.position.x -= blockList.GetFirstPoint().x;
            blockList.ResetOffset();
        }
    }

    auto virtualPoints = blockList.GetPointList();
    std::vector<Vector2> real_points = GetShaderReadyPoints(transform, virtualPoints);

    SetShaderValueV(terrainShader.shader, terrainShader.pointListLoc, real_points.data(), UNIFORM_VEC2, (int) real_points.size());
    SetShaderValue(terrainShader.shader, terrainShader.offsetLoc, &transform.position, UNIFORM_VEC2);

    BeginDrawing();
    ClearBackground(WHITE);
    BeginShaderMode(terrainShader.shader);
    BeginTextureMode(target);
    DrawTextureRec(target.texture,
                   (Rectangle) {0, 0, (float) target.texture.width, (float) -target.texture.height},
                   (Vector2) {0, 0},
                   WHITE);
    EndShaderMode();
    EndTextureMode();

    DrawTexture(target.texture, 0, 0, WHITE);

    Vector2 startPoint = blockList.GetFirstPoint();

    Vector2 realPointsOffset = {transform.position.x - startPoint.x, transform.position.y};
    Vector2 virtualPointsOffset = {-startPoint.x, transform.position.y + 5.f};
    static Vector2 curvePos = Vector2{50.f, 50.f};
    static Vector2 curveScale = Vector2{0.5f, 0.5f};
    Transform2D debugOffset = {
            curvePos,
            curveScale,
    };

    DrawRectangleV(Vector2Add(player.pos, {0, transform.position.y}), Vector2{10, 10}, MAGENTA);

#if 1//defined(PLATFORM_DESKTOP)
    BeginTextureMode(miniMap);
    ClearBackground(BLANK);
    DrawDebugPoints(real_points, GREEN, Vector2Subtract(realPointsOffset, {0, gameConfig.targetHeight}), debugOffset.scale);
    DrawDebugPoints(virtualPoints, RED, Vector2Subtract(virtualPointsOffset, {0, gameConfig.targetHeight}), debugOffset.scale);
    EndTextureMode();

    BeginRLImGui();
    static bool isDebugOpen = true;
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("FPS: %i", GetFPS());
        ImGui::Separator();
        ImGui::Text("Offset: %f, %f", transform.position.x, transform.position.y);
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
        ImGui::DragFloat("Speed", &gameConfig.speed);
        ImGui::DragFloat("lerpSpeed", &gameConfig.lerpSpeed, 0.01, 0, 1, "%.3f", ImGuiSliderFlags_Logarithmic);
        ImGui::DragFloat("targetHeight", &gameConfig.targetHeight);
        ImGui::DragFloat2("Horizontal Scale", (float *) &transform.scale, 0.01);
        ImGui::DragFloat2("CurvePos", (float *) &curvePos);
        ImGui::DragFloat2("curveScale", (float *) &curveScale, 0.01);
        RLImGuiImage(&miniMap.texture);
        ImGui::End();
    }
    EndRLImGui();
#endif
    EndDrawing();
}

Application::~Application()
{
#if defined(PLATFORM_DESKTOP)
    ShutdownRLImGui();
#endif
    CloseWindow();
}
