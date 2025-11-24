#include "repetition_info.hpp"
#include "bitboard.hpp"
#include "cuckoo.hpp"
#include "position.hpp"
#include "rays.hpp"
#include "util/types.hpp"

namespace Clockwork {

void RepetitionInfo::push(HashKey key, bool is_reversible) {
    m_repetition_table.emplace_back(key, is_reversible);
}

void RepetitionInfo::pop() {
    m_repetition_table.pop_back();
}

bool RepetitionInfo::detect_repetition(usize root_ply) {
    // Short circuit return if move is not reversible
    if (!m_repetition_table.back().second) {
        return false;
    }

    // The hash we will be looking for
    HashKey query  = m_repetition_table.back().first;
    usize   height = m_repetition_table.size() - 1;

    // Count how many times we encounter the hash
    i32 counter = 0;

    // Start from two moves ago
    for (usize idx = 4; idx <= height; idx += 2) {
        // Do the hashes match?
        if (m_repetition_table[height - idx].first == query) {

            // If the match happened inside the search tree, twofold repetition check
            if (idx <= static_cast<usize>(root_ply)) {
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

bool RepetitionInfo::has_game_cycle(const Position& pos, usize ply) {
    usize end = std::min(pos.get_50mr_counter(), static_cast<u16>(m_repetition_table.size() - 1));

    if (end < 3) {
        return false;
    }

    auto old_key = [this](usize i) -> HashKey {
        return m_repetition_table[m_repetition_table.size() - i].first;
    };

    Bitboard occ          = pos.board().get_occupied_bitboard();
    HashKey  original_key = pos.get_hash_key();

    HashKey other = ~(original_key ^ old_key(2));

    for (usize i = 3; i <= end; i += 2) {
        HashKey curr_key = old_key(i + 1);

        other ^= ~(curr_key ^ old_key(i));
        if (other != 0) {
            continue;
        }

        HashKey diff = original_key ^ curr_key;

        usize slot = Cuckoo::h1(diff);

        if (diff != Cuckoo::keys[slot]) {
            slot = Cuckoo::h2(diff);
        }

        if (diff != Cuckoo::keys[slot]) {
            continue;
        }

        Move mv = Cuckoo::moves[slot];

        if ((occ & rays::exclusive(mv.from(), mv.to())).empty()) {
            // repetition is after root, done:
            if (ply > i) {
                return true;
            }

            auto piece = pos.board()[mv.from()];
            if (piece.is_empty()) {
                piece = pos.board()[mv.to()];
            }

            return piece.color() == pos.active_color();
        }
    }

    return false;
}

}  // namespace Clockwork
