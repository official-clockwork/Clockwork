#pragma once

#include "util/types.hpp"
#include <bit>
#include <cassert>

namespace Clockwork {

[[nodiscard]] constexpr i64 log2i(i32 x) {
    assert(x > 0);

    int head = std::countl_zero(static_cast<u32>(x));
    u32 rem  = static_cast<u32>(x) << (head + 1) >> 22;

    u32 big   = static_cast<u32>(31 - head) * 1024;
    u32 small = rem + (355 * rem * (1024 - rem)) / (1024 * 1024);

    return static_cast<i64>(big + small);
}

}  // namespace Clockwork
