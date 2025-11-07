#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <lps/lps.hpp>

#define forceinline inline __attribute__((always_inline))
#define unreachable() (__builtin_unreachable())

namespace Clockwork {

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
using u128 = unsigned __int128;
#pragma GCC diagnostic pop

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
using i128 = __int128;
#pragma GCC diagnostic pop

using Value   = i32;
using Depth   = i32;
using HashKey = u64;

using isize = std::intptr_t;
using usize = std::size_t;
static_assert(sizeof(isize) == sizeof(usize));

using f32 = float;
using f64 = double;

// Import vector types (u8x64, u64x8, u16x64, ...)
using namespace lps::prelude;

namespace time {
using Clock        = std::chrono::steady_clock;
using TimePoint    = std::chrono::time_point<Clock>;
using Duration     = TimePoint::duration;
using FloatSeconds = std::chrono::duration<f64>;
using Milliseconds = std::chrono::duration<i64, std::milli>;

template<typename T>
constexpr T cast(const auto& x) {
    return std::chrono::duration_cast<T>(x);
}

constexpr u64 nps(u64 nodes, const auto& elapsed) {
    return static_cast<u64>(static_cast<f64>(nodes) / cast<FloatSeconds>(elapsed).count());
}
}  // namespace time
}  // namespace Clockwork
