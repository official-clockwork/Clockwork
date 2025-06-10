#include "repetition_info.hpp"
#include <cstddef>

namespace Clockwork {

RepetitionInfo::RepetitionInfo() :
    m_index(0) {
    m_repetition_table.fill(std::make_pair(0, false));
}

void RepetitionInfo::push(HashKey key, bool is_reversible) {
    // TODO: we may want to handle overflow here
    m_repetition_table[m_index++] = std::make_pair(key, is_reversible);
}

void RepetitionInfo::pop() {
    --m_index;
}

bool RepetitionInfo::detect_repetition(size_t root_ply) {
    // Short circuit return if move is not reversible
    if (!m_repetition_table[m_index - 1].second) {
        return false;
    }
    // The hash we will be looking for
    HashKey query = m_repetition_table[m_index - 1].first;

    // Count how many times we encounter the hash
    i32 counter = 0;

    // Start from two moves ago
    for (size_t idx = 4; idx < m_index; idx += 2) {
        // Do the hashes match?
        if (m_repetition_table[m_index - 1 - idx].first == query) {

            // If the match happened inside the search tree, twofold repetition check
            if (idx <= root_ply) {
                return true;
            }

            // If the match is outside the search tree, threefold repetition check
            if (++counter >= 2) {
                return true;
            }

            // Shortcircuit (only after checking the hash) if the move is nonreversible
            if (!m_repetition_table[m_index - 1 - idx].second) {
                return false;
            }
        }
    }

    // No matches found
    return false;
}

void RepetitionInfo::reset() {
    m_index = 0;
}

}  // namespace Clockwork
