#pragma once

#include <iostream>
#include <cstdlib>

static void RUNNR_ASSERT(const bool &condition,
                         const char *msg) {
    if (!condition) {
        std::cerr << "Assertion failed: " << msg;
        std::abort();
    }
}
