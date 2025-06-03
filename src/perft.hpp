#pragma once

#include "position.hpp"
#include "util/types.hpp"

namespace Clockwork {

u64 perft(const Position& position, usize depth);

void splitPerft(const Position& position, usize depth);

}
