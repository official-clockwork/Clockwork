#pragma once

#include "graph.hpp"
#include "value.hpp"
#include <cmath>
#include <memory>
#include <vector>

namespace Clockwork {
namespace Autograd {

template<typename T = f64>
class SGD {
private:
    std::vector<ValuePtr<T>> m_value_params;
    std::vector<PairPtr<T>>  m_pair_params;

    f64 m_lr;
    f64 m_momentum;

    // Velocity vectors
    std::vector<T>               m_value_velocity;
    std::vector<std::pair<T, T>> m_pair_velocity;

public:
    SGD(f64 lr, f64 momentum = 0.9) :
        m_lr(lr),
        m_momentum(momentum) {
        auto graph     = Graph<T>::get();
        m_value_params = graph->get_parameters();
        m_pair_params  = graph->get_pair_parameters();

        m_value_velocity.resize(m_value_params.size(), 0);
        m_pair_velocity.resize(m_pair_params.size(), {0, 0});
    }

    void step() {
        // Update Value<T> parameters
        for (size_t i = 0; i < m_value_params.size(); ++i) {
            auto& param         = m_value_params[i];
            m_value_velocity[i] = m_momentum * m_value_velocity[i] - m_lr * param->get_gradient();
            param->change_value(m_value_velocity[i]);
        }

        // Update Pair<T> parameters
        for (size_t i = 0; i < m_pair_params.size(); ++i) {
            auto& param = m_pair_params[i];
            auto& vel   = m_pair_velocity[i];

            vel.first  = m_momentum * vel.first - m_lr * param->grad_first();
            vel.second = m_momentum * vel.second - m_lr * param->grad_second();

            param->m_first += vel.first;
            param->m_second += vel.second;
        }
    }

    void set_lr(T lr) {
        m_lr = lr;
    }
    T get_lr() const {
        return m_lr;
    }
};


template<typename T = f64>
class AdamW {
private:
    std::vector<ValuePtr<T>> m_value_params;
    std::vector<PairPtr<T>>  m_pair_params;

    f64 m_lr;
    f64 m_beta1;
    f64 m_beta2;
    f64 m_eps;
    f64 m_weight_decay;
    i64 m_t;

    std::vector<T> m_m;
    std::vector<T> m_v;

    std::vector<std::pair<T, T>> m_pair_m;
    std::vector<std::pair<T, T>> m_pair_v;

public:
    AdamW(
      f64 lr = 1e-3, f64 beta1 = 0.9, f64 beta2 = 0.999, f64 eps = 1e-8, f64 weight_decay = 0.01) :
        m_lr(lr),
        m_beta1(beta1),
        m_beta2(beta2),
        m_eps(eps),
        m_weight_decay(weight_decay),
        m_t(0) {
        auto graph     = Graph<T>::get();
        m_value_params = graph->get_parameters();
        m_pair_params  = graph->get_pair_parameters();

        m_m.resize(m_value_params.size(), 0);
        m_v.resize(m_value_params.size(), 0);

        m_pair_m.resize(m_pair_params.size(), {0, 0});
        m_pair_v.resize(m_pair_params.size(), {0, 0});
    }

    void step() {
        m_t += 1;

        // ---------------- Value<T> parameters ----------------
        for (size_t i = 0; i < m_value_params.size(); ++i) {
            auto& param = m_value_params[i];
            T     grad  = param->get_gradient();

            m_m[i] = m_beta1 * m_m[i] + (1 - m_beta1) * grad;
            m_v[i] = m_beta2 * m_v[i] + (1 - m_beta2) * grad * grad;

            T m_hat = m_m[i] / (1 - std::pow(m_beta1, m_t));
            T v_hat = m_v[i] / (1 - std::pow(m_beta2, m_t));

            T update = -m_lr * (m_hat / (std::sqrt(v_hat) + m_eps));
            update -= m_lr * m_weight_decay * param->get_value();

            param->change_value(update);
        }

        // ---------------- Pair<T> parameters ----------------
        for (size_t i = 0; i < m_pair_params.size(); ++i) {
            auto& param = m_pair_params[i];
            auto& m     = m_pair_m[i];
            auto& v     = m_pair_v[i];

            T grad_f = param->grad_first();
            T grad_s = param->grad_second();

            // Update biased first moment
            m.first  = m_beta1 * m.first + (1 - m_beta1) * grad_f;
            m.second = m_beta1 * m.second + (1 - m_beta1) * grad_s;

            // Update biased second moment
            v.first  = m_beta2 * v.first + (1 - m_beta2) * grad_f * grad_f;
            v.second = m_beta2 * v.second + (1 - m_beta2) * grad_s * grad_s;

            // Bias-corrected
            T m_hat_f = m.first / (1 - std::pow(m_beta1, m_t));
            T m_hat_s = m.second / (1 - std::pow(m_beta1, m_t));

            T v_hat_f = v.first / (1 - std::pow(m_beta2, m_t));
            T v_hat_s = v.second / (1 - std::pow(m_beta2, m_t));

            T update_f = -m_lr * (m_hat_f / (std::sqrt(v_hat_f) + m_eps))
                       - m_lr * m_weight_decay * param->first();
            T update_s = -m_lr * (m_hat_s / (std::sqrt(v_hat_s) + m_eps))
                       - m_lr * m_weight_decay * param->second();

            param->m_first += update_f;
            param->m_second += update_s;
        }
    }

    void set_lr(T lr) {
        m_lr = lr;
    }
    T get_lr() const {
        return m_lr;
    }
};

}  // namespace Autograd
}  // namespace Clockwork
