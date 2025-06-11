#include "common.hpp"
#include "util/types.hpp"
#include <array>

namespace Clockwork {
class RepetitionInfo {
public:
    RepetitionInfo();
    void push(HashKey key, bool is_reversible);
    void pop();
    void reset();

    bool detect_repetition(size_t root_ply);

private:
    std::array<std::pair<HashKey, bool>, MAX_PLY * 4> m_repetition_table;
    size_t                                            m_index;
};
}
