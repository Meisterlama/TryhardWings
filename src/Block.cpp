#include <raymath.h>
#include "Block.hpp"

Block::Block(Vector2 startPos, Vector2 extent, MathFunction* func, bool descending)
: startPos(startPos)
, extent(extent)
, descending(descending)
, func(func)
{
    for (int i = 0; i < BLOCK_POINTS_NUM; i++)
    {
        float step = static_cast<float>(i) / BLOCK_POINTS_NUM;
        Vector2 newPoint = { step, func(step, descending, false)};
        Vector2 newPointDerivative = { step, func(step, descending, true)};
        pointList[i] = newPoint;
        pointDerivativeList[i] = newPointDerivative;
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

std::vector<Vector2> Block::GetPointDerivativeList()
{
    std::vector<Vector2> points;
    points.reserve(BLOCK_POINTS_NUM);
    for (int i = 0; i < BLOCK_POINTS_NUM; i++)
    {
        Vector2 scaledRelPos = Vector2Multiply(pointDerivativeList[i], extent);
        Vector2 pos = Vector2Add(scaledRelPos, startPos);
        points.push_back(pos);
    }
    return points;
}

float Block::GetDerivativeAt(float x)
{
    return func(x / extent.x, descending, true);
}

Vector2 GetRandomVector()
{
    return Vector2{(float)GetRandomValue(BLOCK_MIN_WIDTH, BLOCK_MAX_WIDTH),
                   (float)GetRandomValue(BLOCK_MIN_HEIGHT, BLOCK_MAX_HEIGHT)};
//    return Vector2{50.f, 50.f};
}

BlockList::BlockList(int blockCounts)
{
    blockVec.reserve(blockCounts);
    for (int i = 0; i < blockVec.capacity(); i++)
    {
        if (i == 0)
        {
            blockVec.emplace_back(Vector2{0, 0},
                                  GetRandomVector(),
                                  GetRandomFunction(),
                                  GetDirection());
        }
        else
        {
            blockVec.emplace_back(blockVec[i-1].GetLastPoint(),
                                  GetRandomVector(),
                                  GetRandomFunction(),
                                  GetDirection());
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

std::vector<Vector2> BlockList::GetPointDerivativeList()
{
    std::vector<Vector2> points;
    for (auto & block : blockVec)
    {
        for (auto point : block.GetPointDerivativeList())
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
    blockVec.emplace_back(blockVec.back().GetLastPoint(),
                          GetRandomVector(),
                          GetRandomFunction(),
                          GetDirection());
}

Vector2& BlockList::GetFirstPoint(int blockIndex)
{
    return blockVec[blockIndex].GetStartPoint();
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

MathFunction *BlockList::GetRandomFunction()
{
    MathFunction *function;
    int index = 0;
    switch (profile)
    {
        case RANDOM:
            index = GetRandomValue(0, FUNCTION_COUNT - 1);
            break;
        default:
            index = profile;
            break;
    }
    return functions[index];
}

bool BlockList::GetDirection()
{
    descending = GetRandomValue(0, 10) < 7;
    return descending;
}

float BlockList::GetDerivativeAt(float x)
{
    return (x< blockVec[0].GetWidth()) ? blockVec[0].GetDerivativeAt(x) : blockVec[1].GetDerivativeAt(x - blockVec[0].GetWidth());
}
