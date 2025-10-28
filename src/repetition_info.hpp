#pragma once

#include "common.hpp"
#include "util/types.hpp"
#include <utility>
#include <vector>

namespace Clockwork {

class RepetitionInfo {
public:
    RepetitionInfo() = default;

    void push(HashKey key, bool is_reversible);
    void pop();
    void reset();

    bool detect_repetition(usize root_ply);

private:
    std::vector<std::pair<HashKey, bool>> m_repetition_table;
};

}
