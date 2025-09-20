#pragma once

#include "value.hpp"
#include <memory>
#include <vector>

namespace Clockwork::Autograd {

enum class Reduction {
    None,
    Sum,
    Mean
};

template<typename Target = f64, Reduction R = Reduction::Mean>
auto mse(const std::vector<ValuePtr>& predictions, const std::vector<Target>& targets) {
    if (predictions.size() != targets.size()) {
        throw std::invalid_argument("MSE: predictions and targets must have the same size.");
    }

    if constexpr (R == Reduction::None) {
        // Return vector of squared errors (no reduction)
        std::vector<ValuePtr> losses;
        losses.reserve(predictions.size());
        for (size_t i = 0; i < predictions.size(); ++i) {
            ValuePtr diff = predictions[i] - targets[i];
            losses.push_back(diff * diff);
        }
        return losses;
    } else {
        // Compute sum of squared errors in one accumulated node
        std::vector<ValuePtr> losses;
        losses.reserve(predictions.size());
        for (size_t i = 0; i < predictions.size(); ++i) {
            ValuePtr diff = predictions[i] - targets[i];
            losses.push_back(diff * diff);
        }
        ValuePtr total_loss = Value::sum(losses);

        if constexpr (R == Reduction::Mean) {
            f64 n = static_cast<f64>(predictions.size());
            return total_loss * Value::create(static_cast<f64>(1) / n);
        } else {
            return total_loss;
        }
    }
}

}  // namespace Clockwork::Autograd
