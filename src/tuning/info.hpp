#pragma once

#include "util/types.hpp"
#include "util/vec/sse2.hpp"
#include <cassert>
#include <vector>
#include <random>

namespace Clockwork::Autograd {

struct ParameterCountInfo {
    usize parameter_count;
    usize pair_parameter_count;
};

struct Parameters {
    std::vector<f64>   parameters;
    std::vector<f64x2> pair_parameters;

    static Parameters zeros(ParameterCountInfo counts) {
        Parameters result;
        result.parameters.resize(counts.parameter_count, 0.0);
        result.pair_parameters.resize(counts.pair_parameter_count, f64x2::zero());
        return result;
    }

    static Parameters ones(ParameterCountInfo counts) {
        Parameters result;
        result.parameters.resize(counts.parameter_count, 1.0);
        result.pair_parameters.resize(counts.pair_parameter_count, f64x2::make(1.0, 1.0));
        return result;
    }

    static Parameters rand_init(ParameterCountInfo counts, f64 mean = 0.0, f64 variance = 0.1) {
        std::mt19937_64                     rng{474747};
        std::uniform_real_distribution<f64> distr(mean, variance);

        Parameters result;

        result.parameters.resize(counts.parameter_count);
        std::generate(result.parameters.begin(), result.parameters.end(), [&] {
            return distr(rng);
        });

        result.pair_parameters.resize(counts.pair_parameter_count);
        std::generate(result.pair_parameters.begin(), result.pair_parameters.end(), [&] {
            return f64x2::make(distr(rng), distr(rng));
        });

        return result;
    }

    void accumulate(const Parameters& b) {
        assert(b.parameters.size() == parameters.size());
        assert(b.pair_parameters.size() == pair_parameters.size());
        for (usize i = 0; i < parameters.size(); i++) {
            parameters[i] += b.parameters[i];
        }
        for (usize i = 0; i < pair_parameters.size(); i++) {
            pair_parameters[i] = f64x2::add(pair_parameters[i], b.pair_parameters[i]);
        }
    }

    void weighted_accumulate(double weight, const Parameters& b) {
        assert(b.parameters.size() == parameters.size());
        assert(b.pair_parameters.size() == pair_parameters.size());
        for (usize i = 0; i < parameters.size(); i++) {
            parameters[i] += weight * b.parameters[i];
        }
        for (usize i = 0; i < pair_parameters.size(); i++) {
            pair_parameters[i] =
              f64x2::madd(pair_parameters[i], f64x2::broadcast(weight), b.pair_parameters[i]);
        }
    }
};

}  // namespace Clockwork::Autograd
