#include "util/types.hpp"
#include <charconv>
#include <optional>
#include <string_view>
#include <system_error>

namespace Clockwork {

inline std::optional<i32> parse_i32(std::string_view s) {
    i32 value = 0;
#if __cpp_lib_to_chars >= 202306L
    if (std::from_chars(s.data(), s.data() + s.size(), value)) {
#else
    if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc{}) {
#endif
        return value;
    } else {
        return std::nullopt;
    }
}

}
