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
        values.reserve(n);
        gradients.reserve(n);
    }

    u32 alloc(f64x2 v, f64x2 g = f64x2::zero()) {
        u32 idx = static_cast<u32>(values.size());
        values.push_back(v);
        gradients.push_back(g);
        return idx;
    }

    u32 alloc_uninitialized() {
        u32 idx = static_cast<u32>(values.size());
        values.push_back(f64x2::zero());
        gradients.push_back(f64x2::zero());
        return idx;
    }

    inline u32 next_index() const {
        return static_cast<u32>(values.size());
    }

    // Accessors
    inline f64x2& val(u32 i) {
        assert(i < values.size());
        return values[i];
    }

    inline const f64x2& val(u32 i) const {
        assert(i < values.size());
        return values[i];
    }

    inline f64x2& grad(u32 i) {
        assert(i < gradients.size());
        return gradients[i];
    }

    inline const f64x2& grad(u32 i) const {
        assert(i < gradients.size());
        return gradients[i];
    }

    // Legacy component accessors
    inline f64 p0_ref(u32 i) const {
        assert(i < values.size());
        return values[i].first();
    }

    inline f64 p1_ref(u32 i) const {
        assert(i < values.size());
        return values[i].second();
    }

    inline f64 g0_ref(u32 i) const {
        assert(i < gradients.size());
        return gradients[i].first();
    }

    inline f64 g1_ref(u32 i) const {
        assert(i < gradients.size());
        return gradients[i].second();
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

    // Pointer accessors for hot loops
    inline f64x2* values_data() {
        return values.data();
    }

    inline f64x2* gradients_data() {
        return gradients.data();
    }

    inline const f64x2* values_data() const {
        return values.data();
    }

    inline const f64x2* gradients_data() const {
        return gradients.data();
    }

private:
    std::vector<f64x2> values;
    std::vector<f64x2> gradients;
};

}  // namespace Clockwork::Autograd
