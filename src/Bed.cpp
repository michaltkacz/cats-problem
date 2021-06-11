#include "Bed.h"

int Bed::ID_COUNTER = 0;

Bed::Bed(int y_, int x_)
    : MapObject(y_, x_),
      id(Bed::ID_COUNTER++)
{
}

Bed::~Bed()
{
}

int Bed::getId()
{
    return id;
}

DrawInfo Bed::drawInfo()
{
    return DrawInfo{y, x, "  ", 0};
}