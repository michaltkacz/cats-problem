#include "Bowl.h"

int Bowl::ID_COUNTER = 0;

Bowl::Bowl(int y_, int x_)
    : MapObject(y_, x_),
      id(Bowl::ID_COUNTER++),
      stock(0)
{
}

Bowl::~Bowl()
{
}

int Bowl::getId()
{
    return id;
}

DrawInfo Bowl::drawInfo()
{
    return DrawInfo{y, x, "%2d", stock};
}

bool Bowl::isBowlEmpty()
{
    return stock <= 0;
}

void Bowl::fillBowl(int amount)
{
    stock = amount;
}

void Bowl::eatFromBowl()
{
    stock -= 1;
}