#pragma once

#include "position.hpp"

namespace Clockwork {

using MainHistory = std::array<std::array<i32, 4096>, 2>;

constexpr i32 HISTORY_MAX = 16384;

class History {
public:
    History() = default;

    i32 get_quiet_stats(const Position& pos, Move move) const {
        return m_main_hist[static_cast<usize>(pos.active_color())][move.from_to()];
    }

    void update_quiet_stats(const Position& pos, Move move, i32 bonus) {
        update_hist_entry(m_main_hist[static_cast<usize>(pos.active_color())][move.from_to()],
                          bonus);
    }

    void clear() {
        std::memset(&m_main_hist[0], 0, sizeof(MainHistory));
    }

private:
    static void update_hist_entry(i32& entry, i32 bonus) {
        entry += bonus - entry * std::abs(bonus) / HISTORY_MAX;
    }

    MainHistory m_main_hist;
};

}
