#pragma once

#include "util/types.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <memory>
#include <utility>

namespace Clockwork {


/// C++26 has `std::inplace_vector`, but that's not yet implemented in any major stdlib.
/// This template is a simplified version of that idea.
/// We don't need to worry about special cases like elements being (partially) `const`, etc.
/// We also don't care about exception safety and simply assume that copying/moving `T` doesn't throw.
template<typename T, usize cap>
class StaticVector {

public:
    using value_type     = T;
    using iterator       = T*;
    using const_iterator = const T*;
    using size_type      = usize;

    StaticVector() :
        m_len(0) {
        // don't zero `m_storage`
    }
    ~StaticVector() {
        clear();
    }

    StaticVector(std::initializer_list<T> list) :
        m_len(list.size()) {
        assert(m_len <= cap);
        std::uninitialized_copy(list.begin(), list.end(), data());
    }

    explicit StaticVector(usize len) :
        m_len(len) {
        std::uninitialized_value_construct_n(data(), len);
    }

    StaticVector(const StaticVector& other) :
        m_len(other.m_len) {
        std::uninitialized_copy(other.begin(), other.end(), data());
    }

    StaticVector(StaticVector&& other) noexcept :
        m_len(other.m_len) {
        std::uninitialized_move(other.begin(), other.end(), data());
        // technically, this isn't necessary, but it should help catch potential bugs.
        other.clear();
    }

    StaticVector& operator=(const StaticVector& other) {
        if (other.m_len > m_len) {
            std::copy_n(other.begin(), m_len, data());
            std::uninitialized_copy(other.begin() + m_len, other.end(), end());
            m_len = other.m_len;
        } else {
            std::copy(other.begin(), other.end(), data());
            resize(other.m_len);
        }
        return *this;
    }

    StaticVector& operator=(StaticVector&& other) noexcept {
        if (&other == this) {
            return *this;
        }
        if (other.m_len > m_len) {
            std::move(other.begin(), other.begin() + m_len, data());
            std::uninitialized_move(other.begin() + m_len, other.end(), end());
            m_len = other.m_len;
        } else {
            std::move(other.begin(), other.end(), data());
            resize(other.m_len);
        }
        other.clear();
        return *this;
    }

    template<typename... Args>
        requires(std::constructible_from<T, Args...>)
    iterator emplace_back(Args&&... args) {
        assert(m_len < cap);
        T* res = std::construct_at(data() + m_len, std::forward<Args>(args)...);
        m_len++;
        return res;
    }

    template<typename F>
    void unsafe_append(F&& f) {
        usize sz = f(end());
        m_len += sz;
    }

    void append(const StaticVector& other) {
        assert(m_len + other.m_len <= cap);
        std::uninitialized_copy(other.begin(), other.end(), end());
        m_len += other.m_len;
    }

    iterator push_back(const T& value) {
        assert(m_len < cap);
        T* res = std::construct_at(data() + m_len, value);
        m_len++;
        return res;
    }

    iterator push_back(T&& value) {
        return emplace_back(std::move(value));
    }

    void append(StaticVector&& other) {
        assert(m_len + other.m_len <= cap);
        std::uninitialized_move(other.begin(), other.end(), end());
        m_len += other.m_len;
        other.clear();
    }

    T pop_back() {
        T& last = back();
        T  res  = std::move(last);
        std::destroy_at(&last);
        m_len--;
        return res;
    }

    void clear() {
        std::destroy_n(data(), m_len);
        m_len = 0;
    }

    [[nodiscard]] usize size() const {
        return m_len;
    }
    [[nodiscard]] usize capacity() const {
        return cap;
    }
    [[nodiscard]] bool empty() const {
        return m_len == 0;
    }

    /// This value initializes elements, that is, it zeros primitive types like `int`.
    void resize(usize new_size) {
        assert(new_size <= cap);
        if (new_size <= m_len) {
            std::destroy(begin() + new_size, end());
        } else {
            std::uninitialized_value_construct(end(), begin() + new_size);
        }
        m_len = new_size;
    }

    void resize(usize new_size, const T& value) {
        assert(new_size <= cap);
        if (new_size <= m_len) {
            std::destroy(begin() + new_size, end());
        } else {
            std::uninitialized_fill(end(), begin() + new_size, value);
        }
        m_len = new_size;
    }

    template<typename F>
    void resize_with(usize new_size, F&& f)
        requires std::invocable<F&> && std::constructible_from<T, std::invoke_result_t<F&>>
    {
        resize_with(new_size, [&](usize) {
            return f();
        });
    }


    T& operator[](usize index) {
        assert(index < m_len);
        return data()[index];
    }
    const T& operator[](usize index) const {
        assert(index < m_len);
        return data()[index];
    }

    // We never access the raw bytes directly except to immediately call `std::construct_at`.
    // This means we don't need to call `std::launder`.
    // Unfortunately, the reinterpret_cast means this can't be constexpr.
    [[nodiscard]] T* data() {
        return reinterpret_cast<T*>(m_storage.data());
    }

    [[nodiscard]] const T* data() const {
        return reinterpret_cast<const T*>(m_storage.data());
    }

    [[nodiscard]] iterator begin() {
        return data();
    }
    [[nodiscard]] const_iterator begin() const {
        return data();
    }
    [[nodiscard]] const_iterator cbegin() const {
        return begin();
    }

    [[nodiscard]] iterator end() {
        return begin() + m_len;
    }
    [[nodiscard]] const_iterator end() const {
        return begin() + m_len;
    }
    [[nodiscard]] const_iterator cend() const {
        return end();
    }

    T& back() {
        assert(m_len > 0);
        return data()[m_len - 1];
    }
    const T& back() const {
        assert(m_len > 0);
        return data()[m_len - 1];
    }

    friend auto operator<=>(const StaticVector& lhs, const StaticVector& rhs) {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(),
                                                      rhs.end());
    }

    friend auto operator==(const StaticVector& lhs, const StaticVector& rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

private:
    alignas(T) std::array<std::byte, cap * sizeof(T)> m_storage;

    usize m_len;
};

}  // namespace Clockwork
