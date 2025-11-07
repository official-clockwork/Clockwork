#include "util/types.hpp"
#include <charconv>
#include <optional>
#include <string_view>
#include <system_error>
#include <type_traits>

namespace Clockwork {

template<typename T>
inline std::optional<T> parse_number(std::string_view s) {
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);

    T value{};
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

}  // namespace Clockwork
