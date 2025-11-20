#pragma once

#include "value.hpp"
#include <stdexcept>
#include <vector>

namespace Clockwork::Autograd {

enum class Reduction {
    None,
    Sum,
    Mean
};

template<typename Target = f64, Reduction R = Reduction::Mean>
auto mse(const std::vector<ValueHandle>& predictions, const std::vector<Target>& targets) {
    if (predictions.size() != targets.size()) {
        throw std::invalid_argument("MSE: predictions and targets must have the same size.");
    }

    if constexpr (R == Reduction::None) {
        std::vector<ValueHandle> losses;
        losses.reserve(predictions.size());
        for (size_t i = 0; i < predictions.size(); ++i) {
            ValueHandle diff = predictions[i] - targets[i];
            losses.push_back(diff * diff);
        }
        return losses;
    } else {
        std::vector<ValueHandle> losses;
        losses.reserve(predictions.size());
        for (size_t i = 0; i < predictions.size(); ++i) {
            ValueHandle diff = predictions[i] - targets[i];
            losses.push_back(diff * diff);
        }
        ValueHandle total_loss = ValueHandle::sum(losses);

        if constexpr (R == Reduction::Mean) {
            f64 n = static_cast<f64>(predictions.size());
            return total_loss * (1.0 / n);
        } else {
            return total_loss;
        }
    }
}

}  // namespace Clockwork::Autograd
