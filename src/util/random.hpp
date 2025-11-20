#pragma once

#include "types.hpp"
#include <cstdint>
#include <cstring>


namespace Clockwork {
// Important notes on the random number generator:
// In a chess engine, random numbers are essentially used for zobrist hashing. While the quality of randomness is not as critical as in cryptographic applications, it is still important to ensure a good distribution to minimize hash collisions.
// In particular, zobrist hashing is a linear hashing technique that relies on XOR operations. Therefore, if we want to ensure a good distribution of hash values, we need the underlying generator to be nonlinear.
// An efficient and clean implementation of PCG. It uses the full 128-bit state, but only outputs 64 bits at a time.
// Reference: https://www.pcg-random.org/pdf/hmc-cs-2014-0905.pdf
#pragma once

#pragma once
#include <cstdint>
#include <cstring>

class Random {
public:
    using u128 = unsigned __int128;

    struct u256 {
        u64 lo0;
        u64 lo1;
        u64 hi0;
        u64 hi1;

        constexpr u128 low128() const {
            return (u128)lo0 | ((u128)lo1 << 64);
        }
        constexpr u128 high128() const {
            return (u128)hi0 | ((u128)hi1 << 64);
        }
    };


    static inline u128 state     = 0;
    static inline u128 increment = 0;

    // Multiplier constant from PCG paper
    static constexpr u128 MULT = (u128)0x2360ED051FC65DA4ULL << 64 | (u128)0x4385DF649FCCF645ULL;


    static constexpr u256 FIXED_SEED{0x5132397362474669ULL, 0x62334a6864476c32ULL,
                                     0x5a53424951305567ULL, 0x5257356e6157356cULL};

    struct Init {
        Init() {
            Random::seed(FIXED_SEED);
        }
    };
    static inline Init _auto_init;

    static void seed(const u256& s) {
        u128 state_ = s.low128();
        u128 incr_  = s.high128() | 1;  // must be odd for LCG
        from_state_incr(state_, incr_);
    }

    static u64 rand_64() {
        step();
        return output_xsl_rr(state);
    }

    static void advance(u128 delta) {
        u128 acc_mult = 1;
        u128 acc_plus = 0;
        u128 cur_mult = MULT;
        u128 cur_plus = increment;
        u128 d        = delta;

        while (d > 0) {
            if (d & 1) {
                acc_mult = acc_mult * cur_mult;
                acc_plus = acc_plus * cur_mult + cur_plus;
            }
            cur_plus = (cur_mult + 1) * cur_plus;
            cur_mult *= cur_mult;
            d >>= 1;
        }

        state = acc_mult * state + acc_plus;
    }

private:
    static void from_state_incr(u128 st, u128 inc) {
        state     = st;
        increment = inc;
        state += increment;
        step();
    }

    // LCG step
    static inline void step() {
        state = state * MULT + increment;
    }

    // Output permutation PCG-XSL-RR
    static inline u64 output_xsl_rr(u128 s) {
        constexpr u32 XSHIFT = 64;
        constexpr u32 ROTATE = 122;

        u32 rot = (u32)(s >> ROTATE);
        u64 xsl = (u64)(s >> XSHIFT) ^ (u64)s;

        return (xsl >> rot) | (xsl << ((-rot) & 63));
    }
};


}  // namespace Clockwork
