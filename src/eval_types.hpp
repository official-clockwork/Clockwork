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
// ============================================================================
//   NORMAL BUILD (NO TUNING)
// ============================================================================
using Score = i16;

class PScore {
private:
    i32 m_score;

    explicit constexpr PScore(i32 score) :
        m_score(score) {
    }

public:
    constexpr PScore() :
        m_score{} {
    }

    constexpr PScore(Score midgame, Score endgame) :
        m_score(static_cast<i32>((u32(endgame) << 16) + u16(midgame))) {
        assert(std::numeric_limits<i16>::min() <= midgame
               && midgame <= std::numeric_limits<i16>::max());
        assert(std::numeric_limits<i16>::min() <= endgame
               && endgame <= std::numeric_limits<i16>::max());
    }

    inline Score mg() const {
        u16 mg = u16(m_score);
        i16 v;
        std::memcpy(&v, &mg, sizeof(mg));
        return v;
    }

    inline Score eg() const {
        u16 eg = u16(u32(m_score + 0x8000) >> 16);
        i16 v;
        std::memcpy(&v, &eg, sizeof(eg));
        return v;
    }

    // Operators identical to original version
    constexpr PScore operator+(const PScore& o) const {
        return PScore(m_score + o.m_score);
    }
    constexpr PScore operator-(const PScore& o) const {
        return PScore(m_score - o.m_score);
    }
    constexpr PScore operator*(i32 v) const {
        return PScore(m_score * v);
    }
    constexpr PScore& operator+=(const PScore& o) {
        m_score += o.m_score;
        return *this;
    }
    constexpr PScore& operator-=(const PScore& o) {
        m_score -= o.m_score;
        return *this;
    }
    constexpr PScore& operator*=(i32 v) {
        m_score *= v;
        return *this;
    }
    constexpr PScore operator-() const {
        return PScore(-m_score);
    }

    constexpr bool operator==(const PScore&) const = default;

    constexpr const PScore* operator->() const {
        return this;
    }

    // Phase function (non-tuning: returns int)
    template<i32 max>
    inline Value phase(i32 alpha) const {
        assert(0 <= alpha && alpha <= max);
        return Value((mg() * alpha + eg() * (max - alpha)) / max);
    }

    friend std::ostream& operator<<(std::ostream& os, const PScore& s) {
        os << "(" << s.mg() << "\t" << s.eg() << ")";
        return os;
    }
};

using PParam = PScore;

#else
// ============================================================================
//   TUNING BUILD (NEW AUTOGRAD API)
// ============================================================================

using Score  = Autograd::ValueHandle;
using PScore = Autograd::PairHandle;  // (mg, eg) handle
using PParam = Autograd::PairHandle;  // tunable pair

#endif


// ============================================================================
//   Macro Definitions
// ============================================================================

#ifdef EVAL_TUNING
    // Tunable scalar pair (mg, eg)
    #define S(a, b) Autograd::PairPlaceholder::create_tunable((a), (b))

    // Constant (fixed) scalar pair (mg, eg)
    #define CS(a, b) Autograd::PairPlaceholder::create((a), (b))

    // Zero pair
    #define PSCORE_ZERO Autograd::PairPlaceholder::create(0, 0)

#else
    // Non-tuning build: use fixed, non-autograd PScore
    #define S(a, b) PScore((a), (b))
    #define CS(a, b) PScore((a), (b))
    #define PSCORE_ZERO PScore(0, 0)
#endif

}  // namespace Clockwork