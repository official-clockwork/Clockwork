#pragma once

#include "../util/types.hpp"
#include <iostream>

#include <cmath>
#include <functional>
#include <memory>
#include <vector>

namespace Clockwork {

namespace Autograd {

// Forward declarations
template<typename T>
class Backwardable;
template<typename TuneType, typename T>
class SmartBackwardable;
template<typename T>
class Value;
template<typename T>
class Pair;
template<typename T>
class Graph;

template<typename T>
using ValuePtr = std::shared_ptr<Value<T>>;
template<typename T>
using PairPtr = std::shared_ptr<Pair<T>>;
template<typename T>
using BackwardablePtr = std::shared_ptr<Backwardable<T>>;

template<typename T>
class Backwardable {
public:
    friend class Graph<T>;
    std::function<void()> m_backward_func;
    virtual ~Backwardable() = default;
    virtual void backward() = 0;
};

template<typename TuneType, typename T>
class SmartBackwardable : public Backwardable<T>, public std::enable_shared_from_this<TuneType> {
public:
    virtual ~SmartBackwardable() = default;
};

template<typename T = f64>
class Value : public SmartBackwardable<Value<T>, T> {
private:
    T                                m_value    = 0;
    T                                m_gradient = 0;
    std::function<void(ValuePtr<T>)> m_backward_func;

public:
    friend class Graph<T>;
    friend class Pair<T>;

    explicit Value(T data) :
        m_value(data) {};

    inline T get_value() const {
        return m_value;
    }
    inline void change_value(T amount) {
        m_value += amount;
    }
    inline T get_gradient() const {
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
        result->m_backward_func = [this_value](ValuePtr<T> out) {
            T grad = out->m_value;  // Avoid recomputing exp val
            this_value->m_gradient += grad * out->m_gradient;
        };
        return result;
    }

    ValuePtr<T> log() {
        auto        this_value  = this->shared_from_this();
        ValuePtr<T> result      = Value<T>::create(std::log(this->m_value));
        result->m_backward_func = [this_value](ValuePtr<T> out) {
            T grad = (1 / this_value->m_value);
            this_value->m_gradient += grad * out->m_gradient;
        };
        return result;
    }

    ValuePtr<T> sigmoid() {
        auto        this_value  = this->shared_from_this();
        ValuePtr<T> result      = Value<T>::create(1 / (1 + std::exp(-this_value->m_value)));
        result->m_backward_func = [this_value](ValuePtr<T> out) {
            T grad = out->m_value
                   * (1 - out->m_value);  // Same trick as before, avoid recomputing sigmoid(x)
            this_value->m_gradient += grad * out->m_gradient;
        };
        return result;
    }

    ValuePtr<T> pow(ValuePtr<T> exponent) {
        auto        this_value = this->shared_from_this();
        ValuePtr<T> result     = Value<T>::create(std::pow(this_value->m_value, exponent->m_value));
        result->m_backward_func = [this_value, exponent](ValuePtr<T> out) {
            this_value->m_gradient += exponent->m_value
                                    * std::pow(this_value->m_value, exponent->m_value - 1)
                                    * out->m_gradient;
            exponent->m_gradient += out->m_value * std::log(this_value->m_value) * out->m_gradient;
        };
        return result;
    }

    ValuePtr<T> pow(T exponent) {
        auto        this_value  = this->shared_from_this();
        ValuePtr<T> result      = Value<T>::create(std::pow(this_value->m_value, exponent));
        result->m_backward_func = [this_value, exponent](ValuePtr<T> out) {
            this_value->m_gradient +=
              exponent * std::pow(this_value->m_value, exponent - 1) * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator-(ValuePtr<T> a) {
        ValuePtr<T> result      = Value<T>::create(-a->m_value);
        result->m_backward_func = [a](ValuePtr<T> out) {
            T grad = -out->m_gradient;
            a->m_gradient += grad;
        };
        return result;
    }

    friend ValuePtr<T> operator+(ValuePtr<T> a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value + b->m_value);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += out->m_gradient;
            b->m_gradient += out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator-(ValuePtr<T> a,
                                 ValuePtr<T> b) {  // We are NOT cheaping out with a + (-b)
        ValuePtr<T> result      = Value<T>::create(a->m_value - b->m_value);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += out->m_gradient;
            b->m_gradient -= out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator*(ValuePtr<T> a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value * b->m_value);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += b->m_value * out->m_gradient;
            b->m_gradient += a->m_value * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T>
    operator/(ValuePtr<T> a,
              ValuePtr<T> b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
        ValuePtr<T> result      = Value<T>::create(a->m_value / b->m_value);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += 1.0 / b->m_value * out->m_gradient;
            b->m_gradient += -a->m_value / (b->m_value * b->m_value) * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator+(ValuePtr<T> a, T b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value + b);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator-(ValuePtr<T> a, T b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value - b);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator*(ValuePtr<T> a, T b) {
        ValuePtr<T> result      = Value<T>::create(a->m_value * b);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += b * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator/(ValuePtr<T> a,
                                 T b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
        ValuePtr<T> result      = Value<T>::create(a->m_value / b);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            a->m_gradient += 1.0 / b * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator+(T a, ValuePtr<T> b) {
        return b + a;
    }

    friend ValuePtr<T> operator-(T a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(a - b->m_value);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            b->m_gradient -= out->m_gradient;
        };
        return result;
    }

    friend ValuePtr<T> operator*(T a, ValuePtr<T> b) {
        return b * a;
    }

    friend ValuePtr<T> operator/(T a, ValuePtr<T> b) {
        ValuePtr<T> result      = Value<T>::create(a / b->m_value);
        result->m_backward_func = [a, b](ValuePtr<T> out) {
            b->m_gradient += -a / (b->m_value * b->m_value) * out->m_gradient;
        };
        return result;
    }

    static ValuePtr<T> sum(const std::vector<ValuePtr<T>>& inputs) {
        if (inputs.empty()) {
            return Value<T>::create(0.0);
        }

        T sum = 0;
        for (auto& v : inputs) {
            sum += v->m_value;
        }

        ValuePtr<T> result = Value<T>::create(sum);

        result->m_backward_func = [inputs](ValuePtr<T> out) {
            for (auto& v : inputs) {
                v->m_gradient += out->m_gradient;
            }
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

    void backward() override {
        auto this_value = this->shared_from_this();
        if (this_value->m_backward_func) {
            this_value->m_backward_func(this_value);
        }
    }
};

template<typename T = f64>
class Pair : public SmartBackwardable<Pair<T>, T> {
private:
    std::function<void(PairPtr<T>)> m_backward_func;

public:
    friend class Graph<T>;
    friend class Value<T>;

    T m_first;
    T m_second;
    T m_grad_first  = 0;
    T m_grad_second = 0;

    explicit Pair(T first, T second) :
        m_first(first),
        m_second(second) {
    }

    // Create tunable (does not register in graph)
    static PairPtr<T> create_tunable(T first, T second) {
        return std::make_shared<Pair<T>>(first, second);
    }

    // Create and register in graph
    static PairPtr<T> create(T first, T second) {
        PairPtr<T> res = std::make_shared<Pair<T>>(first, second);
        Graph<T>::get()->register_value(res);
        return res;
    }

    inline T first() const {
        return m_first;
    }
    inline T second() const {
        return m_second;
    }

    inline T grad_first() const {
        return m_grad_first;
    }
    inline T grad_second() const {
        return m_grad_second;
    }

    // ------------------- Backward -------------------
    void backward() override {
        auto self = this->shared_from_this();
        if (m_backward_func) {
            m_backward_func(self);
        }
    }

    // ------------------- Arithmetic -------------------
    friend PairPtr<T> operator+(const PairPtr<T>& a, const PairPtr<T>& b) {
        PairPtr<T> result = Pair<T>::create(a->m_first + b->m_first, a->m_second + b->m_second);
        result->m_backward_func = [a, b](PairPtr<T> out) {
            a->m_grad_first += out->m_grad_first;
            a->m_grad_second += out->m_grad_second;
            b->m_grad_first += out->m_grad_first;
            b->m_grad_second += out->m_grad_second;
        };
        return result;
    }

    friend PairPtr<T> operator-(const PairPtr<T>& a, const PairPtr<T>& b) {
        PairPtr<T> result = Pair<T>::create(a->m_first - b->m_first, a->m_second - b->m_second);
        result->m_backward_func = [a, b](PairPtr<T> out) {
            a->m_grad_first += out->m_grad_first;
            a->m_grad_second += out->m_grad_second;
            b->m_grad_first -= out->m_grad_first;
            b->m_grad_second -= out->m_grad_second;
        };
        return result;
    }

    // ------------------- Scalar multiplication -------------------
    friend PairPtr<T> operator*(const PairPtr<T>& a, T scalar) {
        PairPtr<T> result       = Pair<T>::create(a->m_first * scalar, a->m_second * scalar);
        result->m_backward_func = [a, scalar](PairPtr<T> out) {
            a->m_grad_first += scalar * out->m_grad_first;
            a->m_grad_second += scalar * out->m_grad_second;
        };
        return result;
    }

    friend PairPtr<T> operator*(T scalar, const PairPtr<T>& a) {
        return a * scalar;
    }

    // ------------------- Pair * Value -------------------
    friend PairPtr<T> operator*(const PairPtr<T>& a, const ValuePtr<T>& v) {
        PairPtr<T> result =
          Pair<T>::create(a->m_first * v->get_value(), a->m_second * v->get_value());
        result->m_backward_func = [a, v](PairPtr<T> out) {
            a->m_grad_first += v->get_value() * out->m_grad_first;
            a->m_grad_second += v->get_value() * out->m_grad_second;
            v->m_gradient += a->m_first * out->m_grad_first + a->m_second * out->m_grad_second;
        };
        return result;
    }

    friend PairPtr<T> operator*(const ValuePtr<T>& v, const PairPtr<T>& a) {
        return a * v;
    }

    // ------------------- Pair / scalar -------------------
    friend PairPtr<T> operator/(const PairPtr<T>& a, T scalar) {
        PairPtr<T> result       = Pair<T>::create(a->m_first / scalar, a->m_second / scalar);
        result->m_backward_func = [a, scalar](PairPtr<T> out) {
            a->m_grad_first += out->m_grad_first / scalar;
            a->m_grad_second += out->m_grad_second / scalar;
        };
        return result;
    }

    // ------------------- Scalar / Pair -------------------
    friend PairPtr<T> operator/(T scalar, const PairPtr<T>& a) {
        PairPtr<T> result       = Pair<T>::create(scalar / a->m_first, scalar / a->m_second);
        result->m_backward_func = [a, scalar](PairPtr<T> out) {
            a->m_grad_first -= scalar / (a->m_first * a->m_first) * out->m_grad_first;
            a->m_grad_second -= scalar / (a->m_second * a->m_second) * out->m_grad_second;
        };
        return result;
    }

    // ------------------- Pair / Value -------------------
    friend PairPtr<T> operator/(const PairPtr<T>& a, const ValuePtr<T>& v) {
        PairPtr<T> result =
          Pair<T>::create(a->m_first / v->get_value(), a->m_second / v->get_value());
        result->m_backward_func = [a, v](PairPtr<T> out) {
            a->m_grad_first += out->m_grad_first / v->get_value();
            a->m_grad_second += out->m_grad_second / v->get_value();
            v->m_gradient -= (a->m_first * out->m_grad_first + a->m_second * out->m_grad_second)
                           / (v->get_value() * v->get_value());
        };
        return result;
    }

    // ------------------- Value / Pair -------------------
    friend PairPtr<T> operator/(const ValuePtr<T>& v, const PairPtr<T>& a) {
        PairPtr<T> result =
          Pair<T>::create(v->get_value() / a->m_first, v->get_value() / a->m_second);
        result->m_backward_func = [a, v](PairPtr<T> out) {
            a->m_grad_first -= v->get_value() / (a->m_first * a->m_first) * out->m_grad_first;
            a->m_grad_second -= v->get_value() / (a->m_second * a->m_second) * out->m_grad_second;
            v->m_gradient += out->m_grad_first / a->m_first + out->m_grad_second / a->m_second;
        };
        return result;
    }

    // ------------------- Unary negation -------------------
    friend PairPtr<T> operator-(PairPtr<T> a) {
        PairPtr<T> result       = Pair<T>::create(-a->m_first, -a->m_second);
        result->m_backward_func = [a](PairPtr<T> out) {
            a->m_grad_first -= out->m_grad_first;
            a->m_grad_second -= out->m_grad_second;
        };
        return result;
    }

    // ------------------- Phasing -------------------
    ValuePtr<T> phase(T alpha) {
        auto        self   = this->shared_from_this();
        ValuePtr<T> result = Value<T>::create(alpha * m_first + (1 - alpha) * m_second);

        result->m_backward_func = [self, alpha](ValuePtr<T> out) {
            // Gradient of output w.r.t first and second
            self->m_grad_first += alpha * out->m_gradient;
            self->m_grad_second += (1 - alpha) * out->m_gradient;
        };

        return result;
    }

    // ------------------- Print -------------------
    friend std::ostream& operator<<(std::ostream& os, const PairPtr<T>& p) {
        os << "Pair(first=" << p->m_first << ", second=" << p->m_second
           << ", grad_first=" << p->m_grad_first << ", grad_second=" << p->m_grad_second << ")";
        return os;
    }
};

}
}
