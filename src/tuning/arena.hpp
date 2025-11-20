#pragma once

#include "util/types.hpp"
#include <cassert>
#include <vector>

namespace Clockwork::Autograd {

// A simple contiguous storage for types T.
// Returns indices (handles) instead of pointers.
template<typename T>
class Arena {
private:
    std::vector<T> m_data;

public:
    // Allocates a new slot and returns its index
    u32 alloc(const T& initial_value) {
        u32 idx = static_cast<u32>(m_data.size());
        m_data.push_back(initial_value);
        return idx;
    }

    // Emplace version
    template<typename... Args>
    u32 emplace(Args&&... args) {
        u32 idx = static_cast<u32>(m_data.size());
        m_data.emplace_back(std::forward<Args>(args)...);
        return idx;
    }

    // Accessors
    inline T& operator[](u32 index) {
        assert(index < m_data.size());
        return m_data[index];
    }

    inline const T& operator[](u32 index) const {
        assert(index < m_data.size());
        return m_data[index];
    }

    inline usize size() const {
        return m_data.size();
    }

    // Resets the arena size, effectively clearing it.
    // Note: Does not free memory (capacity remains) to reduce allocations next cycle.
    void clear() {
        m_data.clear();
    }

    // Keeps the first `n` elements, effectively clearing the rest.
    // Useful for keeping parameters which are at the start of the arena.
    void reset_to(usize n) {
        if (n < m_data.size()) {
            m_data.resize(n);
        }
    }

    std::vector<T>& raw() {
        return m_data;
    }
};

}  // namespace Clockwork::Autograd
