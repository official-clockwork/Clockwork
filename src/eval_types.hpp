#pragma once

#include "util/types.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <cmath>

#ifdef EVAL_TUNING
    #include "tuning/globals.hpp"
    #include "tuning/loss.hpp"
    #include "tuning/optim.hpp"
    #include "tuning/value.hpp"
#endif

namespace Clockwork {

#ifndef EVAL_TUNING
using Score = i16;

class PScore {
private:
    i32 m_score;
    explicit constexpr PScore(i32 score) :
        m_score{score} {
    }

public:
    constexpr PScore() :
        m_score{} {
    }

    constexpr PScore(Score midgame, Score endgame) :
        m_score{static_cast<i32>(static_cast<u32>(endgame) << 16) + midgame} {
        assert(std::numeric_limits<i16>::min() <= midgame
               && std::numeric_limits<i16>::max() >= midgame);
        assert(std::numeric_limits<i16>::min() <= endgame
               && std::numeric_limits<i16>::max() >= endgame);
    }

    [[nodiscard]] inline auto mg() const {
        const auto mg = static_cast<u16>(m_score);

        i16 v{};
        std::memcpy(&v, &mg, sizeof(mg));

        return static_cast<Score>(v);
    }

    [[nodiscard]] inline auto eg() const {
        const auto eg = static_cast<u16>(static_cast<u32>(m_score + 0x8000) >> 16);

        i16 v{};
        std::memcpy(&v, &eg, sizeof(eg));

        return static_cast<Score>(v);
    }

    [[nodiscard]] constexpr auto operator+(const PScore& other) const {
        return PScore{m_score + other.m_score};
    }

    constexpr auto operator+=(const PScore& other) -> auto& {
        m_score += other.m_score;
        return *this;
    }

    [[nodiscard]] constexpr auto operator-(const PScore& other) const {
        return PScore{m_score - other.m_score};
    }

    constexpr auto operator-=(const PScore& other) -> auto& {
        m_score -= other.m_score;
        return *this;
    }

    [[nodiscard]] constexpr auto operator*(i32 v) const {
        return PScore{m_score * v};
    }

    constexpr auto operator*=(i32 v) -> auto& {
        m_score *= v;
        return *this;
    }

    [[nodiscard]] constexpr auto operator-() const {
        return PScore{-m_score};
    }

    [[nodiscard]] constexpr bool operator==(const PScore& other) const = default;

    [[nodiscard]] constexpr const PScore* operator->() const {
        return this;
    }

    // Phasing between two scores
    template<i32 max>
    Value phase(i32 alpha) const {
        assert(0 <= alpha && alpha <= max);
        return static_cast<Value>((mg() * alpha + eg() * (max - alpha)) / max);
    }

    friend std::ostream& operator<<(std::ostream& stream, const PScore& score) {
        stream << "(" << score.mg() << "\t" << score.eg() << ")";
        return stream;
    }
};

using PParam = PScore;
#else

using Score  = Autograd::ValueHandle;
using PScore = Autograd::PairHandle;
using PParam = Autograd::PairPlaceholder;  // Handle for the TUNABLE parameter

#endif


#ifdef EVAL_TUNING
    // Tunable scalar pair (mg, eg)
    #define S(a, b) Autograd::PairPlaceholder::create_tunable((a), (b))

    // Constant scalar pair (mg, eg)
    #define CS(a, b) Autograd::PairPlaceholder::create((a), (b))

    // Zero pair FOR PARAMETERS (e.g., in an array)
    #define PPARAM_ZERO Autograd::PairPlaceholder::create(0, 0)

    // Zero pair FOR INTERMEDIATES (e.g., scores)
    #define PSCORE_ZERO Autograd::PairHandle::create(0, 0)

#else
// ... (non-tuning definitions) ...
    #define S(a, b) PScore((a), (b))
    #define CS(a, b) PScore((a), (b))
    #define PPARAM_ZERO PScore(0, 0)
    #define PSCORE_ZERO PScore(0, 0)
#endif


// TunableSigmoid: a * sigmoid((x + c) / b)
// a and c are tunable pairs (mg, eg), b is a constant scale parameter
// For inference: uses a lookup table with linear interpolation in the 95% range
// 95% range is approximately [-b*ln(39), b*ln(39)] = [-3.664b, 3.664b]

template<i32 B_SCALE = 1000>
#ifdef EVAL_TUNING
class TunableSigmoid {
private:
    PParam m_a;  // Scaling parameter
    PParam m_c;  // Offset parameter

    static constexpr f64 B = static_cast<f64>(B_SCALE);

public:
    TunableSigmoid(PParam a, PParam c)
        m_a(a),
        m_c(c) {
            std::cout << m_a;
            std::cout << m_c;
    }

    PScore operator()(PScore x) const {
        auto scaled  = x / B;
        auto shifted = scaled + (m_c / B);
        auto sig = shifted.sigmoid();
        return m_a * sig;
    }

    PParam a() const {
        return m_a;
    }
    i32 b() const {
        return B_SCALE;
    }
    PParam c() const {
        return m_c;
    }
};
#else
class TunableSigmoid {
private:
    static constexpr i32 TABLE_SIZE = 256;
    static constexpr i32 FP_SHIFT   = 16;
    static constexpr i32 FP_ONE     = 1 << FP_SHIFT;
    static constexpr f64 B          = static_cast<f64>(B_SCALE);
    static constexpr f64 LN_39      = 3.6635616461296463;

    struct Table {
        i32                         range_min;
        i32                         range_max;
        i32                         range_span;
        i32                         scale_fp;
        std::array<i16, TABLE_SIZE> values;
    };

    Table m_mg;
    Table m_eg;

public:
    TunableSigmoid(PParam a, PParam c) {
        build_table(m_mg, a.mg(), c.mg());
        build_table(m_eg, a.eg(), c.eg());
    }

    PScore operator()(PScore x) const {
        return PScore(lookup(x.mg(), m_mg), lookup(x.eg(), m_eg));
    }

private:
    static void build_table(Table& tbl, i32 a, i32 c) {
        const f64 bound = B * LN_39;
        tbl.range_min   = static_cast<i32>(-bound) - c;
        tbl.range_max   = static_cast<i32>(bound) - c;
        tbl.range_span  = tbl.range_max - tbl.range_min;

        tbl.scale_fp =
          static_cast<i32>((static_cast<i64>(TABLE_SIZE - 1) << FP_SHIFT) / tbl.range_span);

        for (i32 i = 0; i < TABLE_SIZE; ++i) {
            const f64 t   = static_cast<f64>(i) / (TABLE_SIZE - 1);
            const f64 x   = tbl.range_min + t * tbl.range_span;
            const f64 z   = (x + c) / B;
            const f64 sig = 1.0 / (1.0 + std::exp(-z));

            tbl.values[i] = static_cast<i16>(std::lround(a * sig));
        }
    }

    static i16 lookup(i16 x_val, const Table& tbl) {
        const i32 x      = std::clamp(static_cast<i32>(x_val), tbl.range_min, tbl.range_max);
        const i64 idx_fp = static_cast<i64>(x - tbl.range_min) * tbl.scale_fp;

        const i32 idx  = static_cast<i32>(idx_fp >> FP_SHIFT);
        const i32 frac = static_cast<i32>(idx_fp & (FP_ONE - 1));

        const i32 v0 = tbl.values[idx];
        const i32 v1 = tbl.values[std::min(idx + 1, TABLE_SIZE - 1)];

        return static_cast<i16>(v0 + ((v1 - v0) * frac >> FP_SHIFT));
    }
};
#endif

}  // namespace Clockwork
