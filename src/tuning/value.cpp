#include "tuning/value.hpp"
#include "operations.hpp"
#include "tuning/graph.hpp"
#include "util/types.hpp"
#include <iostream>

namespace Clockwork::Autograd {

// ------------------ ValueHandle ------------------

ValueHandle ValueHandle::create(f64 data) {
    return Graph::get().create_value(data);
}

ValueHandle ValueHandle::sum(const std::vector<ValueHandle>& inputs) {
    if (inputs.empty()) {
        return ValueHandle::create(0.0);
    }
    ValueHandle total = inputs[0];
    for (size_t i = 1; i < inputs.size(); ++i) {
        total = total + inputs[i];
    }
    return total;
}

ValueHandle ValueHandle::exp() const {
    return Graph::get().record_op(OpType::Exp, *this);
}
ValueHandle ValueHandle::log() const {
    return Graph::get().record_op(OpType::Log, *this);
}
ValueHandle ValueHandle::sigmoid() const {
    return Graph::get().record_op(OpType::Sigmoid, *this);
}
ValueHandle ValueHandle::pow(ValueHandle exponent) const {
    return Graph::get().record_op(OpType::Pow, *this, exponent);
}
ValueHandle ValueHandle::pow(f64 exponent) const {
    return Graph::get().record_op(OpType::PowConst, *this, exponent);
}

void ValueHandle::add_gradient(f64 rhs) const {
    if (is_valid()) {
        Graph::get().add_value_gradient(index, rhs);
    }
}

f64 ValueHandle::get_value() const {
    return is_valid() ? Graph::get().get_value(index) : 0.0;
}

f64 ValueHandle::get_gradient() const {
    return is_valid() ? Graph::get().get_gradient(index) : 0.0;
}

void ValueHandle::zero_grad() const {
    if (is_valid()) {
        Graph::get().zero_value_grad(index);
    }
}

void ValueHandle::set_value(f64 v) const {
    if (is_valid()) {
        Graph::get().set_value(index, v);
    }
}

// PairHandle implementations

PairHandle PairHandle::create(f64 first, f64 second) {
    return Graph::get().create_pair(f64x2::make(first, second));
}

PairHandle PairHandle::create(const f64x2& values) {
    return Graph::get().create_pair(values);
}

f64x2 PairHandle::get_values() const {
    return Graph::get().get_pair_values(index);
}
f64x2 PairHandle::get_gradients() const {
    return Graph::get().get_pair_gradients(index);
}
f64 PairHandle::first() const {
    return get_values().first();
}
f64 PairHandle::second() const {
    return get_values().second();
}

void PairHandle::set_values(const f64x2& v) const {
    Graph::get().set_pair_values(index, v);
}
void PairHandle::set_values(f64 f, f64 s) const {
    set_values(f64x2::make(f, s));
}

void PairHandle::zero_grad() const {
    Graph::get().zero_pair_grad(index);
}

// Special phasing case
ValueHandle PairHandle::phase_impl(f64 scaled_alpha) const {
    return Graph::get().record_phase(*this, scaled_alpha);
}
// Sigmoid operation
PairHandle PairHandle::sigmoid() const {
    return Graph::get().record_pair_unary(OpType::PairSigmoid, *this);
}

// ValueHandle Operators
ValueHandle operator-(ValueHandle a) {
    return Graph::get().record_op(OpType::Neg, a);
}
ValueHandle operator+(ValueHandle a, ValueHandle b) {
    return Graph::get().record_op(OpType::Add, a, b);
}
ValueHandle operator-(ValueHandle a, ValueHandle b) {
    return Graph::get().record_op(OpType::Sub, a, b);
}
ValueHandle operator*(ValueHandle a, ValueHandle b) {
    return Graph::get().record_op(OpType::Mul, a, b);
}
ValueHandle operator/(ValueHandle a, ValueHandle b) {
    return Graph::get().record_op(OpType::Div, a, b);
}

ValueHandle operator+(ValueHandle a, f64 b) {
    return Graph::get().record_op(OpType::AddScalar, a, b);
}
ValueHandle operator-(ValueHandle a, f64 b) {
    return Graph::get().record_op(OpType::ValSubScalar, a, b);
}
ValueHandle operator*(ValueHandle a, f64 b) {
    return Graph::get().record_op(OpType::MulScalar, a, b);
}
ValueHandle operator/(ValueHandle a, f64 b) {
    return Graph::get().record_op(OpType::ValDivScalar, a, b);
}

ValueHandle operator+(f64 a, ValueHandle b) {
    return b + a;
}
ValueHandle operator-(f64 a, ValueHandle b) {
    return Graph::get().record_op(OpType::SubScalarVal, b, a);
}
ValueHandle operator*(f64 a, ValueHandle b) {
    return b * a;
}
ValueHandle operator/(f64 a, ValueHandle b) {
    return Graph::get().record_op(OpType::DivScalarVal, b, a);
}

bool operator<(ValueHandle a, ValueHandle b) {
    return a.get_value() < b.get_value();
}
bool operator>(ValueHandle a, ValueHandle b) {
    return a.get_value() > b.get_value();
}

// PairHandle Operators
PairHandle operator+(PairHandle a, PairHandle b) {
    return Graph::get().record_pair_op(OpType::PairAdd, a, b);
}
PairHandle operator-(PairHandle a, PairHandle b) {
    return Graph::get().record_pair_op(OpType::PairSub, a, b);
}
PairHandle operator-(PairHandle a) {
    return Graph::get().record_pair_scalar(OpType::PairNeg, a, 0.0);
}

PairHandle operator*(PairHandle a, f64 scalar) {
    return Graph::get().record_pair_scalar(OpType::PairMulScalar, a, scalar);
}
PairHandle operator*(f64 scalar, PairHandle a) {
    return a * scalar;
}
PairHandle operator*(PairHandle a, PairHandle b) {
    return Graph::get().record_pair_value(OpType::PairMulPair, a, b);
}
PairHandle operator/(PairHandle a, f64 scalar) {
    return Graph::get().record_pair_scalar(OpType::PairDivScalar, a, scalar);
}
PairHandle operator/(f64 scalar, PairHandle a) {
    return Graph::get().record_pair_scalar(OpType::ScalarDivPair, a, scalar);
}

PairHandle operator*(PairHandle a, ValueHandle v) {
    return Graph::get().record_pair_value(OpType::PairMulValue, a, v);
}
PairHandle operator*(ValueHandle v, PairHandle a) {
    return Graph::get().record_pair_value(OpType::ValueMulPair, a, v);
}
PairHandle operator/(PairHandle a, ValueHandle v) {
    return Graph::get().record_pair_value(OpType::PairDivValue, a, v);
}
PairHandle operator/(ValueHandle v, PairHandle a) {
    return Graph::get().record_pair_value(OpType::ValueDivPair, a, v);
}

// Printing overloads for debugging
std::ostream& operator<<(std::ostream& os, const PairHandle& p) {
    os << "S(" << std::round(p.first()) << ", " << std::round(p.second()) << ")";
    return os;
}

// Value Inplaces
ValueHandle& operator+=(ValueHandle& a, ValueHandle b) {
    a = a + b;
    return a;
}
ValueHandle& operator-=(ValueHandle& a, ValueHandle b) {
    a = a - b;
    return a;
}
ValueHandle& operator*=(ValueHandle& a, ValueHandle b) {
    a = a * b;
    return a;
}
ValueHandle& operator/=(ValueHandle& a, ValueHandle b) {
    a = a / b;
    return a;
}

ValueHandle& operator+=(ValueHandle& a, f64 b) {
    a = a + b;
    return a;
}
ValueHandle& operator-=(ValueHandle& a, f64 b) {
    a = a - b;
    return a;
}
ValueHandle& operator*=(ValueHandle& a, f64 b) {
    a = a * b;
    return a;
}
ValueHandle& operator/=(ValueHandle& a, f64 b) {
    a = a / b;
    return a;
}

// Pair Inplaces
PairHandle& operator+=(PairHandle& a, PairHandle b) {
    a = a + b;
    return a;
}
PairHandle& operator-=(PairHandle& a, PairHandle b) {
    a = a - b;
    return a;
}
PairHandle& operator*=(PairHandle& a, f64 scalar) {
    a = a * scalar;
    return a;
}
PairHandle& operator*=(PairHandle& a, ValueHandle v) {
    a = a * v;
    return a;
}
PairHandle& operator/=(PairHandle& a, f64 scalar) {
    a = a / scalar;
    return a;
}
PairHandle& operator/=(PairHandle& a, ValueHandle v) {
    a = a / v;
    return a;
}

}  // namespace Clockwork::Autograd
