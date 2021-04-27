#include <vector>
#define BLOCK_LIST_SIZE 10
#define BLOCK_MIN_WIDTH 5
#define BLOCK_MAX_WIDTH 15
#define BLOCK_MIN_HEIGHT 5
#define BLOCK_MAX_HEIGHT 15
#define BLOCK_POINTS_NUM 10

struct Vector2;
class Block
{
private:

    Vector2 startPos;
    Vector2 pointList[BLOCK_POINTS_NUM];

public:

    Block(Vector2 startPos, float (*func)(float));
    float *GetPointList();
};

class BlockList
{
private:
    std::vector<Block> blockVec;
public:
    BlockList();

    //TODO: Block function
//    void Shift();
//    std::vector<float> GetPointList();

};