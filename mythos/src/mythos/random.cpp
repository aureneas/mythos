#include <random>
#include "../../include/mythos/random.h"

static std::default_random_engine generator;

template <>
int random<int>(int low, int hi) {
    return low + (rand() % (hi - low + 1));
}

template <>
unsigned random<unsigned>(unsigned low, unsigned hi) {
    return low + (rand() % (hi - low + 1));
}
