#pragma once

#include <cstdint>
#include <cstddef>

#define forceinline inline __attribute__((always_inline))

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

using isize = std::intptr_t;
using usize = std::size_t;
static_assert(sizeof(isize) == sizeof(usize));

using f32 = float;
using f64 = double;

}
