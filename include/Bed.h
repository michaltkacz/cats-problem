#pragma once

#include <mutex>

#include "MapObject.h"

class Bed : public MapObject
{
private:
    int id;

public:
    static int ID_COUNTER;

    Bed(int y_, int x_);
    ~Bed();

    std::mutex mutex;

    int getId();
    DrawInfo drawInfo() override;
};
