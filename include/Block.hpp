#pragma once
#include <vector>

#define FUNCTION_IMPLEM
#include "Functions.hpp"

#define BLOCK_LIST_SIZE 10
#define BLOCK_MIN_WIDTH 50
#define BLOCK_MAX_WIDTH 100
#define BLOCK_MIN_HEIGHT 10
#define BLOCK_MAX_HEIGHT 100
#define BLOCK_POINTS_NUM 10

#define FUNCTION_COUNT 4

typedef float(MathFunction)(float, bool);

struct Vector2;
Vector2 GetRandomVector();

class Block
{
private:

    Vector2 startPos;
    Vector2 extent;
    Vector2 pointList[BLOCK_POINTS_NUM];
    bool descending;

public:

    Block(Vector2 startPos, Vector2 extent, MathFunction* func, bool descending);
    std::vector<Vector2> GetPointList();
    Vector2 GetLastPoint();
    Vector2& GetStartPoint();
    void SetStartPoint(Vector2 startPoint);
    float GetWidth();
};

class BlockList
{
private:
    std::vector<Block> blockVec;
    bool descending = false;
    MathFunction* functions[FUNCTION_COUNT] = {
            sinMapped,
            tanhMapped,
            ellipticMapped,
            polynomialMapped,
    };

    MathFunction* GetRandomFunction();
    bool GetDirection();
public:
    BlockList(int blockCounts);

    //TODO: Block function
//    void Shift();
    std::vector<Vector2> GetPointList();
    float GetFirstBlockWidth();
    Vector2& GetFirstPoint();
    void Shift();
    void ResetOffset();

    enum Profile
    {
        SIN,
        TAN,
        ELLIPTIC,
        POLYNOMIAL,
        RANDOM,
    };

    Profile profile{RANDOM};
};