#define BLOCK_LIST_SIZE 1000

struct Vector2;
class BlockList;
class Block
{
    friend BlockList;
    Block *next = nullptr;
    Vector2 pos = {0, 0};

    Block(Vector2 _pos = {0, 0}) : pos(_pos) {};

    virtual float operator()(float x);
    Vector2 GetStart();
    Vector2 GetEnd();
    virtual void Draw(Vector2 offset);
};

class BlockList
{
public:
    Block blockList[BLOCK_LIST_SIZE]{};
    Block* head = nullptr;
    float BlockStep;

    BlockList();

    void Shift();
    void Draw(Vector2 offset);
};
