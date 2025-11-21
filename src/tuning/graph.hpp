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

struct ValueData {
    f64 value;
    f64 gradient;
};

struct PairData {
    f128 values;
    f128 gradients;
};

class Graph {
private:
    // Storage
    Arena<ValueData> m_values;
    Arena<PairData>  m_pairs;

    // Tape (Linear record of operations)
    std::vector<Node> m_tape;

    // Counts of global parameters (they sit at the start of the arenas)
    usize m_global_param_count = 0;
    usize m_global_pair_count  = 0;

    Graph();

public:
    static Graph& get();

    // Creation
    ValueHandle create_value(f64 data, bool is_parameter = false);
    PairHandle  create_pair(f128 data, bool is_parameter = false);

    // Operation recording stuff

    // Value-Value Binary
    ValueHandle record_op(OpType op, ValueHandle lhs, ValueHandle rhs);
    // Value Unary / Scalar
    ValueHandle record_op(OpType op, ValueHandle input, f64 scalar = 0.0);
    // Pair-Pair Binary
    PairHandle record_pair_op(OpType op, PairHandle lhs, PairHandle rhs);
    // Pair-Scalar
    PairHandle record_pair_scalar(OpType op, PairHandle input, f64 scalar);
    // Pair-Value
    PairHandle record_pair_value(OpType op, PairHandle pair, ValueHandle val);

    // Handling phasing separately due to its unique nature, probably can be done better
    ValueHandle record_phase(PairHandle input, f64 alpha);

    void backward();

    void       cleanup();
    void       zero_grad();
    void       copy_parameter_values(const Parameters& source);
    Parameters get_all_parameter_values() const;
    Parameters get_all_parameter_gradients() const;

    // Accessors for Handles
    ValueData& get_value_data(ValueHandle h) {
        return m_values[h.index];
    }
    const ValueData& get_value_data(ValueHandle h) const {
        return m_values[h.index];
    }

    PairData& get_pair_data(PairHandle h) {
        return m_pairs[h.index];
    }
    const PairData& get_pair_data(PairHandle h) const {
        return m_pairs[h.index];
    }

    ValueHandle get_parameter(usize global_index) const {
        return ValueHandle(static_cast<u32>(global_index));
    }

    PairHandle get_pair_parameter(usize global_index) const {
        return PairHandle(static_cast<u32>(global_index));
    }
};

}  // namespace Clockwork::Autograd
