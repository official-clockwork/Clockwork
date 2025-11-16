#pragma once

#include "util/types.hpp"

namespace Clockwork {

template<typename T>
[[nodiscard]] constexpr T clear_lowest_bit(T x) {
    return x & (x - 1);
}

template<typename T>
[[nodiscard]] constexpr T lowest_bit(T x) {
    return x & -x;
}

[[nodiscard]] constexpr i32 sign(i32 x) {
    if (x == 0) {
        return 0;
    }
    if (x < 0) {
        return -1;
    }
    return 1;
}

}  // namespace Clockwork
