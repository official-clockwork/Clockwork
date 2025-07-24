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


template<typename T>
class Value;
template<typename T>
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

    Value(T data) :
        m_value(data) {};

public:
    friend class Graph<T>;

    T get_value() const {
        return m_value;
    }
    void change_value(T amount) {
        m_value += amount;
    }
    T get_gradient() const {
        return m_gradient;
    }


    static ValuePtr<T> create_tunable(T data) {
        return std::make_shared<Value<T>>(data);
    }

    static ValuePtr<T> create(T data) {
        ValuePtr<T> res = std::make_shared<Value<T>>(data);
        Graph<T>::get()->register_value(res);
        return res;
    }


    ValuePtr<T> exp() {
        auto        this_value  = this->shared_from_this();
        ValuePtr<T> result      = Value<T>::create(std::exp(this->m_value));
        result->m_dependencies  = {this_value};
        result->m_backward_func = [this_value, result]() {
            T grad = result->m_value;  // Avoid recomputing exp val
            this_value->m_gradient += grad * result->m_gradient;
        };
        return result;
    }


    ValuePtr<T> log() {
        auto        this_value  = this->shared_from_this();
        ValuePtr<T> result      = Value<T>::create(std::log(this->m_value));
        result->m_dependencies  = {this_value};
        result->m_backward_func = [this_value, result]() {
            T grad = (1 / this_value->m_value);
            this_value->m_gradient += grad * result->m_gradient;
        };
        return result;
    }


    ValuePtr<T> sigmoid() {
        auto        this_value  = this->shared_from_this();
        ValuePtr<T> result      = Value<T>::create(1 / (1 + std::exp(-this_value->m_value)));
        result->m_dependencies  = {this_value};
        result->m_backward_func = [this_value, result]() {
            T grad = result->m_value
                   * (1 - result->m_value);  // Same trick as before, avoid recomputing sigmoid(x)
            this_value->m_gradient += grad * result->m_gradient;
        };
        return result;
    }


    ValuePtr<T> pow(ValuePtr<T> exponent) {
        auto        this_value = this->shared_from_this();
        ValuePtr<T> result     = Value<T>::create(std::pow(this_value->m_value, exponent->m_value));
        result->m_dependencies = {this_value, exponent};
        result->m_backward_func = [this_value, exponent, result]() {
            this_value->m_gradient += exponent->m_value
                                    * std::pow(this_value->m_value, exponent->m_value - 1)
                                    * result->m_gradient;
            exponent->m_gradient +=
              result->m_value * std::log(this_value->m_value) * result->m_gradient;
        };
        return result;
    }


    ValuePtr<T> pow(T exponent) {
        auto        this_value  = this->shared_from_this();
        ValuePtr<T> result      = Value<T>::create(std::pow(this_value->m_value, exponent));
        result->m_dependencies  = {this_value};
        result->m_backward_func = [this_value, exponent, result]() {
            this_value->m_gradient +=
              exponent * std::pow(this_value->m_value, exponent - 1) * result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator-(ValuePtr<T> a) {
        ValuePtr<T> result      = Value<T>::create(-a->m_value);
        result->m_dependencies  = {a};
        result->m_backward_func = [a, result]() {
            T grad = -result->m_gradient;
            a->m_gradient -= grad;
        };
        return result;
    }


    friend ValuePtr<T> operator+(ValuePtr<T> a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value + b->m_value);
        result->m_dependencies  = {a, b};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += result->m_gradient;
            b->m_gradient += result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator-(ValuePtr<T> a,
                                 ValuePtr<T> b) {  // We are NOT cheaping out with a + (-b)
        ValuePtr<T> result      = Value<T>::create(a->m_value - b->m_value);
        result->m_dependencies  = {a, b};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += result->m_gradient;
            b->m_gradient -= result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator*(ValuePtr<T> a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value * b->m_value);
        result->m_dependencies  = {a, b};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += b->m_value * result->m_gradient;
            b->m_gradient += a->m_value * result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T>
    operator/(ValuePtr<T> a,
              ValuePtr<T> b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
        ValuePtr<T> result      = Value<T>::create(a->m_value / b->m_value);
        result->m_dependencies  = {a, b};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += 1.0 / b->m_value * result->m_gradient;
            b->m_gradient += -a->m_value / (b->m_value * b->m_value) * result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator+(ValuePtr<T> a, T b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value + b);
        result->m_dependencies  = {a};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator-(ValuePtr<T> a, T b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value - b);
        result->m_dependencies  = {a};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator*(ValuePtr<T> a, T b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value * b);
        result->m_dependencies  = {a};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += b * result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator/(ValuePtr<T> a,
                                 T b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
        ValuePtr<T> result      = Value<T>::create(a->m_value / b);
        result->m_dependencies  = {a};
        result->m_backward_func = [a, b, result]() {
            a->m_gradient += 1.0 / b * result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator+(T a, ValuePtr<T> b) {
        return b + a;
    }


    friend ValuePtr<T> operator-(T a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(b->m_value - a);
        result->m_dependencies  = {b};
        result->m_backward_func = [a, b, result]() {
            b->m_gradient -= result->m_gradient;
        };
        return result;
    }


    friend ValuePtr<T> operator*(T a, ValuePtr<T> b) {
        return b * a;
    }


    friend ValuePtr<T> operator/(T a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(a / b->m_value);
        result->m_dependencies  = {b};
        result->m_backward_func = [a, b, result]() {
            b->m_gradient += -a / (b->m_value * b->m_value) * result->m_gradient;
        };
        return result;
    }


    friend bool operator==(ValuePtr<T> a, ValuePtr<T> b) {
        return a->m_value == b->m_value;
    }


    friend bool operator!=(ValuePtr<T> a, ValuePtr<T> b) {
        return a->m_value != b->m_value;
    }


    friend bool operator<(ValuePtr<T> a, ValuePtr<T> b) {
        return a->m_value < b->m_value;
    }


    friend bool operator<=(ValuePtr<T> a, ValuePtr<T> b) {
        return a->m_value <= b->m_value;
    }


    friend bool operator>(ValuePtr<T> a, ValuePtr<T> b) {
        return a->m_value > b->m_value;
    }


    friend bool operator>=(ValuePtr<T> a, ValuePtr<T> b) {
        return a->m_value >= b->m_value;
    }


    friend std::ostream& operator<<(std::ostream& os, const ValuePtr<T>& value) {
        os << "Value(data=" << value->get_value() << ", grad=" << value->get_gradient() << ")";
        return os;
    }


    void backward() {
        auto this_value = this->shared_from_this();
        this_value->m_backward_func();
    }
};


}
}
