#pragma once

#include "value.hpp"
#include <memory>
#include <vector>

namespace Clockwork {
namespace Autograd {

template<typename T>
class Backwardable;
template<typename T>
using BackwardablePtr = std::shared_ptr<Backwardable<T>>;

template<typename TuneType, typename T>
class SmartBackwardable;
template<typename TuneType, typename T>
using SmartBackwardablePtr = std::shared_ptr<SmartBackwardable<TuneType, T>>;

template<typename T>
class Value;
template<typename T>
using ValuePtr = std::shared_ptr<Value<T>>;

template<typename T>
class Pair;
template<typename T>
using PairPtr = std::shared_ptr<Pair<T>>;

template<typename T = f64>
class Graph {
private:
    // Tunable parameters
    std::vector<ValuePtr<T>> m_parameters;
    std::vector<PairPtr<T>>  m_pair_parameters;

    // All backwardable nodes in insertion order (intermediates + outputs + parameters)
    std::vector<BackwardablePtr<T>> m_backwardables;

    Graph() = default;

public:
    static std::shared_ptr<Graph<T>> get() {
        static std::shared_ptr<Graph<T>> instance(new Graph<T>());
        return instance;
    }

    // ------------------ Registration ------------------
    void register_param(const ValuePtr<T>& param) {
        m_parameters.push_back(param);
        m_backwardables.push_back(std::static_pointer_cast<Backwardable<T>>(param));
    }

    void register_param(const PairPtr<T>& param) {
        m_pair_parameters.push_back(param);
        m_backwardables.push_back(std::static_pointer_cast<Backwardable<T>>(param));
    }

    void register_value(const BackwardablePtr<T>& node) {
        m_backwardables.push_back(node);
    }

    void register_value(const ValuePtr<T>& node) {
        m_backwardables.push_back(std::static_pointer_cast<Backwardable<T>>(node));
    }

    void register_value(const PairPtr<T>& node) {
        m_backwardables.push_back(std::static_pointer_cast<Backwardable<T>>(node));
    }

    // ------------------ Backward Pass ------------------
    void backward() {
        if (m_backwardables.empty()) {
            return;
        }

        // Initialize gradient on last node (loss node)
        auto last        = std::static_pointer_cast<Value<T>>(m_backwardables.back());
        last->m_gradient = static_cast<T>(1);

        // Reverse pass
        for (auto it = m_backwardables.rbegin(); it != m_backwardables.rend(); ++it) {
            (*it)->backward();
        }
    }

    // ------------------ Cleanup ------------------
    void cleanup() {
        for (auto& param : m_parameters) {
            param->m_gradient = static_cast<T>(0);
        }
        for (auto& param : m_pair_parameters) {
            param->m_grad_first  = static_cast<T>(0);
            param->m_grad_second = static_cast<T>(0);
        }

        m_backwardables.clear();
    }

    // ------------------ Accessors ------------------
    const std::vector<ValuePtr<T>>& get_parameters() const {
        return m_parameters;
    }
    const std::vector<PairPtr<T>>& get_pair_parameters() const {
        return m_pair_parameters;
    }
};

}  // namespace Autograd
}  // namespace Clockwork
