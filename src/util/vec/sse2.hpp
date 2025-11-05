#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>

#if defined(__SSE2__)
    #include <immintrin.h>
    #define F128_USE_SSE2 1
#else
    #define F128_USE_SSE2 0
#endif

struct f128 {
#if F128_USE_SSE2
    __m128d v = _mm_setzero_pd();
#else
    double lo = 0.0;
    double hi = 0.0;
#endif

    // ---- Constructors ----
    static inline f128 make(double a, double b) {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_set_pd(b, a);
        return r;
#else
        return {a, b};
#endif
    }

    static inline f128 broadcast(double x) {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_set1_pd(x);
        return r;
#else
        return {x, x};
#endif
    }

    static inline f128 zero() {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_setzero_pd();
        return r;
#else
        return {0.0, 0.0};
#endif
    }

    // ---- Extract ----
    inline double first() const {
#if F128_USE_SSE2
        alignas(16) double buf[2];
        _mm_store_pd(buf, v);
        return buf[0];
#else
        return lo;
#endif
    }

    inline double second() const {
#if F128_USE_SSE2
        alignas(16) double buf[2];
        _mm_store_pd(buf, v);
        return buf[1];
#else
        return hi;
#endif
    }

    // ---- Arithmetic ----
    static inline f128 add(const f128& a, const f128& b) {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_add_pd(a.v, b.v);
        return r;
#else
        return {a.lo + b.lo, a.hi + b.hi};
#endif
    }

    static inline f128 sub(const f128& a, const f128& b) {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_sub_pd(a.v, b.v);
        return r;
#else
        return {a.lo - b.lo, a.hi - b.hi};
#endif
    }

    static inline f128 mul(const f128& a, const f128& b) {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_mul_pd(a.v, b.v);
        return r;
#else
        return {a.lo * b.lo, a.hi * b.hi};
#endif
    }

    static inline f128 div(const f128& a, const f128& b) {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_div_pd(a.v, b.v);
        return r;
#else
        return {a.lo / b.lo, a.hi / b.hi};
#endif
    }

    static inline f128 neg(const f128& a) {
#if F128_USE_SSE2
        __m128d zero = _mm_setzero_pd();
        f128    r;
        r.v = _mm_sub_pd(zero, a.v);
        return r;
#else
        return {-a.lo, -a.hi};
#endif
    }

    // ---- Scalar ops ----
    static inline f128 add_scalar(const f128& a, double s) {
        return add(a, broadcast(s));
    }

    static inline f128 sub_scalar(const f128& a, double s) {
        return sub(a, broadcast(s));
    }

    static inline f128 mul_scalar(const f128& a, double s) {
        return mul(a, broadcast(s));
    }

    static inline f128 div_scalar(const f128& a, double s) {
        return div(a, broadcast(s));
    }

    static inline f128 scalar_div(double s, const f128& a) {
#if F128_USE_SSE2
        __m128d num = _mm_set1_pd(s);
        f128    r;
        r.v = _mm_div_pd(num, a.v);
        return r;
#else
        return {s / a.lo, s / a.hi};
#endif
    }

    // ---- Math functions ----
    static inline f128 sqrt(const f128& a) {
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_sqrt_pd(a.v);
        return r;
#else
        return {std::sqrt(a.lo), std::sqrt(a.hi)};
#endif
    }

    // ---- FMA-style (useful for gradient updates) ----
    static inline f128 madd(const f128& a, const f128& b, const f128& c) {
        // a + b*c
#if F128_USE_SSE2
        f128 r;
        r.v = _mm_add_pd(a.v, _mm_mul_pd(b.v, c.v));
        return r;
#else
        return {a.lo + b.lo * c.lo, a.hi + b.hi * c.hi};
#endif
    }

    // ---- Printing ----
    friend std::ostream& operator<<(std::ostream& os, const f128& f) {
        os << "(" << f.first() << ", " << f.second() << ")";
        return os;
    }
};
