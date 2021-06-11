#include "Supplier.h"
#include <iostream>

Supplier::Supplier(std::vector<Bowl *> &bowls_)
    : MapObject(0, 0),
      bowls(bowls_),
      keepRunning(true),
      thisThread(&Supplier::run, this)
{
}

Supplier::~Supplier()
{
    std::cout << "Supplier joining..." << std::endl;
    keepRunning = false;
    thisThread.join();
    std::cout << "Supplier joined" << std::endl;
}

void Supplier::run()
{
    while (keepRunning)
    {
        setPos(0, 0);
        for (auto &b : bowls)
        {
            if (b->mutex.try_lock())
            {
                if (b->isBowlEmpty())
                {
                    setPos(b->yPos() + 1, b->xPos());
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    thread_local std::uniform_int_distribution<> value(1, 11);
                    b->fillBowl(value(rng));
                }
                b->mutex.unlock();
            }
        }
    }
}

DrawInfo Supplier::drawInfo()
{
    return DrawInfo{y, x, "  ", 0};
}
