#pragma once

#include "tuning/globals.hpp"
#include "tuning/info.hpp"
#include "util/types.hpp"
#include "util/vec/sse2.hpp"

#include <cmath>
#include <vector>

namespace Clockwork::Autograd {

class SGD {
private:
    ParameterCountInfo m_counts;
    f64                m_lr;
    f64                m_momentum;

    std::vector<f64>   m_value_velocity;
    std::vector<f64x2> m_pair_velocity;

public:
    explicit SGD(ParameterCountInfo counts, f64 lr, f64 momentum = 0.9) :
        m_counts(counts),
        m_lr(lr),
        m_momentum(momentum) {
        m_value_velocity.resize(m_counts.parameter_count, 0.0);
        m_pair_velocity.resize(m_counts.pair_parameter_count, f64x2::zero());
    }

    void step(Parameters& values, const Parameters& gradients) {
        const auto& globals = Globals::get();

        // ---- Value parameters ----
        for (size_t i = 0; i < m_counts.parameter_count; ++i) {
            if (globals.is_parameter_constant(i)) {
                continue;
            }

            auto& p_value = values.parameters[i];
            auto& p_grad  = gradients.parameters[i];
            auto& v       = m_value_velocity[i];

            v = m_momentum * v - m_lr * p_grad;
            p_value += v;
        }

        // ---- Pair parameters ----
        for (size_t i = 0; i < m_counts.pair_parameter_count; ++i) {
            if (globals.is_pair_parameter_constant(i)) {
                continue;
            }

            auto& p_value = values.pair_parameters[i];
            auto& p_grad  = gradients.pair_parameters[i];
            auto& v       = m_pair_velocity[i];

            const f64x2 lr_grad     = f64x2::mul_scalar(p_grad, m_lr);
            const f64x2 mom_v       = f64x2::mul_scalar(v, m_momentum);
            const f64x2 neg_lr_grad = f64x2::neg(lr_grad);
            v                       = f64x2::add(mom_v, neg_lr_grad);
            p_value                 = f64x2::add(p_value, v);
        }
    }

    void set_lr(f64 lr) {
        m_lr = lr;
    }
    f64 get_lr() const {
        return m_lr;
    }
};


class AdamW {
private:
    ParameterCountInfo m_counts;
    f64                m_lr;
    f64                m_beta1;
    f64                m_beta2;
    f64                m_eps;
    f64                m_weight_decay;
    long long          m_t;

    std::vector<f64>   m_m;
    std::vector<f64>   m_v;
    std::vector<f64x2> m_pair_m;
    std::vector<f64x2> m_pair_v;

public:
    explicit AdamW(ParameterCountInfo counts,
                   f64                lr           = 1e-3,
                   f64                beta1        = 0.9,
                   f64                beta2        = 0.999,
                   f64                eps          = 1e-8,
                   f64                weight_decay = 0.01) :
        m_counts(counts),
        m_lr(lr),
        m_beta1(beta1),
        m_beta2(beta2),
        m_eps(eps),
        m_weight_decay(weight_decay),
        m_t(0) {
        m_m.resize(m_counts.parameter_count, 0.0);
        m_v.resize(m_counts.parameter_count, 0.0);
        m_pair_m.resize(m_counts.pair_parameter_count, f64x2::zero());
        m_pair_v.resize(m_counts.pair_parameter_count, f64x2::zero());
    }

    void step(Parameters& values, const Parameters& gradients) {
        m_t += 1;
        const auto& globals = Globals::get();

        const f64 b1t      = std::pow(m_beta1, static_cast<f64>(m_t));
        const f64 b2t      = std::pow(m_beta2, static_cast<f64>(m_t));
        const f64 inv1mb1t = 1.0 / (1.0 - b1t);
        const f64 inv1mb2t = 1.0 / (1.0 - b2t);

        // ---------------- Value parameters ----------------
        for (size_t i = 0; i < m_counts.parameter_count; ++i) {
            if (globals.is_parameter_constant(i)) {
                continue;
            }

            auto& p = values.parameters[i];
            auto& g = gradients.parameters[i];

            m_m[i] = m_beta1 * m_m[i] + (1.0 - m_beta1) * g;
            m_v[i] = m_beta2 * m_v[i] + (1.0 - m_beta2) * g * g;

            const f64 m_hat               = m_m[i] * inv1mb1t;
            const f64 v_hat               = m_v[i] * inv1mb2t;
            const f64 adam_update         = m_lr * m_hat / (std::sqrt(v_hat) + m_eps);
            const f64 weight_decay_update = m_lr * m_weight_decay * p;

            p += -(adam_update + weight_decay_update);
        }

        // ---------------- Pair parameters ----------------
        for (size_t i = 0; i < m_counts.pair_parameter_count; ++i) {
            if (globals.is_pair_parameter_constant(i)) {
                continue;
            }

            auto& p = values.pair_parameters[i];
            auto& g = gradients.pair_parameters[i];
            auto& m = m_pair_m[i];
            auto& v = m_pair_v[i];

            const f64x2 g2 = f64x2::mul(g, g);

            const f64x2 m_scaled = f64x2::mul_scalar(m, m_beta1);
            const f64x2 g_scaled = f64x2::mul_scalar(g, (1.0 - m_beta1));
            m                    = f64x2::add(m_scaled, g_scaled);

            const f64x2 v_scaled  = f64x2::mul_scalar(v, m_beta2);
            const f64x2 g2_scaled = f64x2::mul_scalar(g2, (1.0 - m_beta2));
            v                     = f64x2::add(v_scaled, g2_scaled);

            const f64x2 m_hat = f64x2::mul_scalar(m, inv1mb1t);
            const f64x2 v_hat = f64x2::mul_scalar(v, inv1mb2t);

            const f64 adam_upd_f = m_lr * m_hat.first() / (std::sqrt(v_hat.first()) + m_eps);
            const f64 adam_upd_s = m_lr * m_hat.second() / (std::sqrt(v_hat.second()) + m_eps);

            const f64 decay_upd_f = m_lr * m_weight_decay * p.first();
            const f64 decay_upd_s = m_lr * m_weight_decay * p.second();

            const f64 total_upd_f = -(adam_upd_f + decay_upd_f);
            const f64 total_upd_s = -(adam_upd_s + decay_upd_s);

            p = f64x2::add(p, f64x2::make(total_upd_f, total_upd_s));
        }
    }

    void set_lr(f64 lr) {
        m_lr = lr;
    }
    f64 get_lr() const {
        return m_lr;
    }
};

}  // namespace Clockwork::Autograd
