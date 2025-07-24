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
            m_velocity[i] = m_momentum * m_velocity[i] - m_lr * param->m_gradient;

            // Update parameter: param += v
            param->m_value += m_velocity[i];
        }
    }
};
}
}
