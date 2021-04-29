#include "Utils.hpp"
#include "raymath.h"
#include "Application.hpp"

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

std::vector<Vector2>
GetShaderReadyPoints(const Transform2D &transform, const std::vector<Vector2> &virtualPoints)
{
    std::vector<Vector2> real_points;
    real_points.reserve(SHADER_ARRAY_SIZE);
    for (int i = 0; i < SHADER_ARRAY_SIZE; i++)
    {
        float step = (float) i;
        float x = transform.position.x + step * transform.scale.x;
        Vector2 point = {
                x,
                GetPointFromFunction(x, virtualPoints) * transform.scale.y + transform.position.y,
        };
        real_points.push_back(point);
    }
    return real_points;
}

void DrawDebugPoints(const std::vector<Vector2> &points, Color color, Vector2 position, Vector2 scale)
{
    for (int i = 0; i < points.size() - 1; i++)
    {
        Vector2 scaledPoint1 = Vector2Multiply(points[i], scale);
        Vector2 scaledPoint2 = Vector2Multiply(points[i+1], scale);
        DrawLineV(Vector2Add(position, scaledPoint1) , Vector2Add(position, scaledPoint2), color);
    }
}