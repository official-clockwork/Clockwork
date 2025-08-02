#pragma once

#include "types.hpp"

namespace Clockwork {

void print_progress(size_t current, size_t total, size_t bar_width = 40) {
    f32    progress = static_cast<float>(current) / total;
    size_t pos      = static_cast<size_t>(bar_width * progress);

    std::cout << "\r[";
    for (size_t i = 0; i < bar_width; ++i) {
        if (i < pos) {
            std::cout << "=";
        } else if (i == pos) {
            std::cout << ">";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "] " << i32(progress * 100.0) << "% (" << current << "/" << total << ")";
    std::cout.flush();
}

}  // namespace Clockwork
