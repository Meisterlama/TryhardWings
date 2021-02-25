#include <raymath.h>
#include "Block.hpp"

float Block::operator()(float x)
{
    return 0;
}
Vector2 Block::GetStart()
{
    return pos;
}
Vector2 Block::GetEnd()
{
    if (next == nullptr)
        return Vector2{(float) GetScreenWidth(), (float) GetScreenHeight() / 2};
    return next->pos;
}
void Block::Draw(Vector2 offset)
{
    DrawLineV(Vector2Add(GetStart(), offset), Vector2Add(Vector2Add({GetStart().x, 0}, GetEnd()), offset), DARKBROWN);
}

BlockList::BlockList()
{
    BlockStep = (float) GetScreenWidth() / (BLOCK_LIST_SIZE - 3);
    for (int i = 0; i < BLOCK_LIST_SIZE; i++) {
        if (i < BLOCK_LIST_SIZE - 1)
            blockList[i].next = &blockList[i + 1];
        blockList[i].pos = {BlockStep, (float) GetRandomValue(-200, 200)};
    }
    head = &blockList[0];
}

void BlockList::Shift()
{
    Block *cur = head;
    while (cur->next != nullptr)
        cur = cur->next;
    head->pos = {BlockStep, (float) GetRandomValue(-20, 20)};
    cur->next = head;
    Block *tmp = head->next;
    head->next = nullptr;
    head = tmp;
}

void BlockList::Draw(Vector2 offset = {0, 0})
{
    Block *cur = head;
    Vector2 curOffset{-BlockStep, 0};
    while (cur != nullptr)
    {
        cur->Draw(Vector2Add(curOffset, offset));
        curOffset = Vector2Add(curOffset, {BlockStep, 0});
        cur = cur->next;
    }
}

