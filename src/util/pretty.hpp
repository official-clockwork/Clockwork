#pragma once

#include "types.hpp"

namespace Clockwork {

inline void print_progress(size_t current, size_t total, size_t bar_width = 40) {
    if (total == 0) {
        std::cout << "\r[error: total=0]" << std::flush;
        return;
    }

    f32 progress = static_cast<f32>(current) / static_cast<f32>(total);
    if (progress > 1.0f) {
        progress = 1.0f;  // clamp if overshoot
    }

    size_t pos = static_cast<size_t>(bar_width * progress);

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

    std::cout << "] " << static_cast<int>(progress * 100.0f) << "% (" << current << "/" << total
              << ")" << std::flush;
}

}  // namespace Clockwork
