#pragma once

#include <string>
#include <vector>
#include <random>
#include <thread>
#include <atomic>

#include "MapObject.h"
#include "Bowl.h"
#include "Bed.h"

struct CatStats
{
    std::string currentState;
    int eats;
    int sleeps;
    int eatWaits;
    int sleepWaits;
};

class Cat : public MapObject
{
private:
    int id;
    int eatTime;
    int sleepTime;
    std::vector<Bowl *> &bowls;
    std::vector<Bed *> &beds;

    CatStats stats;

    int xWaitEat;
    int yWaitEat;
    int xWaitSleep;
    int yWaitSleep;

    void eat();
    void sleep();

    void eatWait();
    void sleepWait();

public:
    static int ID_COUNTER;

    Cat(int eatTime_, int sleepTime_, std::vector<Bowl *> &bowls_, std::vector<Bed *> &beds_, int yWaitEat_, int xWaitEat_, int yWaitSleep_, int xWaitSleep_);
    ~Cat();

    std::mt19937 rng{std::random_device{}()};

    std::thread thisThread;
    std::atomic_bool keepRunning;

    void run();

    int getId();
    CatStats currentStats();

    DrawInfo drawInfo() override;
};
