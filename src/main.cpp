#include "raylib.h"
#include "rlgl.h"
#include "imgui.h"
#include "rlImGui.h"

#include "Block.hpp"
#include "Structs.hpp"

#define SHADER_ARRAY_SIZE 512

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

std::vector<Vector2> GetShaderReadyPoints(const Transform2D &transform, const std::vector<Vector2> &virtualPoints);

float GetPointFromFunction(float x, const std::vector<Vector2> &points)
{
    for (int i = 0; i < points.size() - 1; i++)
    {
        if (points[i + 1].x > x)
        {
            float diff = (x - points[i].x) / (points[i + 1].x - points[i].x);
            return Lerp(points[i].y, points[i + 1].y, diff);
        }
    }
    return points.back().y;
}

void DrawDebugPoints(const std::vector<Vector2> &points, Color color, Vector2 position, Vector2 scale)
{
    for (int i = 0; i < points.size() - 1; i++)
    {
        Vector2 firstPoint = Vector2Add(points[i], position);
        Vector2 secondPoint = Vector2Add(points[i + 1], position);
        DrawLineV(Vector2Multiply(firstPoint, scale), Vector2Multiply(secondPoint, scale), color);
    }
}

int main()
{
    //WINDOW INIT
    Vector2 screenSize = {(float) 1000, (float) 600};


    InitWindow((int) screenSize.x, (int) screenSize.y, "Try Hard Wings");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(0);


//    Image icon = LoadImage("assets/david.png");
//    SetWindowIcon(icon);

    // GAME INIT
    BlockList blockList(15);
    Player player;

    Shader shader = LoadShader(0, TextFormat("assets/terrain%i.fs", GLSL_VERSION));
    int pointListLoc = GetShaderLocation(shader, "pointList");
    int offsetLoc = GetShaderLocation(shader, "offset");
    int resolutionLoc = GetShaderLocation(shader, "resolution");
    int upColorLoc = GetShaderLocation(shader, "upColor");
    int downColorLoc = GetShaderLocation(shader, "downColor");

    Vector4 upColor = ColorNormalize(DARKBLUE);
    Vector4 downColor = ColorNormalize(DARKBROWN);
    SetShaderValue(shader, upColorLoc, &upColor, UNIFORM_VEC4);
    SetShaderValue(shader, downColorLoc, &downColor, UNIFORM_VEC4);
    SetShaderValue(shader, resolutionLoc, &screenSize, UNIFORM_VEC2);
    RenderTexture2D target;
    bool shouldRefreshTarget = true;
    double lastTimeSinceResize = 0;

    float lerpSpeed = 10;
    float targetHeight = screenSize.y / 3;

    Transform2D transform =
            {
                    {0,   (float) GetScreenHeight() / 2},
                    {1.0, 1.0},
            };
    float speed = 100;

    SetupRLImGui(true);
    while (!WindowShouldClose())
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
            target = LoadRenderTexture((int) screenSize.x, (int) screenSize.y);
            SetShaderValue(shader, resolutionLoc, &screenSize, UNIFORM_VEC2);
            shouldRefreshTarget = false;
        }

        // INPUTS
        if (IsKeyPressed(KEY_KP_ADD))
        {
            speed *= 2;
        }
        if (IsKeyPressed(KEY_KP_SUBTRACT))
        {
            speed /= 2;
        }

        transform.position.x += GetFrameTime() * speed;
        transform.position.y = Lerp(transform.position.y, screenSize.y - targetHeight - blockList.GetFirstPoint().y,
                                    GetFrameTime() * lerpSpeed);

        if (transform.position.x > blockList.GetFirstBlockWidth())
        {
            blockList.Shift();

            if (transform.position.x > 10000.f)
            {
                transform.position.x -= blockList.GetFirstPoint().x;
                blockList.ResetOffset();
            }
        }

        auto virtualPoints = blockList.GetPointList();

        transform.position.y += GetMouseWheelMove() * 10;

        std::vector<Vector2> real_points = GetShaderReadyPoints(transform, virtualPoints);

        SetShaderValueV(shader, pointListLoc, real_points.data(), UNIFORM_VEC2, (int) real_points.size());
        SetShaderValue(shader, offsetLoc, &transform.position, UNIFORM_VEC2);

        BeginDrawing();
        ClearBackground(WHITE);
        BeginShaderMode(shader);
        DrawTextureRec(target.texture,
                       (Rectangle) {0, 0, (float) target.texture.width, (float) target.texture.height},
                       (Vector2) {0, 0},
                       WHITE);
        EndShaderMode();

        Vector2 startPoint = blockList.GetFirstPoint();

        Vector2 realPointsOffset = {transform.position.x - startPoint.x, transform.position.y};
        Vector2 virtualPointsOffset = {-startPoint.x, transform.position.y + 5.f};
        static Vector2 curvePos = Vector2{50.f, 50.f};
        static Vector2 curveScale = Vector2{0.5f, 0.5f};
        Transform2D debugOffset = {
                curvePos,
                curveScale,
        };

        DrawDebugPoints(real_points, GREEN, Vector2Add(debugOffset.position, realPointsOffset), debugOffset.scale);

        DrawDebugPoints(virtualPoints, RED, Vector2Add(debugOffset.position, virtualPointsOffset), debugOffset.scale);

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
            ImGui::Begin("Debug", &isDebugOpen);
            ImGui::DragFloat("Speed", &speed);
            ImGui::DragFloat("lerpSpeed", &lerpSpeed);
            ImGui::DragFloat("targetHeight", &targetHeight);
            ImGui::DragFloat2("Horizontal Scale", (float *) &transform.scale, 0.01);
            ImGui::DragFloat2("CurvePos", (float *) &curvePos);
            ImGui::DragFloat2("curveScale", (float *) &curveScale, 0.01);
            ImGui::End();
        }
        EndRLImGui();

        EndDrawing();
    }
    ShutdownRLImGui();
    CloseWindow();
    return 0;
}

std::vector<Vector2>
GetShaderReadyPoints(const Transform2D &transform, const std::vector<Vector2> &virtualPoints)
{
    std::vector<Vector2> real_points;
    real_points.reserve(SHADER_ARRAY_SIZE);
    for (int i = 0; i < SHADER_ARRAY_SIZE; i++)
    {
        float step = (float) i / SHADER_ARRAY_SIZE;
        float x = transform.position.x + step * SHADER_ARRAY_SIZE * transform.scale.x;
        Vector2 point = {
                x - transform.position.x,
                GetPointFromFunction(x, virtualPoints) * transform.scale.y,
        };
        real_points.push_back(point);
    }
    return real_points;
}
