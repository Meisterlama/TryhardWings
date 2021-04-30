#pragma once
#include <raymath.h>
#include "raylib.h"

struct Player
{
    Vector2 position = {60.0f, 0.0f};
    Vector2 velocity = {1.0f, 0.0f};
    float acceleration = 100.0f;

    void Update()
    {
        if (IsKeyDown(KEY_SPACE) ||
            IsKeyPressed(KEY_UP) ||
            IsKeyPressed(KEY_W)  ||
            GetTouchPointsCount() == 1 && IsGestureDetected(GESTURE_HOLD | GESTURE_DRAG))
        {
            velocity.y += 200 * GetFrameTime();
            velocity.x += 10 * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN) ||
            IsKeyDown(KEY_S)    ||
            GetTouchPointsCount() == 2)
        {
            velocity.y += 200 * GetFrameTime();
        }

#ifndef NDEBUG
        if (IsKeyPressed(KEY_KP_ADD) ||
            IsKeyPressed(KEY_RIGHT)  ||
            IsKeyPressed(KEY_D))
        {
            velocity.x += 20.f;
        }
        if (GetTouchPointsCount() == 2 && GetTouchX() > GetScreenWidth() / 2)
        {
            velocity.x += 20.f * GetFrameTime();
        }
        if (IsKeyPressed(KEY_KP_SUBTRACT) ||
            IsKeyPressed(KEY_LEFT)        ||
            IsKeyPressed(KEY_A))
        {
            velocity.x -= 20.f;
        }
        if (GetTouchPointsCount() == 2 && GetTouchX() < GetScreenWidth() / 2)
        {
            velocity.x -= 20.f * GetFrameTime();
        }
#endif
        velocity.y += acceleration * GetFrameTime();

        position = Vector2Add(position, Vector2Multiply(velocity, {GetFrameTime(), GetFrameTime()}));
    }
};

struct Transform2D
{
    Vector2 position = Vector2{0.f, 0.f};
    Vector2 scale = Vector2{0.f, 0.f};
};

struct GameConfig
{
    float targetHeight = 0;
    float highScore = 0;
    float currentScore = 0;
};

struct TerrainShader
{
    Shader shader;
    int pointListLoc;
    int offsetLoc;
    int resolutionLoc;
    int upColorLoc;
    int downColorLoc;

    TerrainShader(const char *fsFileName, Color upColor, Color downColor)
    {
        shader = LoadShader(0, fsFileName);
        pointListLoc = GetShaderLocation(shader, "pointList");
        offsetLoc = GetShaderLocation(shader, "offset");
        resolutionLoc = GetShaderLocation(shader, "resolution");
        upColorLoc = GetShaderLocation(shader, "upColor");
        downColorLoc = GetShaderLocation(shader, "downColor");

        Vector4 upColorNorm = ColorNormalize(upColor);
        Vector4 downColorNorm = ColorNormalize(downColor);
        SetShaderValue(shader, upColorLoc, &upColorNorm, UNIFORM_VEC4);
        SetShaderValue(shader, downColorLoc, &downColorNorm, UNIFORM_VEC4);
    }
};