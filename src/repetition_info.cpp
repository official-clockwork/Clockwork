#include "repetition_info.hpp"
#include <cstddef>

namespace Clockwork {

void RepetitionInfo::push(HashKey key, bool is_reversible) {
    m_repetition_table.emplace_back(key, is_reversible);
}

void RepetitionInfo::pop() {
    m_repetition_table.pop_back();
}

bool RepetitionInfo::detect_repetition(size_t root_ply) {
    // Short circuit return if move is not reversible
    if (!m_repetition_table.back().second) {
        return false;
    }

    // The hash we will be looking for
    HashKey query  = m_repetition_table.back().first;
    size_t  height = m_repetition_table.size() - 1;

    // Count how many times we encounter the hash
    i32 counter = 0;

    // Start from two moves ago
    for (size_t idx = 4; idx <= height; idx += 2) {
        // Do the hashes match?
        if (m_repetition_table[height - idx].first == query) {

            // If the match happened inside the search tree, twofold repetition check
            if (idx <= root_ply) {
                return true;
            }

            // If the match is outside the search tree, threefold repetition check
            if (++counter >= 2) {
                return true;
            }

            // Shortcircuit (only after checking the hash) if the move is nonreversible
            if (!m_repetition_table[height - idx].second) {
                return false;
            }
        }
    }

    // No matches found
    return false;
}

void RepetitionInfo::reset() {
    m_repetition_table.clear();
}

}  // namespace Clockwork
