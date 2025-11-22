#include "tuning/graph.hpp"
#include "tuning/globals.hpp"
#include <cmath>

namespace Clockwork::Autograd {

Graph::Graph() {
    // Initialize arenas with global parameters
    auto params      = Globals::get().get_parameters();
    auto pair_params = Globals::get().get_pair_parameters();

    m_global_param_count = params.size();
    m_global_pair_count  = pair_params.size();

    // reserve some headroom
    m_values.reserve(m_global_param_count + 1024);
    m_pairs.reserve(m_global_pair_count + 1024);
    m_tape.reserve(16384 * 2 * 16);

    for (auto* p : params) {
        m_values.alloc(p->default_value(), 0.0);
    }
    for (auto* p : pair_params) {
        m_pairs.alloc(p->default_value(), f64x2::zero());
    }
}

Graph& Graph::get() {
    thread_local Graph instance;
    return instance;
}

ValueHandle Graph::create_value(f64 data) {
    return ValueHandle(m_values.alloc(data, 0.0));
}

PairHandle Graph::create_pair(f64x2 data) {
    return PairHandle(m_pairs.alloc(data, f64x2::zero()));
}

// Recording

ValueHandle Graph::record_op(OpType op, ValueHandle lhs, ValueHandle rhs) {
    u32 out = m_values.alloc_uninitialized();
    f64 l   = m_values.val(lhs.index);
    f64 r   = m_values.val(rhs.index);
    f64 res = 0.0;

    switch (op) {
    case OpType::Add:
        res = l + r;
        break;
    case OpType::Sub:
        res = l - r;
        break;
    case OpType::Mul:
        res = l * r;
        break;
    case OpType::Div:
        res = l / r;
        break;
    case OpType::Pow:
        res = std::pow(l, r);
        break;
    default:
        break;
    }
    m_values.val(out) = res;
    m_tape.push_back({op, out, lhs.index, rhs.index, 0.0});
    return ValueHandle(out);
}

ValueHandle Graph::record_op(OpType op, ValueHandle input, f64 scalar) {
    u32 out = m_values.alloc_uninitialized();
    f64 l   = m_values.val(input.index);
    f64 res = 0.0;

    switch (op) {
    case OpType::Exp:
        res = std::exp(l);
        break;
    case OpType::Log:
        res = std::log(l);
        break;
    case OpType::Sigmoid:
        res = 1.0 / (1.0 + std::exp(-l));
        break;
    case OpType::Neg:
        res = -l;
        break;
    case OpType::PowConst:
        res = std::pow(l, scalar);
        break;
    case OpType::AddScalar:
        res = l + scalar;
        break;
    case OpType::SubScalarVal:
        res = scalar - l;
        break;
    case OpType::ValSubScalar:
        res = l - scalar;
        break;
    case OpType::MulScalar:
        res = l * scalar;
        break;
    case OpType::DivScalarVal:
        res = scalar / l;
        break;
    case OpType::ValDivScalar:
        res = l / scalar;
        break;
    default:
        break;
    }
    m_values.val(out) = res;
    m_tape.push_back({op, out, input.index, 0, scalar});
    return ValueHandle(out);
}

PairHandle Graph::record_pair_op(OpType op, PairHandle lhs, PairHandle rhs) {
    u32   out = m_pairs.alloc_uninitialized();
    f64   l0  = m_pairs.p0_ref(lhs.index);
    f64   l1  = m_pairs.p1_ref(lhs.index);
    f64   r0  = m_pairs.p0_ref(rhs.index);
    f64   r1  = m_pairs.p1_ref(rhs.index);
    f64x2 res = f64x2::zero();

    switch (op) {
    case OpType::PairAdd:
        res = f64x2::add(f64x2::make(l0, l1), f64x2::make(r0, r1));
        break;
    case OpType::PairSub:
        res = f64x2::sub(f64x2::make(l0, l1), f64x2::make(r0, r1));
        break;
    default:
        break;
    }

    m_pairs.p0_mut(out) = res.first();
    m_pairs.p1_mut(out) = res.second();
    m_tape.push_back({op, out, lhs.index, rhs.index, 0.0});
    return PairHandle(out);
}

PairHandle Graph::record_pair_scalar(OpType op, PairHandle input, f64 scalar) {
    u32   out = m_pairs.alloc_uninitialized();
    f64   l0  = m_pairs.p0_ref(input.index);
    f64   l1  = m_pairs.p1_ref(input.index);
    f64x2 res = f64x2::zero();

    switch (op) {
    case OpType::PairNeg:
        res = f64x2::neg(f64x2::make(l0, l1));
        break;
    case OpType::PairMulScalar:
        res = f64x2::mul_scalar(f64x2::make(l0, l1), scalar);
        break;
    case OpType::PairDivScalar:
        res = f64x2::div_scalar(f64x2::make(l0, l1), scalar);
        break;
    case OpType::ScalarDivPair:
        res = f64x2::scalar_div(scalar, f64x2::make(l0, l1));
        break;
    default:
        break;
    }

    m_pairs.p0_mut(out) = res.first();
    m_pairs.p1_mut(out) = res.second();
    m_tape.push_back({op, out, input.index, 0, scalar});
    return PairHandle(out);
}

PairHandle Graph::record_pair_value(OpType op, PairHandle pair, ValueHandle val) {
    u32   out = m_pairs.alloc_uninitialized();
    f64   p0  = m_pairs.p0_ref(pair.index);
    f64   p1  = m_pairs.p1_ref(pair.index);
    f64   v   = m_values.val(val.index);
    f64x2 res = f64x2::zero();

    switch (op) {
    case OpType::PairMulValue:
    case OpType::ValueMulPair:
        res = f64x2::mul_scalar(f64x2::make(p0, p1), v);
        break;
    case OpType::PairDivValue:
        res = f64x2::div_scalar(f64x2::make(p0, p1), v);
        break;
    case OpType::ValueDivPair:
        res = f64x2::scalar_div(v, f64x2::make(p0, p1));
        break;
    default:
        break;
    }
    m_pairs.p0_mut(out) = res.first();
    m_pairs.p1_mut(out) = res.second();
    m_tape.push_back({op, out, pair.index, val.index, 0.0});
    return PairHandle(out);
}

ValueHandle Graph::record_phase(PairHandle input, f64 alpha) {
    u32 out = m_values.alloc_uninitialized();
    f64 p0  = m_pairs.p0_ref(input.index);
    f64 p1  = m_pairs.p1_ref(input.index);

    f64 val           = alpha * p0 + (1.0 - alpha) * p1;
    m_values.val(out) = val;
    m_tape.push_back({OpType::Phase, out, input.index, 0, alpha});
    return ValueHandle(out);
}

void Graph::backward() {
    if (m_tape.empty()) {
        return;
    }

    const auto& last_node               = m_tape.back();
    m_values.grad(last_node.output_idx) = 1.0;

    // Raw pointers for hot loops
    f64* vals  = m_values.values_data();
    f64* grads = m_values.gradients_data();

    f64* p0 = m_pairs.p0_data();
    f64* p1 = m_pairs.p1_data();
    f64* g0 = m_pairs.g0_data();
    f64* g1 = m_pairs.g1_data();

    for (auto it = m_tape.rbegin(); it != m_tape.rend(); ++it) {
        const Node& node = *it;

        switch (node.type) {
        // Value Binary
        case OpType::Add: {
            f64 grad = grads[node.output_idx];
            grads[node.lhs_idx] += grad;
            grads[node.rhs_idx] += grad;
            break;
        }
        case OpType::Sub: {
            f64 grad = grads[node.output_idx];
            grads[node.lhs_idx] += grad;
            grads[node.rhs_idx] -= grad;
            break;
        }
        case OpType::Mul: {
            f64 grad = grads[node.output_idx];
            f64 l    = vals[node.lhs_idx];
            f64 r    = vals[node.rhs_idx];
            grads[node.lhs_idx] += r * grad;
            grads[node.rhs_idx] += l * grad;
            break;
        }
        case OpType::Div: {
            f64 grad = grads[node.output_idx];
            f64 l    = vals[node.lhs_idx];
            f64 r    = vals[node.rhs_idx];
            grads[node.lhs_idx] += (1.0 / r) * grad;
            grads[node.rhs_idx] += (-l / (r * r)) * grad;
            break;
        }
        case OpType::Pow: {
            f64 grad = grads[node.output_idx];
            f64 base = vals[node.lhs_idx];
            f64 exp  = vals[node.rhs_idx];
            grads[node.lhs_idx] += exp * std::pow(base, exp - 1) * grad;
            grads[node.rhs_idx] += std::pow(base, exp) * std::log(base) * grad;
            break;
        }

        // Value Unary
        case OpType::Exp: {
            f64 grad = grads[node.output_idx];
            f64 val  = vals[node.output_idx];
            grads[node.lhs_idx] += val * grad;
            break;
        }
        case OpType::Log: {
            f64 grad = grads[node.output_idx];
            f64 l    = vals[node.lhs_idx];
            grads[node.lhs_idx] += (1.0 / l) * grad;
            break;
        }
        case OpType::Sigmoid: {
            f64 grad = grads[node.output_idx];
            f64 s    = vals[node.output_idx];
            grads[node.lhs_idx] += (s * (1.0 - s)) * grad;
            break;
        }
        case OpType::Neg: {
            grads[node.lhs_idx] -= grads[node.output_idx];
            break;
        }
        case OpType::PowConst: {
            f64 grad = grads[node.output_idx];
            f64 l    = vals[node.lhs_idx];
            f64 exp  = node.scalar_data;
            grads[node.lhs_idx] += exp * std::pow(l, exp - 1.0) * grad;
            break;
        }
        case OpType::AddScalar:
        case OpType::SubScalarVal:
        case OpType::ValSubScalar: {
            grads[node.lhs_idx] += grads[node.output_idx];
            break;
        }
        case OpType::MulScalar: {
            grads[node.lhs_idx] += node.scalar_data * grads[node.output_idx];
            break;
        }
        case OpType::ValDivScalar: {
            grads[node.lhs_idx] += (1.0 / node.scalar_data) * grads[node.output_idx];
            break;
        }
        case OpType::DivScalarVal: {
            f64 grad = grads[node.output_idx];
            f64 l    = vals[node.lhs_idx];
            grads[node.lhs_idx] += (-node.scalar_data / (l * l)) * grad;
            break;
        }

        // Pair Binary
        case OpType::PairAdd: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            g0[node.lhs_idx] += g0_out;
            g1[node.lhs_idx] += g1_out;
            g0[node.rhs_idx] += g0_out;
            g1[node.rhs_idx] += g1_out;
            break;
        }
        case OpType::PairSub: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            g0[node.lhs_idx] += g0_out;
            g1[node.lhs_idx] += g1_out;
            g0[node.rhs_idx] -= g0_out;
            g1[node.rhs_idx] -= g1_out;
            break;
        }

        // Pair Scalar
        case OpType::PairNeg: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            g0[node.lhs_idx] -= g0_out;
            g1[node.lhs_idx] -= g1_out;
            break;
        }
        case OpType::PairMulScalar: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            g0[node.lhs_idx] += g0_out * node.scalar_data;
            g1[node.lhs_idx] += g1_out * node.scalar_data;
            break;
        }
        case OpType::PairDivScalar: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            g0[node.lhs_idx] += g0_out / node.scalar_data;
            g1[node.lhs_idx] += g1_out / node.scalar_data;
            break;
        }
        case OpType::ScalarDivPair: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            f64 l0     = p0[node.lhs_idx];
            f64 l1     = p1[node.lhs_idx];
            f64 u0     = -node.scalar_data / (l0 * l0);
            f64 u1     = -node.scalar_data / (l1 * l1);
            g0[node.lhs_idx] += u0 * g0_out;
            g1[node.lhs_idx] += u1 * g1_out;
            break;
        }

        // Pair-Value
        case OpType::PairMulValue:
        case OpType::ValueMulPair: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            f64 p0v    = p0[node.lhs_idx];
            f64 p1v    = p1[node.lhs_idx];
            f64 v      = vals[node.rhs_idx];

            g0[node.lhs_idx] += g0_out * v;
            g1[node.lhs_idx] += g1_out * v;

            f64 contrib = p0v * g0_out + p1v * g1_out;
            grads[node.rhs_idx] += contrib;
            break;
        }
        case OpType::PairDivValue: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            f64 p0v    = p0[node.lhs_idx];
            f64 p1v    = p1[node.lhs_idx];
            f64 v      = vals[node.rhs_idx];

            g0[node.lhs_idx] += g0_out / v;
            g1[node.lhs_idx] += g1_out / v;

            f64 num = p0v * g0_out + p1v * g1_out;
            grads[node.rhs_idx] += -num / (v * v);
            break;
        }
        case OpType::ValueDivPair: {
            f64 g0_out = g0[node.output_idx];
            f64 g1_out = g1[node.output_idx];
            f64 p0v    = p0[node.lhs_idx];
            f64 p1v    = p1[node.lhs_idx];
            f64 v      = vals[node.rhs_idx];

            f64 u0 = -v / (p0v * p0v);
            f64 u1 = -v / (p1v * p1v);
            g0[node.lhs_idx] += u0 * g0_out;
            g1[node.lhs_idx] += u1 * g1_out;

            grads[node.rhs_idx] += g0_out / p0v + g1_out / p1v;
            break;
        }

        case OpType::Phase: {
            f64 grad  = grads[node.output_idx];
            f64 alpha = node.scalar_data;
            g0[node.lhs_idx] += alpha * grad;
            g1[node.lhs_idx] += (1.0 - alpha) * grad;
            break;
        }
        default:
            break;
        }
    }
}

void Graph::cleanup() {
    m_values.reset_to(m_global_param_count);
    m_pairs.reset_to(m_global_pair_count);
    m_tape.clear();
}

void Graph::zero_grad() {
    for (usize i = 0; i < m_global_param_count; ++i) {
        m_values.grad(i) = 0.0;
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        m_pairs.g0_mut(i) = 0.0;
        m_pairs.g1_mut(i) = 0.0;
    }
}

void Graph::copy_parameter_values(const Parameters& source) {
    if (source.parameters.size() != m_global_param_count
        || source.pair_parameters.size() != m_global_pair_count) {
        std::cerr << "Graph parameter count desync!" << std::endl;
        std::terminate();
    }
    for (usize i = 0; i < m_global_param_count; ++i) {
        m_values.val(i) = source.parameters[i];
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        m_pairs.p0_mut(i) = source.pair_parameters[i].first();
        m_pairs.p1_mut(i) = source.pair_parameters[i].second();
    }
}

Parameters Graph::get_all_parameter_values() const {
    Parameters p;
    p.parameters.reserve(m_global_param_count);
    p.pair_parameters.reserve(m_global_pair_count);
    for (usize i = 0; i < m_global_param_count; ++i) {
        p.parameters.push_back(m_values.val(i));
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        p.pair_parameters.push_back(f64x2::make(m_pairs.p0_ref(i), m_pairs.p1_ref(i)));
    }
    return p;
}

Parameters Graph::get_all_parameter_gradients() const {
    Parameters p;
    p.parameters.reserve(m_global_param_count);
    p.pair_parameters.reserve(m_global_pair_count);
    for (usize i = 0; i < m_global_param_count; ++i) {
        p.parameters.push_back(m_values.grad(i));
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        p.pair_parameters.push_back(f64x2::make(m_pairs.g0_ref(i), m_pairs.g1_ref(i)));
    }
    return p;
}

// Mutation Helpers

void Graph::add_value_gradient(u32 idx, f64 delta) {
    m_values.grad(idx) += delta;
}

void Graph::set_value(u32 idx, f64 v) {
    m_values.val(idx) = v;
}

void Graph::zero_value_grad(u32 idx) {
    m_values.grad(idx) = 0.0;
}

void Graph::set_pair_values(u32 idx, const f64x2& v) {
    m_pairs.p0_mut(idx) = v.first();
    m_pairs.p1_mut(idx) = v.second();
}

void Graph::zero_pair_grad(u32 idx) {
    m_pairs.g0_mut(idx) = 0.0;
    m_pairs.g1_mut(idx) = 0.0;
}

}  // namespace Clockwork::Autograd
