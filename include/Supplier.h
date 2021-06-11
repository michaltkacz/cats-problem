#pragma once

#include <vector>
#include <random>
#include <thread>
#include <atomic>

#include "MapObject.h"
#include "Bowl.h"

class Supplier : public MapObject
{
private:
    std::vector<Bowl *> &bowls;

    std::thread thisThread;
    std::atomic_bool keepRunning;

    std::mt19937 rng{std::random_device{}()};

public:
    Supplier(std::vector<Bowl *> &bowls_);
    ~Supplier();

    void run();

    DrawInfo drawInfo() override;
};