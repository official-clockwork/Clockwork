#include "tuning/graph.hpp"
#include "tuning/globals.hpp"
#include "util/types.hpp"
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

ValueHandle Graph::create_value(f64 data) {
    return ValueHandle(m_values.alloc(data, 0.0));
}

PairHandle Graph::create_pair(f64x2 data) {
    return PairHandle(m_pairs.alloc(data, f64x2::zero()));
}

// Recording

ValueHandle Graph::record_op(OpType op, ValueHandle lhs, ValueHandle rhs) {
    ValueHandle out = m_values.next_handle();
    f64         l   = m_values.val(lhs.index);
    f64         r   = m_values.val(rhs.index);
    f64         res = 0.0;

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

    m_values.alloc(res, 0.0);

    m_tape.push_back(Node::make_binary(op, out.index, lhs.index, rhs.index));

    return out;
}

ValueHandle Graph::record_op(OpType op, ValueHandle lhs, f64 scalar) {
    ValueHandle out = m_values.next_handle();
    f64         l   = m_values.val(lhs.index);
    f64         res = 0.0;

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

    m_values.alloc(res, 0.0);

    m_tape.push_back(Node::make_scalar(op, out.index, lhs.index, scalar));

    return out;
}

PairHandle Graph::record_pair_op(OpType op, PairHandle lhs, PairHandle rhs) {
    PairHandle out = m_pairs.next_handle();
    f64x2      l   = m_pairs.val(lhs.index);
    f64x2      r   = m_pairs.val(rhs.index);
    f64x2      res = f64x2::zero();

    switch (op) {
    case OpType::PairAdd:
        res = f64x2::add(l, r);
        break;
    case OpType::PairSub:
        res = f64x2::sub(l, r);
        break;
    default:
        break;
    }

    m_pairs.alloc(res, f64x2::zero());

    m_tape.push_back(Node::make_binary(op, out.index, lhs.index, rhs.index));

    return out;
}

PairHandle Graph::record_pair_scalar(OpType op, PairHandle lhs, f64 scalar) {
    PairHandle out = m_pairs.next_handle();
    f64x2      l   = m_pairs.val(lhs.index);
    f64x2      res = f64x2::zero();

    switch (op) {
    case OpType::PairNeg:
        res = f64x2::neg(l);
        break;
    case OpType::PairMulScalar:
        res = f64x2::mul_scalar(l, scalar);
        break;
    case OpType::PairDivScalar:
        res = f64x2::div_scalar(l, scalar);
        break;
    case OpType::ScalarDivPair:
        res = f64x2::scalar_div(scalar, l);
        break;
    default:
        break;
    }

    m_pairs.alloc(res, f64x2::zero());

    m_tape.push_back(Node::make_scalar(op, out.index, lhs.index, scalar));

    return out;
}

PairHandle Graph::record_pair_value(OpType op, PairHandle lhs, ValueHandle rhs) {
    PairHandle out      = m_pairs.next_handle();
    f64x2      pair_val = m_pairs.val(lhs.index);
    f64        v        = m_values.val(rhs.index);
    f64x2      res      = f64x2::zero();

    switch (op) {
    case OpType::PairMulValue:
    case OpType::ValueMulPair:
        res = f64x2::mul_scalar(pair_val, v);
        break;
    case OpType::PairDivValue:
        res = f64x2::div_scalar(pair_val, v);
        break;
    case OpType::ValueDivPair:
        res = f64x2::scalar_div(v, pair_val);
        break;
    default:
        break;
    }

    m_pairs.alloc(res, f64x2::zero());

    m_tape.push_back(Node::make_binary(op, out.index, lhs.index, rhs.index));

    return out;
}

PairHandle Graph::record_pair_unary(OpType op, PairHandle input) {
    PairHandle out    = m_pairs.next_handle();
    f64x2      in_val = m_pairs.val(input.index);
    f64x2      res    = f64x2::zero();

    switch (op) {
    case OpType::PairSigmoid: {
        // Apply sigmoid to each component: 1 / (1 + exp(-x))
        f64 mg     = in_val.first();
        f64 eg     = in_val.second();
        f64 sig_mg = 1.0 / (1.0 + std::exp(-mg));
        f64 sig_eg = 1.0 / (1.0 + std::exp(-eg));
        res        = f64x2::make(sig_mg, sig_eg);
        break;
    }
    default:
        break;
    }

    m_pairs.alloc(res, f64x2::zero());

    m_tape.push_back(Node::make_scalar(op, out.index, input.index, 0.0));

    return out;
}

PairHandle Graph::record_pair_value(OpType op, PairHandle lhs, PairHandle rhs) {
    PairHandle out      = m_pairs.next_handle();
    f64x2      l        = m_pairs.val(lhs.index);
    f64x2      r        = m_pairs.val(rhs.index);
    f64x2      res      = f64x2::zero();

    switch (op) {
    case OpType::PairMulPair:
        res = f64x2::mul(l, r);
        break;
    default:
        break;
    }

    m_pairs.alloc(res, f64x2::zero());

    m_tape.push_back(Node::make_binary(op, out.index, lhs.index, rhs.index));

    return out;
}

ValueHandle Graph::record_phase(PairHandle lhs, f64 alpha) {
    ValueHandle out      = m_values.next_handle();
    f64x2       pair_val = m_pairs.val(lhs.index);
    f64         val      = alpha * pair_val.first() + (1.0 - alpha) * pair_val.second();

    m_values.alloc(val, 0.0);

    m_tape.push_back(Node::make_scalar(OpType::Phase, out.index, lhs.index, alpha));

    return out;
}

void Graph::backward() {
    if (m_tape.empty()) {
        return;
    }

    // Initialize gradient of last output to 1
    // (This assumes the final output is always a ValueHandle)
    m_values.grad(m_tape.back().out()) = 1.0;

    f64*   vals       = m_values.values_data();
    f64*   grads      = m_values.gradients_data();
    f64x2* pair_vals  = m_pairs.values_data();
    f64x2* pair_grads = m_pairs.gradients_data();

    for (auto it = m_tape.rbegin(); it != m_tape.rend(); ++it) {
        const Node& node = *it;

        const u32 out_idx = node.out();

        switch (node.type) {
            // Value-Binary

        case OpType::Add: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] += grad_out;
            grads[node.rhs()] += grad_out;
            break;
        }
        case OpType::Sub: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] += grad_out;
            grads[node.rhs()] -= grad_out;
            break;
        }
        case OpType::Mul: {
            const f64 grad_out = grads[out_idx];
            f64       l        = vals[node.lhs()];
            f64       r        = vals[node.rhs()];
            grads[node.lhs()] += r * grad_out;
            grads[node.rhs()] += l * grad_out;
            break;
        }
        case OpType::Div: {
            const f64 grad_out = grads[out_idx];
            f64       l        = vals[node.lhs()];
            f64       r        = vals[node.rhs()];
            grads[node.lhs()] += grad_out / r;
            grads[node.rhs()] += -l * grad_out / (r * r);
            break;
        }
        case OpType::Pow: {
            const f64 grad_out = grads[out_idx];
            f64       base     = vals[node.lhs()];
            f64       exp      = vals[node.rhs()];
            grads[node.lhs()] += exp * std::pow(base, exp - 1) * grad_out;
            grads[node.rhs()] += std::pow(base, exp) * std::log(base) * grad_out;
            break;
        }

        // Value-Scalar
        case OpType::Exp: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] += vals[out_idx] * grad_out;
            break;
        }
        case OpType::Log: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] += grad_out / vals[node.lhs()];
            break;
        }
        case OpType::Sigmoid: {
            const f64 grad_out = grads[out_idx];
            f64       s        = vals[out_idx];
            grads[node.lhs()] += s * (1.0 - s) * grad_out;
            break;
        }
        case OpType::Neg: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] -= grad_out;
            break;
        }
        case OpType::PowConst: {
            const f64 grad_out = grads[out_idx];
            f64       l        = vals[node.lhs()];
            f64       exp      = node.scalar();
            grads[node.lhs()] += exp * std::pow(l, exp - 1.0) * grad_out;
            break;
        }
        case OpType::AddScalar:
        case OpType::ValSubScalar: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] += grad_out;
            break;
        }

        case OpType::SubScalarVal: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] -= grad_out;
            break;
        }
        case OpType::MulScalar: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] += node.scalar() * grad_out;
            break;
        }
        case OpType::ValDivScalar: {
            const f64 grad_out = grads[out_idx];
            grads[node.lhs()] += grad_out / node.scalar();
            break;
        }
        case OpType::DivScalarVal: {
            const f64 grad_out = grads[out_idx];
            f64       l        = vals[node.lhs()];
            grads[node.lhs()] += -node.scalar() * grad_out / (l * l);
            break;
        }
        case OpType::Phase: {
            const f64 grad_out = grads[out_idx];
            f64x2 update = f64x2::make(node.scalar() * grad_out, (1.0 - node.scalar()) * grad_out);
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], update);
            break;
        }

        case OpType::PairSigmoid: { 
            const f64x2 grad_out = pair_grads[out_idx];

            // sigmoid output values already computed in forward pass
            f64x2 sigmoid_out = pair_vals[out_idx];

            f64 sig_mg = sigmoid_out.first();
            f64 sig_eg = sigmoid_out.second();

            f64 grad_mg = sig_mg * (1.0 - sig_mg);
            f64 grad_eg = sig_eg * (1.0 - sig_eg);

            f64x2 local_grad = f64x2::make(grad_mg, grad_eg);
            f64x2 update     = f64x2::mul(local_grad, grad_out);

            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], update);
            break;
        }

        case OpType::PairAdd: {
            const f64x2 grad_out   = pair_grads[out_idx];
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], grad_out);
            pair_grads[node.rhs()] = f64x2::add(pair_grads[node.rhs()], grad_out);
            break;
        }
        case OpType::PairSub: {
            const f64x2 grad_out   = pair_grads[out_idx];
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], grad_out);
            pair_grads[node.rhs()] = f64x2::sub(pair_grads[node.rhs()], grad_out);
            break;
        }
        case OpType::PairNeg: {
            const f64x2 grad_out   = pair_grads[out_idx];
            pair_grads[node.lhs()] = f64x2::sub(pair_grads[node.lhs()], grad_out);
            break;
        }
        case OpType::PairMulScalar: {
            const f64x2 grad_out   = pair_grads[out_idx];
            f64x2       scaled     = f64x2::mul_scalar(grad_out, node.scalar());
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], scaled);
            break;
        }
        case OpType::PairDivScalar: {
            const f64x2 grad_out   = pair_grads[out_idx];
            f64x2       scaled     = f64x2::div_scalar(grad_out, node.scalar());
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], scaled);
            break;
        }
        case OpType::ScalarDivPair: {
            const f64x2 grad_out   = pair_grads[out_idx];
            f64x2       val        = pair_vals[node.lhs()];
            f64x2       grad       = f64x2::scalar_div(-node.scalar(), f64x2::mul(val, val));
            f64x2       update     = f64x2::mul(grad, grad_out);
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], update);
            break;
        }
        case OpType::PairMulValue:
        case OpType::ValueMulPair: {
            const f64x2 grad_out = pair_grads[out_idx];
            f64         val_rhs  = vals[node.rhs()];
            f64x2       val_lhs  = pair_vals[node.lhs()];

            f64x2 grad_pair        = f64x2::mul_scalar(grad_out, val_rhs);
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], grad_pair);

            f64 contrib = grad_out.first() * val_lhs.first() + grad_out.second() * val_lhs.second();
            grads[node.rhs()] += contrib;
            break;
        }
        case OpType::PairDivValue: {
            const f64x2 grad_out = pair_grads[out_idx];
            f64         val_rhs  = vals[node.rhs()];
            f64x2       val_lhs  = pair_vals[node.lhs()];

            f64x2 grad_pair        = f64x2::div_scalar(grad_out, val_rhs);
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], grad_pair);

            f64 num = grad_out.first() * val_lhs.first() + grad_out.second() * val_lhs.second();
            grads[node.rhs()] += -num / (val_rhs * val_rhs);
            break;
        }
        case OpType::ValueDivPair: {
            const f64x2 grad_out = pair_grads[out_idx];
            f64         val_rhs  = vals[node.rhs()];
            f64x2       val_lhs  = pair_vals[node.lhs()];

            f64x2 grad_pair        = f64x2::scalar_div(-val_rhs, f64x2::mul(val_lhs, val_lhs));
            f64x2 update           = f64x2::mul(grad_pair, grad_out);
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], update);

            f64x2 recip = f64x2::scalar_div(1.0, val_lhs);
            grads[node.rhs()] +=
              grad_out.first() * recip.first() + grad_out.second() * recip.second();
            break;
        }
        case OpType::PairMulPair: {
            const f64x2 grad_out = pair_grads[out_idx];
            f64x2       l        = pair_vals[node.lhs()];
            f64x2       r        = pair_vals[node.rhs()];

            f64x2 grad_lhs        = f64x2::mul(grad_out, r);
            f64x2 grad_rhs        = f64x2::mul(grad_out, l);
            pair_grads[node.lhs()] = f64x2::add(pair_grads[node.lhs()], grad_lhs);
            pair_grads[node.rhs()] = f64x2::add(pair_grads[node.rhs()], grad_rhs);
            break;
        }

        default:
            unreachable();
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
        m_values.grad(static_cast<u32>(i)) = 0.0;
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        m_pairs.grad(static_cast<u32>(i)) = f64x2::zero();
    }
}

void Graph::copy_parameter_values(const Parameters& source) {
    if (source.parameters.size() != m_global_param_count
        || source.pair_parameters.size() != m_global_pair_count) {
        std::cerr << "Graph parameter count desync!" << std::endl;
        std::terminate();
    }
    for (usize i = 0; i < m_global_param_count; ++i) {
        m_values.val(static_cast<u32>(i)) = source.parameters[i];
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        m_pairs.val(static_cast<u32>(i)) = source.pair_parameters[i];
    }
}

Parameters Graph::get_all_parameter_values() const {
    Parameters p;
    p.parameters.reserve(m_global_param_count);
    p.pair_parameters.reserve(m_global_pair_count);
    for (usize i = 0; i < m_global_param_count; ++i) {
        p.parameters.push_back(m_values.val(static_cast<u32>(i)));
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        p.pair_parameters.push_back(m_pairs.val(static_cast<u32>(i)));
    }
    return p;
}

Parameters Graph::get_all_parameter_gradients() const {
    Parameters p;
    p.parameters.reserve(m_global_param_count);
    p.pair_parameters.reserve(m_global_pair_count);
    for (usize i = 0; i < m_global_param_count; ++i) {
        p.parameters.push_back(m_values.grad(static_cast<u32>(i)));
    }
    for (usize i = 0; i < m_global_pair_count; ++i) {
        p.pair_parameters.push_back(m_pairs.grad(static_cast<u32>(i)));
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
    m_pairs.val(idx) = v;
}

void Graph::zero_pair_grad(u32 idx) {
    m_pairs.grad(idx) = f64x2::zero();
}

}  // namespace Clockwork::Autograd
