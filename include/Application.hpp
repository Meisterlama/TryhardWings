#pragma once

#include "raylib.h"
#include "Block.hpp"
#include "Structs.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#define SHADER_ARRAY_SIZE 512
#else
#define GLSL_VERSION 100
#define SHADER_ARRAY_SIZE 256
#endif

class Application
{
private:
    BlockList blockList;
    Player player;

    Vector2 screenSize{1000.f, 600.f};
    TerrainShader terrainShader;
    GameConfig gameConfig;
    Transform2D transform;
    RenderTexture2D target;
    RenderTexture2D miniMap;


    bool shouldRefreshTarget = true;
    double lastTimeSinceResize = 0;

public:
    Application(Vector2 screenSize);

    void Update();

    ~Application();
};