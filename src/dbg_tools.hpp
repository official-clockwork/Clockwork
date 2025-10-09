#pragma once

#include <cstddef>
#include <cstdint>

constexpr size_t MAX_DEBUG_SLOTS = 32;

void dbg_hit_on(bool cond, size_t slot = 0);
void dbg_mean_of(int64_t value, size_t slot = 0);
void dbg_stdev_of(int64_t value, size_t slot = 0);
void dbg_extremes_of(int64_t value, size_t slot = 0);
void dbg_correl_of(int64_t value1, int64_t value2, size_t slot = 0);
void dbg_print();
void dbg_clear();
