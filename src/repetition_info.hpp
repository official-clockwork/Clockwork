#include <utility>
#include <vector>

#include "common.hpp"
#include "util/types.hpp"

namespace Clockwork {

class RepetitionInfo {
public:
    RepetitionInfo() = default;

    void push(HashKey key, bool is_reversible);
    void pop();
    void reset();

    bool detect_repetition(size_t root_ply);

private:
    std::vector<std::pair<HashKey, bool>> m_repetition_table;
};

}
