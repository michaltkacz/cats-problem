#pragma once
#include <mutex>

#include "MapObject.h"

class Bowl : public MapObject
{
private:
    int id;
    int stock;

public:
    static int ID_COUNTER;

    Bowl(int y_, int x_);
    ~Bowl();

    std::mutex mutex;

    int getId();

    bool isBowlEmpty();
    void fillBowl(int amount);
    void eatFromBowl();

    DrawInfo drawInfo() override;
};