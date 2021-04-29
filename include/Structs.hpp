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
        if (IsKeyDown(KEY_SPACE))
        {
            velocity.y = -100;
        }
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
    float lerpSpeed = 0.1;
    float targetHeight = 0;
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