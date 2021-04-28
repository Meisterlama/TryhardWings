#pragma once

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

struct Transform2D
{
    Vector2 position = Vector2{0.f, 0.f};
    Vector2 scale = Vector2{0.f, 0.f};
};

struct TerrainShader
{
    Shader shader;
    int pointListLoc;
    int offsetLoc;
    int resolutionLoc;
    int upColorLoc;
    int downColorLoc;

    TerrainShader(const char* fsFileName)
    {
        shader = LoadShader(0, fsFileName);
        pointListLoc = GetShaderLocation(shader, "pointList");
        offsetLoc = GetShaderLocation(shader, "offset");
        resolutionLoc = GetShaderLocation(shader, "resolution");
        upColorLoc = GetShaderLocation(shader, "upColor");
        downColorLoc = GetShaderLocation(shader, "downColor");
    }
};