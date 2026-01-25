#pragma once

#include "util/types.hpp"
#include "util/vec/sse2.hpp"
#include <cmath>
#include <iostream>
#include <vector>

namespace Clockwork::Autograd {

class Graph;

struct ValueHandle {
    u32 index;
    ValueHandle() :
        index(0xFFFFFFFF) {
    }
    explicit ValueHandle(u32 idx) :
        index(idx) {
    }
    bool is_valid() const {
        return index != 0xFFFFFFFF;
    }

    static ValueHandle create(f64 data);
    static ValueHandle sum(const std::vector<ValueHandle>& inputs);

    ValueHandle exp() const;
    ValueHandle log() const;
    ValueHandle sigmoid() const;
    ValueHandle pow(ValueHandle exponent) const;
    ValueHandle pow(f64 exponent) const;

    void add_gradient(f64 rhs) const;
    f64  get_value() const;
    f64  get_gradient() const;
    void zero_grad() const;
    void set_value(f64 v) const;
};

struct PairHandle {
    u32 index;
    PairHandle() :
        index(0xFFFFFFFF) {
    }
    explicit PairHandle(u32 idx) :
        index(idx) {
    }
    bool is_valid() const {
        return index != 0xFFFFFFFF;
    }

    static PairHandle create(f64 first, f64 second);
    static PairHandle create(const f64x2& values);
    static PairHandle create_tunable(f64 a, f64 b) {
        return create(a, b);
    }

    f64x2 get_values() const;
    f64x2 get_gradients() const;
    f64   first() const;
    f64   second() const;
    void  set_values(const f64x2& v) const;
    void  set_values(f64 f, f64 s) const;
    void  zero_grad() const;

    // Internal helper to avoid including Graph in header
    ValueHandle phase_impl(f64 scaled_alpha) const;

    template<i32 max>
    ValueHandle phase(f64 alpha) const {
        return phase_impl(alpha / max);
    }

    PairHandle sigmoid() const;
};

// Operation decls
ValueHandle operator-(ValueHandle a);
ValueHandle operator+(ValueHandle a, ValueHandle b);
ValueHandle operator-(ValueHandle a, ValueHandle b);
ValueHandle operator*(ValueHandle a, ValueHandle b);
ValueHandle operator/(ValueHandle a, ValueHandle b);
ValueHandle operator+(ValueHandle a, f64 b);
ValueHandle operator-(ValueHandle a, f64 b);
ValueHandle operator*(ValueHandle a, f64 b);
ValueHandle operator/(ValueHandle a, f64 b);
ValueHandle operator+(f64 a, ValueHandle b);
ValueHandle operator-(f64 a, ValueHandle b);
ValueHandle operator*(f64 a, ValueHandle b);
ValueHandle operator/(f64 a, ValueHandle b);
bool        operator<(ValueHandle a, ValueHandle b);
bool        operator>(ValueHandle a, ValueHandle b);

PairHandle    operator+(PairHandle a, PairHandle b);
PairHandle    operator-(PairHandle a, PairHandle b);
PairHandle    operator-(PairHandle a);
PairHandle    operator*(PairHandle a, f64 scalar);
PairHandle    operator*(f64 scalar, PairHandle a);
PairHandle    operator/(PairHandle a, f64 scalar);
PairHandle    operator/(f64 scalar, PairHandle a);
PairHandle    operator*(PairHandle a, ValueHandle v);
PairHandle    operator*(ValueHandle v, PairHandle a);
PairHandle    operator*(PairHandle a, PairHandle b);
PairHandle    operator/(PairHandle a, ValueHandle v);
PairHandle    operator/(ValueHandle v, PairHandle a);
std::ostream& operator<<(std::ostream& os, const PairHandle& p);

// Value Inplaces
ValueHandle& operator+=(ValueHandle& a, ValueHandle b);
ValueHandle& operator-=(ValueHandle& a, ValueHandle b);
ValueHandle& operator*=(ValueHandle& a, ValueHandle b);
ValueHandle& operator/=(ValueHandle& a, ValueHandle b);
ValueHandle& operator+=(ValueHandle& a, f64 b);
ValueHandle& operator-=(ValueHandle& a, f64 b);
ValueHandle& operator*=(ValueHandle& a, f64 b);
ValueHandle& operator/=(ValueHandle& a, f64 b);

// Pair Inplaces
PairHandle& operator+=(PairHandle& a, PairHandle b);
PairHandle& operator-=(PairHandle& a, PairHandle b);
PairHandle& operator*=(PairHandle& a, f64 scalar);
PairHandle& operator*=(PairHandle& a, ValueHandle v);
PairHandle& operator/=(PairHandle& a, f64 scalar);
PairHandle& operator/=(PairHandle& a, ValueHandle v);

}  // namespace Clockwork::Autograd
