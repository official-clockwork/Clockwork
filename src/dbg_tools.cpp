#include "dbg_tools.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>

namespace {

template<size_t N>
struct DebugInfo {
    std::array<std::atomic<int64_t>, N> data = {0};

    [[nodiscard]] constexpr std::atomic<int64_t>& operator[](size_t index) {
        assert(index < N);
        return data[index];
    }

    constexpr DebugInfo& operator=(const DebugInfo& other) {
        for (size_t i = 0; i < N; i++) {
            data[i].store(other.data[i].load());
        }
        return *this;
    }
};

struct DebugExtremes : public DebugInfo<3> {
    DebugExtremes() {
        data[1] = std::numeric_limits<int64_t>::min();
        data[2] = std::numeric_limits<int64_t>::max();
    }
};

std::array<DebugInfo<2>, MAX_DEBUG_SLOTS>  hit;
std::array<DebugInfo<2>, MAX_DEBUG_SLOTS>  mean;
std::array<DebugInfo<3>, MAX_DEBUG_SLOTS>  stdev;
std::array<DebugInfo<6>, MAX_DEBUG_SLOTS>  correl;
std::array<DebugExtremes, MAX_DEBUG_SLOTS> extremes;

}  // namespace

void dbg_hit_on(bool cond, size_t slot) {

    ++hit.at(slot)[0];
    if (cond) {
        ++hit.at(slot)[1];
    }
}

void dbg_mean_of(int64_t value, size_t slot) {

    ++mean.at(slot)[0];
    mean.at(slot)[1] += value;
}

void dbg_stdev_of(int64_t value, size_t slot) {

    ++stdev.at(slot)[0];
    stdev.at(slot)[1] += value;
    stdev.at(slot)[2] += value * value;
}

void dbg_extremes_of(int64_t value, size_t slot) {
    ++extremes.at(slot)[0];

    int64_t current_max = extremes.at(slot)[1].load();
    while (current_max < value && !extremes.at(slot)[1].compare_exchange_weak(current_max, value)) {
    }

    int64_t current_min = extremes.at(slot)[2].load();
    while (current_min > value && !extremes.at(slot)[2].compare_exchange_weak(current_min, value)) {
    }
}

void dbg_correl_of(int64_t value1, int64_t value2, size_t slot) {

    ++correl.at(slot)[0];
    correl.at(slot)[1] += value1;
    correl.at(slot)[2] += value1 * value1;
    correl.at(slot)[3] += value2;
    correl.at(slot)[4] += value2 * value2;
    correl.at(slot)[5] += value1 * value2;
}

void dbg_print() {

    int64_t n;
    auto    e = [&n](int64_t x) {
        return static_cast<double>(x) / static_cast<double>(n);
    };
    auto sqr = [](double x) {
        return x * x;
    };

    for (size_t i = 0; i < MAX_DEBUG_SLOTS; ++i) {
        if ((n = hit[i][0])) {
            std::cerr << "Hit #" << i << ": Total " << n << " Hits " << hit[i][1]
                      << " Hit Rate (%) " << 100.0 * e(hit[i][1]) << std::endl;
        }
    }

    for (size_t i = 0; i < MAX_DEBUG_SLOTS; ++i) {
        if ((n = mean[i][0])) {
            std::cerr << "Mean #" << i << ": Total " << n << " Mean " << e(mean[i][1]) << std::endl;
        }
    }

    for (size_t i = 0; i < MAX_DEBUG_SLOTS; ++i) {
        if ((n = stdev[i][0])) {
            double r = sqrt(e(stdev[i][2]) - sqr(e(stdev[i][1])));
            std::cerr << "Stdev #" << i << ": Total " << n << " Stdev " << r << std::endl;
        }
    }

    for (size_t i = 0; i < MAX_DEBUG_SLOTS; ++i) {
        if ((n = extremes[i][0])) {
            std::cerr << "Extremity #" << i << ": Total " << n << " Min " << extremes[i][2]
                      << " Max " << extremes[i][1] << std::endl;
        }
    }

    for (size_t i = 0; i < MAX_DEBUG_SLOTS; ++i) {
        if ((n = correl[i][0])) {
            double r = (e(correl[i][5]) - e(correl[i][1]) * e(correl[i][3]))
                     / (sqrt(e(correl[i][2]) - sqr(e(correl[i][1])))
                        * sqrt(e(correl[i][4]) - sqr(e(correl[i][3]))));
            std::cerr << "Correl. #" << i << ": Total " << n << " Coefficient " << r << std::endl;
        }
    }
}

void dbg_clear() {
    hit.fill({});
    mean.fill({});
    stdev.fill({});
    correl.fill({});
    extremes.fill({});
}
