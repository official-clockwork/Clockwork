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

using Score  = Autograd::ValuePtr;
using PScore = Autograd::PairPtr;
using PParam = Autograd::PairPlaceholder;

#endif

#ifdef EVAL_TUNING
    #define S(a, b) Autograd::PairPlaceholder::create_tunable((a), (b))  // Defines a tunable pscore
    #define CS(a, b) Autograd::PairPlaceholder::create((a), (b))
    #define PSCORE_ZERO Autograd::Pair::create(0, 0)
#else
    #define S(a, b) PScore((a), (b))  // Defines a constant pscore when not tuning
    #define CS(a, b) S((a), (b))
    #define PSCORE_ZERO CS(0, 0)
#endif

}  // namespace Clockwork
