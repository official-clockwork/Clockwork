#pragma once

#include "tuning/info.hpp"
#include "tuning/value.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <vector>

namespace Clockwork::Autograd {

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

    Graph();

    void register_param(const ValuePtr& param) {
        m_parameters.push_back(param);
    }

    void register_param(const PairPtr& param) {
        m_pair_parameters.push_back(param);
    }

public:
    static Graph& get() {
        thread_local Graph instance;
        return instance;
    }

    // ------------------ Registration ------------------
    void register_value(const BackwardablePtr& node) {
        m_backwardables.push_back(node);
    }

    void register_value(const ValuePtr& node) {
        m_backwardables.push_back(std::static_pointer_cast<Backwardable>(node));
    }

    void register_value(const PairPtr& node) {
        m_backwardables.push_back(std::static_pointer_cast<Backwardable>(node));
    }

    // ------------------- Copy Values -------------------
    void copy_parameter_values(const Parameters& source) {
        if (source.parameters.size() != m_parameters.size()
            || source.pair_parameters.size() != m_pair_parameters.size()) {
            std::cerr << "Graph parameters count have desynced" << std::endl;
            std::terminate();
        }

        for (usize i = 0; i < m_parameters.size(); i++) {
            m_parameters[i]->set_value(source.parameters[i]);
        }
        for (usize i = 0; i < m_pair_parameters.size(); i++) {
            m_pair_parameters[i]->set_values(source.pair_parameters[i]);
        }
    }

    Parameters get_all_parameter_values() const {
        Parameters result;
        result.parameters.resize(m_parameters.size());
        result.pair_parameters.resize(m_pair_parameters.size());
        for (usize i = 0; i < m_parameters.size(); i++) {
            result.parameters[i] = m_parameters[i]->get_value();
        }
        for (usize i = 0; i < m_pair_parameters.size(); i++) {
            result.pair_parameters[i] = m_pair_parameters[i]->get_values();
        }
        return result;
    }

    Parameters get_all_parameter_gradients() const {
        Parameters result;
        result.parameters.resize(m_parameters.size());
        result.pair_parameters.resize(m_pair_parameters.size());
        for (usize i = 0; i < m_parameters.size(); i++) {
            result.parameters[i] = m_parameters[i]->get_gradient();
        }
        for (usize i = 0; i < m_pair_parameters.size(); i++) {
            result.pair_parameters[i] = m_pair_parameters[i]->get_graidents();
        }
        return result;
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

    void clear_backwardables() {
        m_backwardables.clear();
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
    ValuePtr get_parameter(usize index) const {
        return m_parameters[index];
    }
    PairPtr get_pair_parameter(usize index) const {
        return m_pair_parameters[index];
    }
};

}  // namespace Clockwork::Autograd
