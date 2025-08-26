#pragma once
#include <array>

#include "position.hpp"

#include "eval_types.hpp"

namespace Clockwork {

Score evaluate_white_pov(const Position& pos);
Score evaluate_stm_pov(const Position& pos);

};
