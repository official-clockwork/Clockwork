#pragma once

#include "value.hpp"
#include <memory>
#include <vector>

namespace Clockwork {
namespace Autograd {

class Backwardable;
using BackwardablePtr = std::shared_ptr<Backwardable>;

template<typename TuneType>
class SmartBackwardable;
template<typename TuneType>
using SmartBackwardablePtr = std::shared_ptr<SmartBackwardable<TuneType>>;

class Value;
using ValuePtr = std::shared_ptr<Value>;

class Pair;
using PairPtr = std::shared_ptr<Pair>;

class Graph {
private:
    // Tunable parameters
    std::vector<ValuePtr> m_parameters;
    std::vector<PairPtr>  m_pair_parameters;

    // All backwardable nodes in insertion order (intermediates + outputs + parameters)
    std::vector<BackwardablePtr> m_backwardables;

    Graph() = default;

public:
    static std::shared_ptr<Graph> get() {
        static std::shared_ptr<Graph> instance(new Graph());
        return instance;
    }

    // ------------------ Registration ------------------
    void register_param(const ValuePtr& param) {
        m_parameters.push_back(param);
        m_backwardables.push_back(std::static_pointer_cast<Backwardable>(param));
    }

    void register_param(const PairPtr& param) {
        m_pair_parameters.push_back(param);
        m_backwardables.push_back(std::static_pointer_cast<Backwardable>(param));
    }

    void register_value(const BackwardablePtr& node) {
        m_backwardables.push_back(node);
    }

    void register_value(const ValuePtr& node) {
        m_backwardables.push_back(std::static_pointer_cast<Backwardable>(node));
    }

    void register_value(const PairPtr& node) {
        m_backwardables.push_back(std::static_pointer_cast<Backwardable>(node));
    }

    // ------------------ Backward Pass ------------------
    void backward() {
        if (m_backwardables.empty()) {
            return;
        }

        // Initialize gradient on last node (loss node)
        auto last        = std::static_pointer_cast<Value>(m_backwardables.back());
        last->m_gradient = static_cast<f64>(1);

        // Reverse pass
        for (auto it = m_backwardables.rbegin(); it != m_backwardables.rend(); ++it) {
            (*it)->backward();
        }
    }

    // ------------------ Cleanup ------------------
    void cleanup() {
        for (auto& param : m_parameters) {
            param->zero_grad();
        }
        for (auto& param : m_pair_parameters) {
            param->zero_grad();
        }

        m_backwardables.clear();
    }

    // ------------------ Reset ------------------
    void init_zeros() {
        for (auto& param : m_parameters) {
            param->set_value(0.0);
        }
        for (auto& param : m_pair_parameters) {
            param->set_values(0.0, 0.0);
        }
        cleanup();
    }

    // ------------------ Accessors ------------------
    const std::vector<ValuePtr>& get_parameters() const {
        return m_parameters;
    }
    const std::vector<PairPtr>& get_pair_parameters() const {
        return m_pair_parameters;
    }
};

}  // namespace Autograd
}  // namespace Clockwork
