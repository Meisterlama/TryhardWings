#include <raymath.h>
#include "Block.hpp"

Block::Block(Vector2 startPos, Vector2 extent, float (*func)(float, bool), bool ascending)
: startPos(startPos)
, extent(extent)
, ascending(ascending)
{
    for (int i = 0; i < BLOCK_POINTS_NUM; i++)
    {
        float step = static_cast<float>(i) / BLOCK_POINTS_NUM;
        Vector2 newPoint = Vector2 {
                step,
                func(step, ascending),
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

Vector2& Block::GetStartPoint()
{
    return startPos;
}

float Block::GetWidth()
{
    return extent.x;
}

void Block::SetStartPoint(Vector2 startPoint)
{
    startPos = startPoint;
}

Vector2 GetRandomVector()
{
    return Vector2{(float)GetRandomValue(BLOCK_MIN_WIDTH, BLOCK_MAX_WIDTH),
                   (float)GetRandomValue(BLOCK_MIN_HEIGHT, BLOCK_MAX_HEIGHT)};
}

float sinMapped(float x, bool ascending)
{
    float value = 0.5f * sinf(x * PI - PI/2) + 0.5f;
    return (ascending) ? value : -value;
}

float tanhMapped(float x, bool ascending)
{
    float expo = exp(-5.f * (2.f * x - 1.f));
    float value = (((1.f - expo)/(1.f + expo)) + 1.f)/2.f;
    return (ascending) ? value : -value;
}

BlockList::BlockList(int blockCounts)
{
    blockVec.reserve(blockCounts);
    for (int i = 0; i < blockVec.capacity(); i++)
    {
        if (i == 0)
        {
            blockVec.emplace_back(Vector2{0, 0}, GetRandomVector(), tanhMapped, true);
        }
        else
        {
            blockVec.emplace_back(blockVec[i-1].GetLastPoint(), GetRandomVector(), tanhMapped, (i%2));
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

float BlockList::GetFirstBlockWidth()
{
    return blockVec[0].GetWidth();// + blockVec[0].GetStartPoint().x;
}

void BlockList::Shift()
{
    blockVec.erase(blockVec.cbegin());
    blockVec.emplace_back(blockVec.back().GetLastPoint(), GetRandomVector(), tanhMapped, GetRandomValue(0, 1));
}

Vector2& BlockList::GetFirstPoint()
{
    return blockVec[0].GetStartPoint();
}

void BlockList::ResetOffset()
{
    for (int i = 0; i < blockVec.size(); i++)
    {
        if (i == 0)
        {
            blockVec[i].GetStartPoint().x = 0;
        }
        else
        {
            blockVec[i].GetStartPoint().x = blockVec[i-1].GetLastPoint().x;
        }
    }
}
