#pragma once

#include <iostream>
#include <cstdlib>
#include <random>

namespace utils
{

static std::random_device rd;
static std::mt19937 gen(rd());

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
    static std::uniform_real_distribution<float> dis;
    dis.param(std::uniform_real_distribution<float>::param_type(min, max));
    return dis(gen);
}

static int GetRandomInt(const float &min,
                        const float &max)
{
    static std::uniform_int_distribution<int> dis;
    dis.param(std::uniform_int_distribution<int>::param_type(min, max));
    return dis(gen);
}

static void SetRandomSeed(const unsigned int &seed) {
    gen.seed(seed); // Set custom seed
    std::cout << "Random seed set to: " << seed << std::endl;
}

}
