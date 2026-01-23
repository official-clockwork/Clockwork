#pragma once

#include "util/types.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>

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


// TunableSigmoid: a * sigmoid(x / b + c)
// a and c are tunable pairs (mg, eg), b is a constant scale parameter
// For inference: uses a lookup table with linear interpolation in the 95% range
// 95% range is approximately [-b*ln(39), b*ln(39)] = [-3.664b, 3.664b]

template<i32 B_SCALE = 1000>
class TunableSigmoid {
#ifdef EVAL_TUNING
private:
    PParam m_a;  // Scaling parameter
    PParam m_c;  // Offset parameter

    static constexpr f64 B = static_cast<f64>(B_SCALE);

public:
    TunableSigmoid(f64 a_mg, f64 a_eg, f64 c_mg, f64 c_eg) :
        m_a(S(a_mg, a_eg)),
        m_c(S(c_mg, c_eg)) {
            std::cout << "init with " << a_mg << a_eg << c_mg << c_eg << std::endl;
            std::cout << m_a;
            std::cout << m_c;
    }

    PScore operator()(PScore x) const {
        auto scaled  = x / B;
        auto shifted = scaled + m_c;
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

#else  // Inference mode
private:
    // Lookup table parameters
    static constexpr i32 TABLE_SIZE = 256;
    static constexpr f64 B          = static_cast<f64>(B_SCALE);
    static constexpr f64 LN_39      = 3.6635616461296463;  // ln(39)
    static constexpr f64 RANGE_MIN  = -B * LN_39;
    static constexpr f64 RANGE_MAX  = B * LN_39;
    static constexpr f64 RANGE_SPAN = RANGE_MAX - RANGE_MIN;

    std::array<i16, TABLE_SIZE> m_table_mg;
    std::array<i16, TABLE_SIZE> m_table_eg;

public:
    TunableSigmoid(f64 a_mg, f64 a_eg, f64 c_mg, f64 c_eg) {
        // Build lookup tables for the function: a * sigmoid(x / b + c)
        for (i32 i = 0; i < TABLE_SIZE; ++i) {
            f64 t = static_cast<f64>(i) / (TABLE_SIZE - 1);
            f64 x = RANGE_MIN + t * RANGE_SPAN;

            // Compute sigmoid for both components
            f64 input_mg = x / B + c_mg;
            f64 input_eg = x / B + c_eg;

            f64 sig_mg = 1.0 / (1.0 + std::exp(-input_mg));
            f64 sig_eg = 1.0 / (1.0 + std::exp(-input_eg));

            m_table_mg[i] = static_cast<i16>(a_mg * sig_mg);
            m_table_eg[i] = static_cast<i16>(a_eg * sig_eg);
        }
    }

    PScore operator()(PScore x) const {
        // Extract MG and EG components
        i16 x_mg = x.mg();
        i16 x_eg = x.eg();

        // Lookup and interpolate for each component
        i16 result_mg = lookup_component(x_mg, m_table_mg);
        i16 result_eg = lookup_component(x_eg, m_table_eg);

        return PScore(result_mg, result_eg);
    }

private:
    i16 lookup_component(i16 x_val, const std::array<i16, TABLE_SIZE>& table) const {
        f64 x = static_cast<f64>(x_val);

        // Map x to [0, 1] range
        f64 t = (x - RANGE_MIN) / RANGE_SPAN;
        t     = std::clamp(t, 0.0, 1.0);

        // Get fractional table index
        f64 idx_f = t * (TABLE_SIZE - 1);
        i32 idx0  = static_cast<i32>(idx_f);
        i32 idx1  = std::min(idx0 + 1, TABLE_SIZE - 1);

        // Linear interpolation
        f64 frac = idx_f - idx0;
        return static_cast<i16>(table[idx0] + frac * (table[idx1] - table[idx0]));
    }
#endif
};

}  // namespace Clockwork
