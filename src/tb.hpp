#pragma once

#include "position.hpp"
#include "repetition_info.hpp"
#include "root_move.hpp"
#include "util/types.hpp"
#include <span>
#include <string_view>
#include <utility>

namespace Clockwork::tb {

enum class InitStatus {
    Failed,
    NoneFound,
    Success,
};

InitStatus init(std::string_view path);
void       free();

[[nodiscard]] u32 dtz_count();
[[nodiscard]] u32 wdl_count();

[[nodiscard]] u32 max_pieces();

// Returns whether the DTZ probe succeeded.
[[nodiscard]] bool probe_root(const Position&             pos,
                              RepetitionInfo&             repetition_info,
                              std::span<Search::RootMove> root_moves);

[[nodiscard]] Search::WDL probe_wdl(const Position& pos);

}
