#pragma once

#include <iostream>
#include <cstdlib>
#include <random>

namespace utils
{

static void RUNNR_ASSERT(const bool &condition,
                         const char *msg)
{
    if (!condition)
    {
        std::cerr << "Assertion failed: " << msg;
        std::abort();
    }
}

static float GetRandomFloat(const float &min,
                            const float &max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

}
