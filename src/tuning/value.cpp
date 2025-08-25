#pragma once
#include "value.hpp"
#include "../util/types.hpp"
#include "graph.hpp"
#include "util/vec/sse2.hpp"
#include <iostream>

namespace Clockwork {
namespace Autograd {

ValuePtr Value::create_tunable(f64 data) {
    ValuePtr res = std::make_shared<Value>(data);
    Graph::get()->register_param(res);
    return res;
}

ValuePtr Value::create(f64 data) {
    ValuePtr res = std::make_shared<Value>(data);
    Graph::get()->register_value(res);
    return res;
}

PairPtr Pair::create_tunable(f64 first, f64 second) {
    PairPtr res = std::make_shared<Pair>(first, second);
    Graph::get()->register_param(res);
    return res;
}

PairPtr Pair::create(f64 first, f64 second) {
    PairPtr res = std::make_shared<Pair>(first, second);
    Graph::get()->register_value(res);
    return res;
}

PairPtr Pair::create(const f128& values) {
    PairPtr res = std::make_shared<Pair>(values);
    Graph::get()->register_value(res);
    return res;
}


}  // namespace Autograd
}  // namespace Clockwork
