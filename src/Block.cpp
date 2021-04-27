#include <raymath.h>
#include "Block.hpp"

Block::Block(Vector2 startPos, float (*func)(float)) : startPos(startPos)
{
    for (int i = 0; i < BLOCK_POINTS_NUM; i++)
    {
        float step = static_cast<float>(i) / BLOCK_POINTS_NUM;
        Vector2 newPoint = Vector2 {
                step * BLOCK_MAX_WIDTH,
                func(step) * BLOCK_MAX_HEIGHT,
                };
        pointList[i] = Vector2Add(startPos, newPoint);
    }
}

//float *Block::GetPointList()
//{
//    return pointList;
//}
