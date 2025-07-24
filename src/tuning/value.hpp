#pragma once

#include "../util/types.hpp"
#include "graph.hpp"
#include <iostream>

#include <cmath>
#include <functional>
#include <memory>
#include <vector>

namespace Clockwork {

namespace Autograd {


template<typename T = f64>
class Value;
template<typename T = f64>
class Graph;
template<typename T>
using ValuePtr = std::shared_ptr<Value<T>>;


template<typename T = f64>
class Value : public std::enable_shared_from_this<Value<T>> {
private:
    T                        m_value    = 0;
    T                        m_gradient = 0;
    std::vector<ValuePtr<T>> m_dependencies;
    std::function<void()>    m_backward_func;

    Value(T data);

public:
    static ValuePtr<T> create(T data);  // We will replace this with a better name

    T get_value() const {
        return m_value;
    }
    T get_gradient() const {
        return m_gradient;
    }

    void backward();

    // Define unary ops here
    ValuePtr<T> exp();
    ValuePtr<T> log();
    ValuePtr<T> sigmoid();
    ValuePtr<T> pow(ValuePtr<T> exponent);
    ValuePtr<T> pow(T exponent);

    // Negation
    template<typename U>
    friend ValuePtr<T> operator-(ValuePtr<U> a);

    // Define binary ops here
    template<typename U>
    friend ValuePtr<T> operator+(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend ValuePtr<T> operator-(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend ValuePtr<T> operator*(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend ValuePtr<T> operator/(ValuePtr<U> a, ValuePtr<U> b);

    template<typename U>
    friend ValuePtr<T> operator+(ValuePtr<U> a, U b);
    template<typename U>
    friend ValuePtr<T> operator-(ValuePtr<U> a, U b);
    template<typename U>
    friend ValuePtr<T> operator*(ValuePtr<U> a, U b);
    template<typename U>
    friend ValuePtr<T> operator/(ValuePtr<U> a, U b);

    // Define comparison ops here
    template<typename U>
    friend bool operator==(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend bool operator!=(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend bool operator<(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend bool operator<=(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend bool operator>(ValuePtr<U> a, ValuePtr<U> b);
    template<typename U>
    friend bool operator>=(ValuePtr<U> a, ValuePtr<U> b);

    // Print << operator
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Value<U>& value);
};

template<typename T>
Value<T>::Value(T data) :
    m_value(data){};

// TODO: might replace with std::make_shared for clearer semantics
template<typename T>
ValuePtr Value<T>::create(T data) {
    ValuePtr res = std::shared_ptr<Value<T>>(new Value<T>(data));
    Graph::get().register_value(res);
    return res;
}

template<typename T>
ValuePtr<T> Value<T>::exp() {
    auto        this_value  = this->shared_from_this();
    ValuePtr<T> result      = Value<T>::create(std::exp(this->m_value));
    result->m_dependencies  = {this_value};
    result->m_backward_func = [this_value, result]() {
        T grad =
          result->m_value
          * result
              ->m_gradient;  // Avoid recomputing std::exp, since grad of e^x is e^x, we can just use the one stored in the result var
        this_value->m_gradient += grad;
    };
    return result;
}

template<typename T>
ValuePtr<T> Value<T>::log() {
    auto        this_value  = this->shared_from_this();
    ValuePtr<T> result      = Value<T>::create(std::log(this->m_value));
    result->m_dependencies  = {this_value};
    result->m_backward_func = [this_value, result]() {
        T grad = (1 / this_value->m_value) * result->m_gradient;
        this_value->m_gradient += grad;
    };
    return result;
}

template<typename T>
ValuePtr<T> Value<T>::sigmoid() {
    auto        this_value  = this->shared_from_this();
    ValuePtr<T> result      = Value<T>::create(1 / (1 + std::exp(-this_value->m_value)));
    result->m_dependencies  = {this_value};
    result->m_backward_func = [this_value, result]() {
        T grad = result->m_value
               * (1 - result->m_value);  // Same trick as before, avoid recomputing sigmoid(x)
        this_value->m_gradient += grad;
    };
    return result;
}

template<typename T>
ValuePtr<T> Value<T>::pow(ValuePtr<T> exponent) {
    auto        this_value  = this->shared_from_this();
    ValuePtr<T> result      = Value<T>::create(std::pow(this_value->m_value, exponent->m_value));
    result->m_dependencies  = {this_value, exponent};
    result->m_backward_func = [this_value, exponent, result]() {
        this_value->m_gradient += exponent->m_value
                                * std::pow(this_value->m_value, exponent->m_value - 1)
                                * result->m_gradient;
        exponent->m_gradient +=
          result->m_value * std::log(this_value->m_value) * result->m_gradient;
    };
    return result;
}

template<typename T>
ValuePtr<T> Value<T>::pow(T exponent) {
    auto        this_value  = this->shared_from_this();
    ValuePtr<T> result      = Value<T>::create(std::pow(this_value->m_value, exponent));
    result->m_dependencies  = {this_value};
    result->m_backward_func = [this_value, exponent, result]() {
        this_value->m_gradient +=
          exponent * std::pow(this_value->m_value, exponent - 1) * result->m_gradient;
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator-(ValuePtr<U> a) {
    auto        this_value  = this->shared_from_this();
    ValuePtr<T> result      = Value<T>::create(static_cast<T>(-a->m_value));
    result->m_dependencies  = {this_value};
    result->m_backward_func = [this_value, result]() {
        T grad = static_cast<U>(-result->m_gradient);
        this_value->m_gradient += grad;
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator+(ValuePtr<U> a, ValuePtr<U> b) {
    ValuePtr<T> result = Value<T>::create(static_cast<T>(a->m_value) + static_cast<T>(b->m_value));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += static_cast<U>(result->m_gradient);
        b->m_gradient += static_cast<U>(result->m_gradient);
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator-(ValuePtr<U> a, ValuePtr<U> b) {  // We are NOT cheaping out with a + (-b)
    ValuePtr<T> result = Value<T>::create(static_cast<T>(a->m_value) - static_cast<T>(b->m_value));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += static_cast<U>(result->m_gradient);
        b->m_gradient -= static_cast<U>(result->m_gradient);
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator*(ValuePtr<U> a, ValuePtr<U> b) {
    ValuePtr<T> result = Value<T>::create(static_cast<T>(a->m_value) * static_cast<T>(b->m_value));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += b->m_value * static_cast<U>(result->m_gradient);
        b->m_gradient += a->m_value * static_cast<U>(result->m_gradient);
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator*(ValuePtr<U> a,
                      ValuePtr<U> b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
    ValuePtr<T> result = Value<T>::create(static_cast<T>(a->m_value) / static_cast<T>(b->m_value));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += static_cast<T>(1) / static_cast<T>(b->m_value) * result->m_gradient;
        b->m_gradient += -static_cast<T>(a->m_value)
                       / (static_cast<T>(b->m_value) * static_cast<T>(b->m_value))
                       * result->m_gradient;
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator+(ValuePtr<U> a, U b) {
    ValuePtr<T> result      = Value<T>::create(static_cast<T>(a->m_value) + static_cast<T>(b));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += static_cast<U>(result->m_gradient);
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator-(ValuePtr<U> a, U b) {
    ValuePtr<T> result      = Value<T>::create(static_cast<T>(a->m_value) - static_cast<T>(b));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += static_cast<U>(result->m_gradient);
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator*(ValuePtr<U> a, U b) {
    ValuePtr<T> result      = Value<T>::create(static_cast<T>(a->m_value) * static_cast<T>(b));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += b * static_cast<U>(result->m_gradient);
    };
    return result;
}

template<typename U>
template<typename T>
ValuePtr<T> operator*(ValuePtr<U> a,
                      U           b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
    ValuePtr<T> result      = Value<T>::create(static_cast<T>(a->m_value) / static_cast<T>(b));
    result->m_dependencies  = {a, b};
    result->m_backward_func = [a, b, result]() {
        a->m_gradient += static_cast<T>(1) / static_cast<T>(b) * result->m_gradient;
    };
    return result;
}

template<typename U>
template<typename T>
bool operator==(ValuePtr<U> a, ValuePtr<U> b) {
    return a->m_value == b->m_value;
}

template<typename U>
template<typename T>
bool operator!=(ValuePtr<U> a, ValuePtr<U> b) {
    return a->m_value != b->m_value;
}

template<typename U>
template<typename T>
bool operator<(ValuePtr<U> a, ValuePtr<U> b) {
    return a->m_value < b->m_value;
}

template<typename U>
template<typename T>
bool operator<=(ValuePtr<U> a, ValuePtr<U> b) {
    return a->m_value <= b->m_value;
}

template<typename U>
template<typename T>
bool operator>(ValuePtr<U> a, ValuePtr<U> b) {
    return a->m_value > b->m_value;
}

template<typename U>
template<typename T>
bool operator>=(ValuePtr<U> a, ValuePtr<U> b) {
    return a->m_value >= b->m_value;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ValuePtr<T>& value) {
    os << "Value(data=" << value->m_value << ", grad=" << value->m_grad << ")";
    return os;
}

template<typename T>
void Value<T>::backward() {
    auto this_value = this->shared_from_this();
    this_value->m_backward_func();
}
}
}
