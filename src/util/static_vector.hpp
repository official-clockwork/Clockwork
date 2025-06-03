#pragma once

#include <algorithm>
#include <array>
#include <utility>
#include <cassert>

#include "util/types.hpp"

namespace Clockwork {

template<typename T, usize cap>
class StaticVector {
   public:
    using value_type     = T;
    using iterator       = T*;
    using const_iterator = const T*;
    using size_type      = usize;

    constexpr StaticVector() = default;
    ~StaticVector()          = default;

    constexpr StaticVector(const StaticVector& other) :
        len(other.len) {
        std::copy(other.begin(), other.end(), begin());
    }

    constexpr StaticVector(StaticVector&& other) :
        len(std::exchange(other.len, 0)) {
        std::move(other.begin(), other.end(), begin());
    }

    constexpr StaticVector& operator=(const StaticVector& other) {
        len = other.len;
        std::copy(other.begin(), other.end(), begin());
        return *this;
    }

    constexpr StaticVector& operator=(StaticVector&& other) {
        len = std::exchange(other.len, 0);
        std::move(other.begin(), other.end(), begin());
        return *this;
    }

    constexpr iterator push_back(const T& value) {
        assert(len < cap);
        data[len] = value;
        return &data[len++];
    }

    constexpr void append(StaticVector&& other) {
        assert(len + other.len < cap);
        std::move(other.begin(), other.end(), end());
        len += std::exchange(other.len, 0);
    }

    constexpr void clear() { len = 0; }

    constexpr usize size() const { return len; }
    constexpr usize capacity() const { return cap; }
    constexpr bool  empty() const { return len == 0; }

    constexpr auto resize(usize new_size) -> void {
        assert(new_size <= cap);
        len = new_size;
    }

    constexpr T& operator[](usize index) {
        assert(index < len);
        return data[index];
    }
    constexpr const T& operator[](usize index) const {
        assert(index < len);
        return data[index];
    }

    constexpr iterator       begin() { return &data[0]; }
    constexpr const_iterator begin() const { return &data[0]; }
    constexpr const_iterator cbegin() const { return begin(); }

    constexpr iterator       end() { return &data[len]; }
    constexpr const_iterator end() const { return &data[len]; }
    constexpr const_iterator cend() const { return end(); }

   protected:
    usize              len = 0;
    std::array<T, cap> data;
};

}  // namespace Clockwork
