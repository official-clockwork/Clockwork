#pragma once

#include "value.hpp"
#include <memory>
#include <vector>

namespace Clockwork {
namespace Autograd {

enum class Reduction {
    None,
    Sum,
    Mean
};


template<typename T = f64, Reduction R = Reduction::Mean>
auto mse(const std::vector<ValuePtr<T>>& predictions, const std::vector<ValuePtr<T>>& targets) {
    if (predictions.size() != targets.size()) {
        throw std::invalid_argument("MSE: predictions and targets must have the same size.");
    }

    if constexpr (R == Reduction::None) {
        // Return vector of squared errors (no reduction)
        std::vector<ValuePtr<T>> losses;
        losses.reserve(predictions.size());
        for (size_t i = 0; i < predictions.size(); ++i) {
            ValuePtr<T> diff = predictions[i] - targets[i];
            losses.push_back(diff * diff);
        }
        return losses;
    } else {
        // Compute sum of squared errors in one accumulated node:
        ValuePtr<T> total_loss = Value<T>::create(0.0);
        for (size_t i = 0; i < predictions.size(); ++i) {
            ValuePtr<T> diff = predictions[i] - targets[i];
            // Accumulate squared error directly to minimize nodes:
            total_loss = total_loss + diff * diff;
        }
        if constexpr (R == Reduction::Mean) {
            T n = static_cast<T>(predictions.size());
            return total_loss * Value<T>::create(1.0 / n);
        } else {  // Reduction::Sum
            return total_loss;
        }
    }
}
}
}