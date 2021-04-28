#pragma once
#include "raylib.h"
#include <vector>
#include "Structs.hpp"
float GetPointFromFunction(float x,
                           const std::vector<Vector2> &points);

std::vector<Vector2>GetShaderReadyPoints(const Transform2D &transform,
                                         const std::vector<Vector2> &virtualPoints);

void DrawDebugPoints(const std::vector<Vector2> &points,
                     Color color,
                     Vector2 position,
                     Vector2 scale);