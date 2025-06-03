#pragma once

#include "util/types.hpp"

namespace Clockwork {

template<typename T>
[[nodiscard]] constexpr T clear_lowest_bit(T x) {
    return x & (x - 1);
}

}
