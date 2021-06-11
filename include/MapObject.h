#pragma once

#include <string>

struct DrawInfo
{
    int y;
    int x;
    std::string str;
    int data;
};

class MapObject
{
protected:
    int y;
    int x;

public:
    MapObject(int y_, int x_);
    ~MapObject();

    int xPos();
    int yPos();

    void setPos(int y_, int x_);

    virtual DrawInfo drawInfo() = 0;
};