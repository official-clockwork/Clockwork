#pragma once

#include "types.hpp"

namespace Clockwork {
class Random {
public:
    Random() = delete;
    static u64 state;

    static u64 rand_64() {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        return state;
    }
};

// Define static member outside the class
inline u64 Random::state = 9049990082738480776ULL;
}  // namespace Clockwork
