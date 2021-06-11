#include "MapObject.h"

MapObject::MapObject(int y_, int x_) : y(y_), x(x_)
{
}

MapObject::~MapObject()
{
}

int MapObject::xPos()
{
    return x;
}

int MapObject::yPos()
{
    return y;
}

void MapObject::setPos(int y_, int x_)
{
    y = y_;
    x = x_;
}