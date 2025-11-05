#pragma once

#include "position.hpp"
#include "util/types.hpp"

namespace Clockwork {

u64 perft(const Position& position, usize depth);

void split_perft(const Position& position, usize depth);

}  // namespace Clockwork
