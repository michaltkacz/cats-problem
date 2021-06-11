#include "Cat.h"
#include <iostream>

int Cat::ID_COUNTER = 0;

Cat::Cat(int eatTime_, int sleepTime_, std::vector<Bowl *> &bowls_, std::vector<Bed *> &beds_, int yWaitEat_, int xWaitEat_, int yWaitSleep_, int xWaitSleep_)
    : MapObject(yWaitEat_, xWaitEat_),
      id(Cat::ID_COUNTER++),
      eatTime(eatTime_),
      sleepTime(sleepTime_),
      bowls(bowls_), beds(beds_),
      keepRunning(true),
      stats({"WaitEat", 0, 0, 0, 0}),
      yWaitEat(yWaitEat_),
      xWaitEat(xWaitEat_),
      yWaitSleep(yWaitSleep_),
      xWaitSleep(xWaitSleep_),
      thisThread(&Cat::run, this)
{
}

Cat::~Cat()
{
    std::cout << "Cat " << id << " joining..." << std::endl;
    keepRunning = false;
    thisThread.join();
    std::cout << "Cat " << id << " joined" << std::endl;
}

void Cat::run()
{
    while (keepRunning)
    {
        // spobuj znalezc wolna miske
        // znaleziono:
        // 	tak --> zjedz,a potem idz spac
        //  nie --> probuj do skutku
        bool lockSuccess = false;
        while (!lockSuccess)
        {
            for (auto &b : bowls)
            {
                if (b->mutex.try_lock())
                {
                    if (!b->isBowlEmpty())
                    {
                        setPos(b->yPos() + 1, b->xPos());
                        b->eatFromBowl();
                        eat();
                        lockSuccess = true;
                    }
                    b->mutex.unlock();
                    break;
                }
            }
            if (!lockSuccess)
            {
                setPos(yWaitEat, xWaitEat);
                eatWait();
            }
            if (!keepRunning)
            {
                return;
            }
        }

        // spobuj znalezc wolne lozko
        // znaleziono:
        // 	tak --> spij, a potem idz zjesc
        //  nie --> probuj do skutku
        lockSuccess = false;
        while (!lockSuccess)
        {
            for (auto &b : beds)
            {
                if (b->mutex.try_lock())
                {
                    setPos(b->yPos() - 1, b->xPos());
                    sleep();
                    lockSuccess = true;
                    b->mutex.unlock();
                    break;
                }
            }
            if (!lockSuccess)
            {
                setPos(yWaitSleep, xWaitSleep);
                sleepWait();
            }
            if (!keepRunning)
            {
                return;
            }
        }
    }
}

void Cat::eat()
{
    stats.currentState = "Eat";
    stats.eats++;
    thread_local std::uniform_real_distribution<> wait(-0.25, 0.25);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(eatTime + int(wait(rng)) * eatTime));
}

void Cat::sleep()
{
    stats.currentState = "Sleep";
    stats.sleeps++;
    thread_local std::uniform_real_distribution<> wait(-0.25, 0.25);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(sleepTime + int(wait(rng) * sleepTime)));
}

void Cat::eatWait()
{
    stats.currentState = "EatWait";
    stats.eatWaits++;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Cat::sleepWait()
{
    stats.currentState = "SleepWait";
    stats.sleepWaits++;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int Cat::getId()
{
    return id;
}

CatStats Cat::currentStats()
{
    return stats;
}

DrawInfo Cat::drawInfo()
{
    return DrawInfo{y, x, "%2d", id};
}
