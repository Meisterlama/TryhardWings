#include <raymath.h>
#include "Block.hpp"

Block::Block(Vector2 startPos, Vector2 extent, float (*func)(float), bool ascending)
: startPos(startPos)
, extent(extent)
, ascending(ascending)
{
    for (int i = 0; i < BLOCK_POINTS_NUM; i++)
    {
        float step = static_cast<float>(i) / BLOCK_POINTS_NUM;
        Vector2 newPoint = Vector2 {
                step,
                (ascending) ? func(step) : -func(step),
                };
        pointList[i] = newPoint;
    }
}

std::vector<Vector2> Block::GetPointList()
{
    std::vector<Vector2> points;
    points.reserve(BLOCK_POINTS_NUM);
    for (int i = 0; i < BLOCK_POINTS_NUM; i++)
    {
        Vector2 scaledRelPos = Vector2Multiply(pointList[i], extent);
        Vector2 pos = Vector2Add(scaledRelPos, startPos);
        points.push_back(pos);
    }
    return points;
}

Vector2 Block::GetLastPoint()
{
    Vector2 scaledRelPos = Vector2Multiply(pointList[BLOCK_POINTS_NUM - 1], extent);
    Vector2 pos = Vector2Add(scaledRelPos, startPos);

    return pos;
}

float sinMapped(float x)
{
    return sinf(x * PI);
}

BlockList::BlockList(int blockCounts)
{
    blockVec.reserve(blockCounts);
    for (int i = 0; i < blockVec.capacity(); i++)
    {
        if (i == 0)
        {
            blockVec.emplace_back(Vector2{0, 0}, Vector2{15, 15}, sinMapped, true);
        }
        else
        {
            blockVec.emplace_back(blockVec[i-1].GetLastPoint(), Vector2{15,100}, sinMapped, (i%2));
        }
    }
}

std::vector<Vector2> BlockList::GetPointList()
{
    std::vector<Vector2> points;
    for (auto & block : blockVec)
    {
        for (auto point : block.GetPointList())
        {
            points.push_back(point);
        }
    }
    return points;
}