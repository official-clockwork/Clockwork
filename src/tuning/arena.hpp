#pragma once
#include "util/vec/sse2.hpp"

#include "util/types.hpp"
#include <cassert>
#include <vector>

namespace Clockwork::Autograd {

class ValueArena {
public:
    ValueArena() = default;

    void reserve(usize n) {
        values.reserve(n);
        gradients.reserve(n);
    }

    u32 alloc(f64 value, f64 grad = 0.0) {
        u32 idx = static_cast<u32>(values.size());
        values.push_back(value);
        gradients.push_back(grad);
        return idx;
    }

    u32 alloc_uninitialized() {
        u32 idx = static_cast<u32>(values.size());
        values.push_back(0.0);
        gradients.push_back(0.0);
        return idx;
    }

    inline u32 next_index() const {
        return static_cast<u32>(values.size()); 
    }

    // Mutating accessors
    inline f64& val(u32 i) {
        assert(i < values.size());
        return values[i];
    }
    inline f64& grad(u32 i) {
        assert(i < gradients.size());
        return gradients[i];
    }

    // Const accessors
    inline const f64& val(u32 i) const {
        assert(i < values.size());
        return values[i];
    }
    inline const f64& grad(u32 i) const {
        assert(i < gradients.size());
        return gradients[i];
    }

    inline usize size() const {
        return values.size();
    }

    void clear() {
        values.clear();
        gradients.clear();
    }

    void reset_to(usize n) {
        if (n < values.size()) {
            values.resize(n);
            gradients.resize(n);
        }
    }

    inline f64* values_data() {
        return values.data();
    }
    inline f64* gradients_data() {
        return gradients.data();
    }
    inline const f64* values_data() const {
        return values.data();
    }
    inline const f64* gradients_data() const {
        return gradients.data();
    }

private:
    std::vector<f64> values;
    std::vector<f64> gradients;
};


class PairArena {
public:
    PairArena() = default;

    void reserve(usize n) {
        p0.reserve(n);
        p1.reserve(n);
        g0.reserve(n);
        g1.reserve(n);
    }

    u32 alloc(f64x2 v, f64x2 g = f64x2::zero()) {
        u32 idx = static_cast<u32>(p0.size());
        p0.push_back(v.first());
        p1.push_back(v.second());
        g0.push_back(g.first());
        g1.push_back(g.second());
        return idx;
    }

    u32 alloc_uninitialized() {
        u32 idx = static_cast<u32>(p0.size());
        p0.push_back(0.0);
        p1.push_back(0.0);
        g0.push_back(0.0);
        g1.push_back(0.0);
        return idx;
    }

    inline u32 next_index() const {
        return static_cast<u32>(p0.size());
    }

    // Mutating accessors
    inline f64& p0_mut(u32 i) {
        assert(i < p0.size());
        return p0[i];
    }
    inline f64& p1_mut(u32 i) {
        assert(i < p1.size());
        return p1[i];
    }
    inline f64& g0_mut(u32 i) {
        assert(i < g0.size());
        return g0[i];
    }
    inline f64& g1_mut(u32 i) {
        assert(i < g1.size());
        return g1[i];
    }

    // Const accessors
    inline const f64& p0_ref(u32 i) const {
        assert(i < p0.size());
        return p0[i];
    }
    inline const f64& p1_ref(u32 i) const {
        assert(i < p1.size());
        return p1[i];
    }
    inline const f64& g0_ref(u32 i) const {
        assert(i < g0.size());
        return g0[i];
    }
    inline const f64& g1_ref(u32 i) const {
        assert(i < g1.size());
        return g1[i];
    }

    inline usize size() const {
        return p0.size();
    }

    void clear() {
        p0.clear();
        p1.clear();
        g0.clear();
        g1.clear();
    }

    void reset_to(usize n) {
        if (n < p0.size()) {
            p0.resize(n);
            p1.resize(n);
            g0.resize(n);
            g1.resize(n);
        }
    }

    inline f64* p0_data() {
        return p0.data();
    }
    inline f64* p1_data() {
        return p1.data();
    }
    inline f64* g0_data() {
        return g0.data();
    }
    inline f64* g1_data() {
        return g1.data();
    }
    inline const f64* p0_data() const {
        return p0.data();
    }
    inline const f64* p1_data() const {
        return p1.data();
    }
    inline const f64* g0_data() const {
        return g0.data();
    }
    inline const f64* g1_data() const {
        return g1.data();
    }

private:
    std::vector<f64> p0;
    std::vector<f64> p1;
    std::vector<f64> g0;
    std::vector<f64> g1;
};

}  // namespace Clockwork::Autograd
