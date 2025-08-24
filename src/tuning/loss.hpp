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

template<typename T = f64, typename Target = f64, Reduction R = Reduction::Mean>
auto mse(const std::vector<ValuePtr<T>>& predictions, const std::vector<Target>& targets) {
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
        // Compute sum of squared errors in one accumulated node
        std::vector<ValuePtr<T>> losses;
        losses.reserve(predictions.size());
        for (size_t i = 0; i < predictions.size(); ++i) {
            ValuePtr<T> diff = predictions[i] - targets[i];
            losses.push_back(diff * diff);
        }
        ValuePtr<T> total_loss = Value<T>::sum(losses);

        if constexpr (R == Reduction::Mean) {
            T n = static_cast<T>(predictions.size());
            return total_loss * Value<T>::create(static_cast<T>(1) / n);
        } else {
            return total_loss;
        }
    }
}

}  // namespace Autograd
}  // namespace Clockwork
