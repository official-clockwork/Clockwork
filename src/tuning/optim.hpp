#pragma once

#include "graph.hpp"
#include "value.hpp"
#include <memory>
#include <vector>


namespace Clockwork {
namespace Autograd {

template<typename T = f64>
class SGD {
private:
    std::vector<ValuePtr<T>> m_parameters;
    f64                      m_lr;
    f64                      m_momentum;

    // Velocity vectors for momentum, same size as parameters
    std::vector<T> m_velocity;

public:
    SGD(std::vector<ValuePtr<T>> parameters, f64 lr, f64 momentum = 0.9) :
        m_parameters(std::move(parameters)),
        m_lr(lr),
        m_momentum(momentum),
        m_velocity(m_parameters.size(), 0) {
    }

    void step() {
        for (size_t i = 0; i < m_parameters.size(); ++i) {
            auto& param = m_parameters[i];
            // Update velocity: v = momentum * v - lr * grad
            m_velocity[i] = m_momentum * m_velocity[i] - m_lr * param->get_gradient();

            // Update parameter: param += v
            param->change_value(m_velocity[i]);
        }
    }

    void set_lr(T lr) {
        m_lr = lr;
    }

    T get_lr() {
        return m_lr;
    }
};

template<typename T = f64>
class AdamW {
private:
    std::vector<ValuePtr<T>> m_parameters;
    f64                      m_lr;
    f64                      m_beta1;
    f64                      m_beta2;
    f64                      m_eps;
    f64                      m_weight_decay;
    i64                      m_t;

    std::vector<T> m_m;  // First moment vector
    std::vector<T> m_v;  // Second moment vector

public:
    AdamW(std::vector<ValuePtr<T>> parameters,
          f64                      lr           = 1e-3,
          f64                      beta1        = 0.9,
          f64                      beta2        = 0.999,
          f64                      eps          = 1e-8,
          f64                      weight_decay = 0.01) :
        m_parameters(std::move(parameters)),
        m_lr(lr),
        m_beta1(beta1),
        m_beta2(beta2),
        m_eps(eps),
        m_weight_decay(weight_decay),
        m_t(0),
        m_m(m_parameters.size(), 0),
        m_v(m_parameters.size(), 0) {
    }

    void step() {
        m_t += 1;

        for (size_t i = 0; i < m_parameters.size(); ++i) {
            auto& param = m_parameters[i];
            T     grad  = param->get_gradient();

            // Update biased first moment estimate
            m_m[i] = m_beta1 * m_m[i] + (1 - m_beta1) * grad;

            // Update biased second raw moment estimate
            m_v[i] = m_beta2 * m_v[i] + (1 - m_beta2) * (grad * grad);

            // Compute bias-corrected first and second moments
            T m_hat = m_m[i] / (1 - std::pow(m_beta1, m_t));
            T v_hat = m_v[i] / (1 - std::pow(m_beta2, m_t));

            // Compute update
            T update = -m_lr * (m_hat / (std::sqrt(v_hat) + m_eps));

            // Apply decoupled weight decay
            update -= m_lr * m_weight_decay * param->get_value();

            // Update parameter
            param->change_value(update);
        }
    }

    void set_lr(T lr) {
        m_lr = lr;
    }

    T get_lr() {
        return m_lr;
    }
};
}
}
