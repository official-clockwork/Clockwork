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

    for (auto* p : params) {
        m_values.alloc({p->default_value(), 0.0});
    }
    for (auto* p : pair_params) {
        m_pairs.alloc({p->default_value(), f128::zero()});
    }
}

Graph& Graph::get() {
    thread_local Graph instance;
    return instance;
}

ValueHandle Graph::create_value(f64 data, bool is_parameter) {
    return ValueHandle(m_values.alloc({data, 0.0}));
}

PairHandle Graph::create_pair(f128 data, bool is_parameter) {
    return PairHandle(m_pairs.alloc({data, f128::zero()}));
}

// ------------------ Recording ------------------

ValueHandle Graph::record_op(OpType op, ValueHandle lhs, ValueHandle rhs) {
    u32 out = m_values.alloc({0.0, 0.0});
    f64 l   = m_values[lhs.index].value;
    f64 r   = m_values[rhs.index].value;
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
    m_values[out].value = res;
    m_tape.push_back({op, out, lhs.index, rhs.index, 0.0});
    return ValueHandle(out);
}

ValueHandle Graph::record_op(OpType op, ValueHandle input, f64 scalar) {
    u32 out = m_values.alloc({0.0, 0.0});
    f64 l   = m_values[input.index].value;
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
    m_values[out].value = res;
    m_tape.push_back({op, out, input.index, 0, scalar});
    return ValueHandle(out);
}

PairHandle Graph::record_pair_op(OpType op, PairHandle lhs, PairHandle rhs) {
    u32  out = m_pairs.alloc({f128::zero(), f128::zero()});
    f128 l   = m_pairs[lhs.index].values;
    f128 r   = m_pairs[rhs.index].values;
    f128 res = f128::zero();

    switch (op) {
    case OpType::PairAdd:
        res = f128::add(l, r);
        break;
    case OpType::PairSub:
        res = f128::sub(l, r);
        break;
    default:
        break;
    }
    m_pairs[out].values = res;
    m_tape.push_back({op, out, lhs.index, rhs.index, 0.0});
    return PairHandle(out);
}

PairHandle Graph::record_pair_scalar(OpType op, PairHandle input, f64 scalar) {
    u32  out = m_pairs.alloc({f128::zero(), f128::zero()});
    f128 l   = m_pairs[input.index].values;
    f128 res = f128::zero();

    switch (op) {
    case OpType::PairNeg:
        res = f128::neg(l);
        break;
    case OpType::PairMulScalar:
        res = f128::mul_scalar(l, scalar);
        break;
    case OpType::PairDivScalar:
        res = f128::div_scalar(l, scalar);
        break;
    case OpType::ScalarDivPair:
        res = f128::scalar_div(scalar, l);
        break;
    default:
        break;
    }
    m_pairs[out].values = res;
    m_tape.push_back({op, out, input.index, 0, scalar});
    return PairHandle(out);
}

PairHandle Graph::record_pair_value(OpType op, PairHandle pair, ValueHandle val) {
    u32  out = m_pairs.alloc({f128::zero(), f128::zero()});
    f128 p   = m_pairs[pair.index].values;
    f64  v   = m_values[val.index].value;
    f128 res = f128::zero();

    switch (op) {
    case OpType::PairMulValue:
    case OpType::ValueMulPair:
        res = f128::mul_scalar(p, v);
        break;
    case OpType::PairDivValue:
        res = f128::div_scalar(p, v);
        break;
    case OpType::ValueDivPair:
        res = f128::scalar_div(v, p);
        break;
    default:
        break;
    }
    m_pairs[out].values = res;
    // Note: rhs is stored in rhs_idx, but it refers to m_values arena!
    // The OpType tells us which arena to look at.
    m_tape.push_back({op, out, pair.index, val.index, 0.0});
    return PairHandle(out);
}

ValueHandle Graph::record_phase(PairHandle input, f64 alpha) {
    u32  out = m_values.alloc({0.0, 0.0});
    f128 p   = m_pairs[input.index].values;
    // Linear interpolation: alpha * first + (1-alpha) * second
    f64 val             = alpha * p.first() + (1.0 - alpha) * p.second();
    m_values[out].value = val;
    m_tape.push_back({OpType::Phase, out, input.index, 0, alpha});
    return ValueHandle(out);
}

// ------------------ Backward ------------------

void Graph::backward() {
    if (m_tape.empty()) {
        return;
    }

    // Seed gradient of last node
    const auto& last_node = m_tape.back();
    // Assuming the last node produces a Value (the loss)
    m_values[last_node.output_idx].gradient = 1.0;

    // Reverse iterate
    for (auto it = m_tape.rbegin(); it != m_tape.rend(); ++it) {
        const Node& node = *it;

        // Fetch gradients and values based on op type
        // Note: References to vector elements are risky if alloc happened, but backward pass doesn't alloc.
        // Using pointers or indices is safer.

        switch (node.type) {
        // --- Value Binary ---
        case OpType::Add: {
            f64 grad = m_values[node.output_idx].gradient;
            m_values[node.lhs_idx].gradient += grad;
            m_values[node.rhs_idx].gradient += grad;
            break;
        }
        case OpType::Sub: {
            f64 grad = m_values[node.output_idx].gradient;
            m_values[node.lhs_idx].gradient += grad;
            m_values[node.rhs_idx].gradient -= grad;
            break;
        }
        case OpType::Mul: {
            f64 grad = m_values[node.output_idx].gradient;
            f64 l    = m_values[node.lhs_idx].value;
            f64 r    = m_values[node.rhs_idx].value;
            m_values[node.lhs_idx].gradient += r * grad;
            m_values[node.rhs_idx].gradient += l * grad;
            break;
        }
        case OpType::Div: {
            f64 grad = m_values[node.output_idx].gradient;
            f64 l    = m_values[node.lhs_idx].value;
            f64 r    = m_values[node.rhs_idx].value;
            m_values[node.lhs_idx].gradient += (1.0 / r) * grad;
            m_values[node.rhs_idx].gradient += (-l / (r * r)) * grad;
            break;
        }
        case OpType::Pow: {
            f64 grad = m_values[node.output_idx].gradient;
            f64 base = m_values[node.lhs_idx].value;
            f64 exp  = m_values[node.rhs_idx].value;
            m_values[node.lhs_idx].gradient += exp * std::pow(base, exp - 1) * grad;
            m_values[node.rhs_idx].gradient += std::pow(base, exp) * std::log(base) * grad;
            break;
        }

        // --- Value Unary ---
        case OpType::Exp: {
            // d/dx e^x = e^x = y
            f64 grad = m_values[node.output_idx].gradient;
            f64 val  = m_values[node.output_idx].value;
            m_values[node.lhs_idx].gradient += val * grad;
            break;
        }
        case OpType::Log: {
            f64 grad = m_values[node.output_idx].gradient;
            f64 l    = m_values[node.lhs_idx].value;
            m_values[node.lhs_idx].gradient += (1.0 / l) * grad;
            break;
        }
        case OpType::Sigmoid: {
            f64 grad = m_values[node.output_idx].gradient;
            f64 s    = m_values[node.output_idx].value;
            m_values[node.lhs_idx].gradient += (s * (1.0 - s)) * grad;
            break;
        }
        case OpType::Neg: {
            m_values[node.lhs_idx].gradient -= m_values[node.output_idx].gradient;
            break;
        }
        case OpType::PowConst: {
            f64 grad = m_values[node.output_idx].gradient;
            f64 l    = m_values[node.lhs_idx].value;
            f64 exp  = node.scalar_data;
            m_values[node.lhs_idx].gradient += exp * std::pow(l, exp - 1.0) * grad;
            break;
        }
        case OpType::AddScalar:
        case OpType::SubScalarVal: {
            m_values[node.lhs_idx].gradient += m_values[node.output_idx].gradient;
            break;
        }
        case OpType::ValSubScalar: {
            m_values[node.lhs_idx].gradient += m_values[node.output_idx].gradient;
            break;
        }
        case OpType::MulScalar: {
            m_values[node.lhs_idx].gradient +=
              node.scalar_data * m_values[node.output_idx].gradient;
            break;
        }
        case OpType::ValDivScalar: {
            m_values[node.lhs_idx].gradient +=
              (1.0 / node.scalar_data) * m_values[node.output_idx].gradient;
            break;
        }
        case OpType::DivScalarVal: {
            f64 grad = m_values[node.output_idx].gradient;
            f64 l    = m_values[node.lhs_idx].value;
            m_values[node.lhs_idx].gradient += (-node.scalar_data / (l * l)) * grad;
            break;
        }

        // --- Pair Binary ---
        case OpType::PairAdd: {
            f128 grad                       = m_pairs[node.output_idx].gradients;
            m_pairs[node.lhs_idx].gradients = f128::add(m_pairs[node.lhs_idx].gradients, grad);
            m_pairs[node.rhs_idx].gradients = f128::add(m_pairs[node.rhs_idx].gradients, grad);
            break;
        }
        case OpType::PairSub: {
            f128 grad                       = m_pairs[node.output_idx].gradients;
            m_pairs[node.lhs_idx].gradients = f128::add(m_pairs[node.lhs_idx].gradients, grad);
            m_pairs[node.rhs_idx].gradients = f128::sub(m_pairs[node.rhs_idx].gradients, grad);
            break;
        }

        // --- Pair Scalar ---
        case OpType::PairNeg: {
            f128 grad                       = m_pairs[node.output_idx].gradients;
            m_pairs[node.lhs_idx].gradients = f128::sub(m_pairs[node.lhs_idx].gradients, grad);
            break;
        }
        case OpType::PairMulScalar: {
            f128 grad        = m_pairs[node.output_idx].gradients;
            f128 scaled_grad = f128::mul_scalar(grad, node.scalar_data);
            m_pairs[node.lhs_idx].gradients =
              f128::add(m_pairs[node.lhs_idx].gradients, scaled_grad);
            break;
        }
        case OpType::PairDivScalar: {
            f128 grad        = m_pairs[node.output_idx].gradients;
            f128 scaled_grad = f128::div_scalar(grad, node.scalar_data);
            m_pairs[node.lhs_idx].gradients =
              f128::add(m_pairs[node.lhs_idx].gradients, scaled_grad);
            break;
        }
        case OpType::ScalarDivPair: {
            f128 grad                       = m_pairs[node.output_idx].gradients;
            f128 l                          = m_pairs[node.lhs_idx].values;
            f128 l_sq                       = f128::mul(l, l);
            f128 neg_s_over_sq              = f128::neg(f128::scalar_div(node.scalar_data, l_sq));
            f128 update                     = f128::mul(neg_s_over_sq, grad);
            m_pairs[node.lhs_idx].gradients = f128::add(m_pairs[node.lhs_idx].gradients, update);
            break;
        }

        // --- Pair Value ---
        case OpType::PairMulValue:
        case OpType::ValueMulPair: {
            // out = p * v
            f128 grad_out = m_pairs[node.output_idx].gradients;
            f128 p        = m_pairs[node.lhs_idx].values;
            f64  v        = m_values[node.rhs_idx].value;

            // d/dp = v
            f128 grad_p                     = f128::mul_scalar(grad_out, v);
            m_pairs[node.lhs_idx].gradients = f128::add(m_pairs[node.lhs_idx].gradients, grad_p);

            // d/dv = p.first * grad.first + p.second * grad.second
            f128 contrib = f128::mul(p, grad_out);
            m_values[node.rhs_idx].gradient += contrib.first() + contrib.second();
            break;
        }
        case OpType::PairDivValue: {
            // out = p / v
            f128 grad_out = m_pairs[node.output_idx].gradients;
            f128 p        = m_pairs[node.lhs_idx].values;
            f64  v        = m_values[node.rhs_idx].value;

            f128 grad_p                     = f128::div_scalar(grad_out, v);
            m_pairs[node.lhs_idx].gradients = f128::add(m_pairs[node.lhs_idx].gradients, grad_p);

            // d/dv = -p/v^2 * grad
            f128 num       = f128::mul(p, grad_out);
            f64  sum_contr = num.first() + num.second();
            m_values[node.rhs_idx].gradient += -sum_contr / (v * v);
            break;
        }
        case OpType::ValueDivPair: {
            // out = v / p
            f128 grad_out = m_pairs[node.output_idx].gradients;
            f128 p        = m_pairs[node.lhs_idx].values;
            f64  v        = m_values[node.rhs_idx].value;

            // d/dp = -v/p^2
            f128 p_sq                       = f128::mul(p, p);
            f128 neg_v_sq                   = f128::neg(f128::scalar_div(v, p_sq));
            f128 grad_p                     = f128::mul(neg_v_sq, grad_out);
            m_pairs[node.lhs_idx].gradients = f128::add(m_pairs[node.lhs_idx].gradients, grad_p);

            // d/dv = 1/p
            f128 v_contr = f128::div(grad_out, p);
            m_values[node.rhs_idx].gradient += v_contr.first() + v_contr.second();
            break;
        }

        // --- Phase ---
        case OpType::Phase: {
            f64 grad  = m_values[node.output_idx].gradient;
            f64 alpha = node.scalar_data;
            // d/d_first = alpha, d/d_second = 1-alpha
            f128 grad_upd                   = f128::make(alpha * grad, (1.0 - alpha) * grad);
            m_pairs[node.lhs_idx].gradients = f128::add(m_pairs[node.lhs_idx].gradients, grad_upd);
            break;
        }
        default:
            break;
        }
    }
}

void Graph::cleanup() {
    // Keep parameters, clear the rest
    m_values.reset_to(m_global_param_count);
    m_pairs.reset_to(m_global_pair_count);
    m_tape.clear();
}

void Graph::zero_grad() {
    for (usize i = 0; i < m_global_param_count; ++i) {
        m_values[i].gradient = 0.0;
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        m_pairs[i].gradients = f128::zero();
    }
}

void Graph::copy_parameter_values(const Parameters& source) {
    if (source.parameters.size() != m_global_param_count
        || source.pair_parameters.size() != m_global_pair_count) {
        std::cerr << "Graph parameter count desync!" << std::endl;
        std::terminate();
    }
    for (usize i = 0; i < m_global_param_count; ++i) {
        m_values[i].value = source.parameters[i];
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        m_pairs[i].values = source.pair_parameters[i];
    }
}

Parameters Graph::get_all_parameter_values() const {
    Parameters p;
    p.parameters.reserve(m_global_param_count);
    p.pair_parameters.reserve(m_global_pair_count);
    for (usize i = 0; i < m_global_param_count; ++i) {
        p.parameters.push_back(m_values[i].value);
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        p.pair_parameters.push_back(m_pairs[i].values);
    }
    return p;
}

Parameters Graph::get_all_parameter_gradients() const {
    Parameters p;
    p.parameters.reserve(m_global_param_count);
    p.pair_parameters.reserve(m_global_pair_count);
    for (usize i = 0; i < m_global_param_count; ++i) {
        p.parameters.push_back(m_values[i].gradient);
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        p.pair_parameters.push_back(m_pairs[i].gradients);
    }
    return p;
}

}  // namespace Clockwork::Autograd
