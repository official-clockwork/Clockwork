#pragma once

#include "arena.hpp"
#include "info.hpp"
#include "operations.hpp"
#include "util/vec/sse2.hpp"
#include "value.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace Clockwork::Autograd {

class Graph {
private:
    ValueArena m_values;
    PairArena  m_pairs;

    // Tape (Linear record of operations)
    std::vector<Node> m_tape;

    // Counts of global parameters
    usize m_global_param_count = 0;
    usize m_global_pair_count  = 0;

    Graph();

public:
    inline static Graph& get() {
        thread_local Graph instance;
        return instance;
    }

    // Creation
    ValueHandle create_value(f64 data);
    PairHandle  create_pair(f64x2 data);

    // Operation recording
    ValueHandle record_op(OpType op, ValueHandle lhs, ValueHandle rhs);
    ValueHandle record_op(OpType op, ValueHandle input, f64 scalar = 0.0);
    PairHandle  record_pair_op(OpType op, PairHandle lhs, PairHandle rhs);
    PairHandle  record_pair_scalar(OpType op, PairHandle input, f64 scalar);
    PairHandle  record_pair_value(OpType op, PairHandle pair, ValueHandle val);
    PairHandle record_pair_value(OpType op, PairHandle lhs, PairHandle rhs);
    PairHandle  record_pair_unary(OpType op, PairHandle input);

    ValueHandle record_phase(PairHandle input, f64 alpha);

    void backward();

    void       cleanup();
    void       zero_grad();
    void       copy_parameter_values(const Parameters& source);
    Parameters get_all_parameter_values() const;
    Parameters get_all_parameter_gradients() const;

    void add_value_gradient(u32 idx, f64 delta);
    void set_value(u32 idx, f64 v);
    void zero_value_grad(u32 idx);

    void set_pair_values(u32 idx, const f64x2& v);
    void zero_pair_grad(u32 idx);

    // Direct SoA accessors
    f64 get_value(u32 idx) const {
        return m_values.val(idx);
    }
    f64 get_gradient(u32 idx) const {
        return m_values.grad(idx);
    }

    f64x2 get_pair_values(u32 idx) const {
        return f64x2::make(m_pairs.p0_ref(idx), m_pairs.p1_ref(idx));
    }
    f64x2 get_pair_gradients(u32 idx) const {
        return f64x2::make(m_pairs.g0_ref(idx), m_pairs.g1_ref(idx));
    }

    // Pointer accessors
    f64* values_data() {
        return m_values.values_data();
    }
    f64* gradients_data() {
        return m_values.gradients_data();
    }

    ValueHandle get_parameter(usize global_index) const {
        return ValueHandle(static_cast<u32>(global_index));
    }

    PairHandle get_pair_parameter(usize global_index) const {
        return PairHandle(static_cast<u32>(global_index));
    }
};

}  // namespace Clockwork::Autograd
