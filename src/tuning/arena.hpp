#pragma once

#include "util/types.hpp"
#include <cassert>
#include <vector>

namespace Clockwork::Autograd {

/// ARENA IMPLEMENTATION \\\
// Simple vector-based arena for storing values and pairs. Surely can be done better. Kek.
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

    // Emplace version we might want later for ops that return many values?
    // Might be seeing things.
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

    // Common std::vector W
    void clear() {
        m_data.clear();
    }

    void reset_to(usize n) {
        if (n < m_data.size()) {
            m_data.resize(n);
        }
    }
};

}  // namespace Clockwork::Autograd
