#pragma once

#include "value.hpp"
#include <memory>
#include <vector>

namespace Clockwork {
namespace Autograd {

template<typename T = f64>
class Graph {
private:
    std::vector<ValuePtr<T>> m_parameters;
    std::vector<ValuePtr<T>> m_values;

    Graph() = default;  // Private constructor

public:
    static std::shared_ptr<Graph<T>> get() {
        static std::shared_ptr<Graph<T>> instance = std::make_shared<Graph<T>>();
        return instance;
    }

    void register_param(const ValuePtr<T>& param) {
        m_parameters.push_back(param);
        m_values.push_back(param);
    }

    void register_value(const ValuePtr<T>& value) {
        m_values.push_back(value);
    }

    void backward(const ValuePtr<T>& root = nullptr) {
        if (root) {
            root->m_gradient = static_cast<T>(1);
            root->backward();
        } else {
            // Last registered value MUST be output of loss (or sum of losses over the output). TODO: maybe add an efficient reduction rather than having batch_sizes nodes?
            m_values.back()->m_gradient = static_cast<T>(1);
            for (auto it = m_values.rbegin(); it != m_values.rend(); ++it) {
                if ((*it)->m_backward_func) {
                    (*it)->m_backward_func();
                }
            }
        }
    }

    void cleanup() {
        for (auto& param : m_parameters) {
            param->m_gradient = static_cast<T>(0);
        }
        m_values.clear();  // Drop intermediates and outputs
    }

    const std::vector<ValuePtr<T>>& get_parameters() const {
        return m_parameters;
    }

    void clear_parameters() {
        m_parameters.clear();
    }
};

}  // namespace Autograd
}  // namespace Clockwork
