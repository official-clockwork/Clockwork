#include "types.hpp"

namespace Clockwork {
class Random {
public:
    Random() = delete;

    static u64 rand_64() {
        m_state ^= m_state << 13;
        m_state ^= m_state >> 7;
        m_state ^= m_state << 17;
        return m_state;
    }

private:
    static u64 m_state;
};

// Define static member outside the class
u64 Random::m_state = 9049990082738480776ULL;
}
