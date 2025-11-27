#pragma once

#include "../util/types.hpp"
#include "util/vec/sse2.hpp"
#include <iostream>

#include <cmath>
#include <functional>
#include <memory>
#include <vector>

namespace Clockwork::Autograd {

// Forward declarations

class Backwardable;
template<typename TuneType>
class SmartBackwardable;
class Value;
class Pair;
class Graph;


using ValuePtr = std::shared_ptr<Value>;

using PairPtr = std::shared_ptr<Pair>;

using BackwardablePtr = std::shared_ptr<Backwardable>;


class Backwardable {
public:
    friend class Graph;
    std::function<void()> m_backward_func;
    virtual ~Backwardable() = default;
    virtual void backward() = 0;
};

template<typename TuneType>
class SmartBackwardable : public Backwardable, public std::enable_shared_from_this<TuneType> {
public:
    virtual ~SmartBackwardable() = default;
};


class Value : public SmartBackwardable<Value> {
private:
    f64                           m_value    = 0;
    f64                           m_gradient = 0;
    std::function<void(ValuePtr)> m_backward_func;

public:
    friend class Graph;
    friend class Pair;

    explicit Value(f64 data) :
        m_value(data) {};

    inline f64 get_value() const {
        return m_value;
    }
    inline void change_value(f64 amount) {
        m_value += amount;
    }
    inline void set_value(f64 value) {
        m_value = value;
    }
    inline f64 get_gradient() const {
        return m_gradient;
    }

    inline void zero_grad() {
        m_gradient = 0.0;
    }
    static ValuePtr create(f64 data);

    ValuePtr exp() {
        auto     this_value     = this->shared_from_this();
        ValuePtr result         = Value::create(std::exp(this->m_value));
        result->m_backward_func = [this_value](ValuePtr out) {
            f64 grad = out->m_value;  // Avoid recomputing exp val
            this_value->m_gradient += grad * out->m_gradient;
        };
        return result;
    }

    ValuePtr log() {
        auto     this_value     = this->shared_from_this();
        ValuePtr result         = Value::create(std::log(this->m_value));
        result->m_backward_func = [this_value](ValuePtr out) {
            f64 grad = (1 / this_value->m_value);
            this_value->m_gradient += grad * out->m_gradient;
        };
        return result;
    }

    ValuePtr sigmoid() {
        auto     this_value     = this->shared_from_this();
        ValuePtr result         = Value::create(1 / (1 + std::exp(-this_value->m_value)));
        result->m_backward_func = [this_value](ValuePtr out) {
            f64 grad = out->m_value
                     * (1 - out->m_value);  // Same trick as before, avoid recomputing sigmoid(x)
            this_value->m_gradient += grad * out->m_gradient;
        };
        return result;
    }

    ValuePtr pow(ValuePtr exponent) {
        auto     this_value     = this->shared_from_this();
        ValuePtr result         = Value::create(std::pow(this_value->m_value, exponent->m_value));
        result->m_backward_func = [this_value, exponent](ValuePtr out) {
            this_value->m_gradient += exponent->m_value
                                    * std::pow(this_value->m_value, exponent->m_value - 1)
                                    * out->m_gradient;
            exponent->m_gradient += out->m_value * std::log(this_value->m_value) * out->m_gradient;
        };
        return result;
    }

    ValuePtr pow(f64 exponent) {
        auto     this_value     = this->shared_from_this();
        ValuePtr result         = Value::create(std::pow(this_value->m_value, exponent));
        result->m_backward_func = [this_value, exponent](ValuePtr out) {
            this_value->m_gradient +=
              exponent * std::pow(this_value->m_value, exponent - 1) * out->m_gradient;
        };
        return result;
    }

    void add_gradient(f64 rhs) {
        m_gradient += rhs;
    }

    friend ValuePtr operator-(ValuePtr a) {
        ValuePtr result         = Value::create(-a->m_value);
        result->m_backward_func = [a](ValuePtr out) {
            f64 grad = -out->m_gradient;
            a->m_gradient += grad;
        };
        return result;
    }

    friend ValuePtr operator+(ValuePtr a, ValuePtr b) {
        ValuePtr result         = Value::create(a->m_value + b->m_value);
        result->m_backward_func = [a, b](ValuePtr out) {
            a->m_gradient += out->m_gradient;
            b->m_gradient += out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator-(ValuePtr a,
                              ValuePtr b) {  // We are NOT cheaping out with a + (-b)
        ValuePtr result         = Value::create(a->m_value - b->m_value);
        result->m_backward_func = [a, b](ValuePtr out) {
            a->m_gradient += out->m_gradient;
            b->m_gradient -= out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator*(ValuePtr a, ValuePtr b) {
        ValuePtr result         = Value::create(a->m_value * b->m_value);
        result->m_backward_func = [a, b](ValuePtr out) {
            a->m_gradient += b->m_value * out->m_gradient;
            b->m_gradient += a->m_value * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator/(ValuePtr a,
                              ValuePtr b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
        ValuePtr result         = Value::create(a->m_value / b->m_value);
        result->m_backward_func = [a, b](ValuePtr out) {
            a->m_gradient += 1.0 / b->m_value * out->m_gradient;
            b->m_gradient += -a->m_value / (b->m_value * b->m_value) * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator+(ValuePtr a, f64 b) {
        ValuePtr result         = Value::create(a->m_value + b);
        result->m_backward_func = [a](ValuePtr out) {
            a->m_gradient += out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator-(ValuePtr a, f64 b) {
        ValuePtr result         = Value::create(a->m_value - b);
        result->m_backward_func = [a](ValuePtr out) {
            a->m_gradient += out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator*(ValuePtr a, f64 b) {
        ValuePtr result         = Value::create(a->m_value * b);
        result->m_backward_func = [a, b](ValuePtr out) {
            a->m_gradient += b * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator/(ValuePtr a,
                              f64      b) {  // We are NOT cheaping out with a * (std::pow(b,-1))
        ValuePtr result         = Value::create(a->m_value / b);
        result->m_backward_func = [a, b](ValuePtr out) {
            a->m_gradient += 1.0 / b * out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator+(f64 a, ValuePtr b) {
        return b + a;
    }

    friend ValuePtr operator-(f64 a, ValuePtr b) {
        ValuePtr result         = Value::create(a - b->m_value);
        result->m_backward_func = [b](ValuePtr out) {
            b->m_gradient -= out->m_gradient;
        };
        return result;
    }

    friend ValuePtr operator*(f64 a, ValuePtr b) {
        return b * a;
    }

    friend ValuePtr operator/(f64 a, ValuePtr b) {
        ValuePtr result         = Value::create(a / b->m_value);
        result->m_backward_func = [a, b](ValuePtr out) {
            b->m_gradient += -a / (b->m_value * b->m_value) * out->m_gradient;
        };
        return result;
    }

    static ValuePtr sum(const std::vector<ValuePtr>& inputs) {
        if (inputs.empty()) {
            return Value::create(0.0);
        }

        f64 sum = 0.0;
        f64 c   = 0.0;
        for (auto& v : inputs) {
            f64 y = v->m_value - c;
            f64 t = sum + y;
            c     = (t - sum) - y;
            sum   = t;
        }

        ValuePtr result = Value::create(sum);

        result->m_backward_func = [inputs](ValuePtr out) {
            f64 grad = out->m_gradient;
            for (auto& v : inputs) {
                v->m_gradient += grad;
            }
        };

        return result;
    }


    friend bool operator==(ValuePtr a, ValuePtr b) {
        return a->m_value == b->m_value;
    }

    friend bool operator!=(ValuePtr a, ValuePtr b) {
        return a->m_value != b->m_value;
    }

    friend bool operator<(ValuePtr a, ValuePtr b) {
        return a->m_value < b->m_value;
    }

    friend bool operator<=(ValuePtr a, ValuePtr b) {
        return a->m_value <= b->m_value;
    }

    friend bool operator>(ValuePtr a, ValuePtr b) {
        return a->m_value > b->m_value;
    }

    friend bool operator>=(ValuePtr a, ValuePtr b) {
        return a->m_value >= b->m_value;
    }

    friend std::ostream& operator<<(std::ostream& os, const ValuePtr& value) {
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

class Pair : public SmartBackwardable<Pair> {
private:
    std::function<void(PairPtr)> m_backward_func;
    bool                         m_constant;

public:
    friend class Graph;
    friend class Value;

    f128 m_values;     // stores (first, second)
    f128 m_gradients;  // stores (grad_first, grad_second)

    explicit Pair(f64 first, f64 second, bool constant = false) :
        m_constant(constant),
        m_values(f128::make(first, second)),
        m_gradients(f128::zero()) {
    }

    explicit Pair(const f128& values, bool constant = false) :
        m_constant(constant),
        m_values(values),
        m_gradients(f128::zero()) {
    }

    static PairPtr create(f64 first, f64 second);

    static PairPtr create(const f128& values);

    inline f64 first() const {
        return m_values.first();
    }

    inline f64 second() const {
        return m_values.second();
    }

    inline f128 get_values() const {
        return m_values;
    }

    inline f64 grad_first() const {
        return m_gradients.first();
    }

    inline f64 grad_second() const {
        return m_gradients.second();
    }

    inline f128 get_graidents() const {
        return m_gradients;
    }

    inline void zero_grad() {
        m_gradients = f128::zero();
    }

    inline void set_values(const f128& values) {
        m_values = values;
    }
    inline void set_values(f64 first, f64 second) {
        m_values = f128::make(first, second);
    }

    // ------------------- Backward -------------------
    void backward() override {
        auto self = this->shared_from_this();
        if (m_backward_func) {
            m_backward_func(self);
        }
    }

    // ------------------- Arithmetic -------------------
    friend PairPtr operator+(const PairPtr& a, const PairPtr& b) {
        f128    result_values = f128::add(a->m_values, b->m_values);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, b](PairPtr out) {
            a->m_gradients = f128::add(a->m_gradients, out->m_gradients);
            b->m_gradients = f128::add(b->m_gradients, out->m_gradients);
        };
        return result;
    }

    friend PairPtr operator-(const PairPtr& a, const PairPtr& b) {
        f128    result_values = f128::sub(a->m_values, b->m_values);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, b](PairPtr out) {
            a->m_gradients = f128::add(a->m_gradients, out->m_gradients);
            b->m_gradients = f128::sub(b->m_gradients, out->m_gradients);
        };
        return result;
    }

    // ------------------- Scalar multiplication -------------------
    friend PairPtr operator*(const PairPtr& a, f64 scalar) {
        f128    result_values = f128::mul_scalar(a->m_values, scalar);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, scalar](PairPtr out) {
            f128 scaled_grad = f128::mul_scalar(out->m_gradients, scalar);
            a->m_gradients   = f128::add(a->m_gradients, scaled_grad);
        };
        return result;
    }

    friend PairPtr operator*(f64 scalar, const PairPtr& a) {
        return a * scalar;
    }

    // ------------------- Pair * Value -------------------
    friend PairPtr operator*(const PairPtr& a, const ValuePtr& v) {
        f64     v_val         = v->get_value();
        f128    result_values = f128::mul_scalar(a->m_values, v_val);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, v](PairPtr out) {
            f64  v_val       = v->get_value();
            f128 scaled_grad = f128::mul_scalar(out->m_gradients, v_val);
            a->m_gradients   = f128::add(a->m_gradients, scaled_grad);

            f128 contribution = f128::mul(a->m_values, out->m_gradients);
            v->add_gradient(contribution.first() + contribution.second());
        };
        return result;
    }

    friend PairPtr operator*(const ValuePtr& v, const PairPtr& a) {
        return a * v;
    }

    // ------------------- Pair / scalar -------------------
    friend PairPtr operator/(const PairPtr& a, f64 scalar) {
        f128    result_values = f128::div_scalar(a->m_values, scalar);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, scalar](PairPtr out) {
            f128 scaled_grad = f128::div_scalar(out->m_gradients, scalar);
            a->m_gradients   = f128::add(a->m_gradients, scaled_grad);
        };
        return result;
    }

    // ------------------- Scalar / Pair -------------------
    friend PairPtr operator/(f64 scalar, const PairPtr& a) {
        f128    result_values = f128::scalar_div(scalar, a->m_values);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, scalar](PairPtr out) {
            f128 a_squared            = f128::mul(a->m_values, a->m_values);
            f128 neg_scalar_over_a_sq = f128::neg(f128::scalar_div(scalar, a_squared));
            f128 grad_update          = f128::mul(neg_scalar_over_a_sq, out->m_gradients);
            a->m_gradients            = f128::add(a->m_gradients, grad_update);
        };
        return result;
    }

    // ------------------- Pair / Value -------------------
    friend PairPtr operator/(const PairPtr& a, const ValuePtr& v) {
        f64     v_val         = v->get_value();
        f128    result_values = f128::div_scalar(a->m_values, v_val);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, v](PairPtr out) {
            f64  v_val       = v->get_value();
            f128 scaled_grad = f128::div_scalar(out->m_gradients, v_val);
            a->m_gradients   = f128::add(a->m_gradients, scaled_grad);

            f128 numerator        = f128::mul(a->m_values, out->m_gradients);
            f64  sum_contribution = numerator.first() + numerator.second();
            v->add_gradient(-sum_contribution / (v_val * v_val));
        };
        return result;
    }

    // ------------------- Value / Pair -------------------
    friend PairPtr operator/(const ValuePtr& v, const PairPtr& a) {
        f64     v_val         = v->get_value();
        f128    result_values = f128::scalar_div(v_val, a->m_values);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a, v](PairPtr out) {
            f64  v_val           = v->get_value();
            f128 a_squared       = f128::mul(a->m_values, a->m_values);
            f128 neg_v_over_a_sq = f128::neg(f128::scalar_div(v_val, a_squared));
            f128 grad_update     = f128::mul(neg_v_over_a_sq, out->m_gradients);
            a->m_gradients       = f128::add(a->m_gradients, grad_update);

            f128 v_grad_contrib = f128::div(out->m_gradients, a->m_values);
            v->add_gradient(v_grad_contrib.first() + v_grad_contrib.second());
        };
        return result;
    }

    // ------------------- Unary negation -------------------
    friend PairPtr operator-(PairPtr a) {
        f128    result_values = f128::neg(a->m_values);
        PairPtr result        = Pair::create(result_values);

        result->m_backward_func = [a](PairPtr out) {
            a->m_gradients = f128::sub(a->m_gradients, out->m_gradients);
        };
        return result;
    }

    // ------------------- Phasing -------------------
    template<i32 max>
    ValuePtr phase(f64 alpha) {
        alpha /= max;
        auto self = this->shared_from_this();

        // Linear interpolation: alpha * first + (1-alpha) * second
        f64      result_val = alpha * first() + (1.0 - alpha) * second();
        ValuePtr result     = Value::create(result_val);

        result->m_backward_func = [self, alpha](ValuePtr out) {
            // Gradient of output w.r.t first and second
            f64  grad_first   = alpha * out->m_gradient;
            f64  grad_second  = (1.0 - alpha) * out->m_gradient;
            f128 grad_update  = f128::make(grad_first, grad_second);
            self->m_gradients = f128::add(self->m_gradients, grad_update);
        };

        return result;
    }

    // ------------------- Print -------------------
    friend std::ostream& operator<<(std::ostream& os, const PairPtr& p) {
#ifdef VERBOSE_AUTOGRAD
        os << "Pair(first=" << p->first() << ", second=" << p->second()
           << ", grad_first=" << p->grad_first() << ", grad_second=" << p->grad_second() << ")";
#else
        os << (p->m_constant ? "CS" : "S");
        os << "(" << std::round(p->first()) << "," << std::round(p->second()) << ")";
#endif
        return os;
    }
};

// Inplace ops (we can't do them as member functions because we use shared pointers)

// ValuePtr += ValuePtr
inline ValuePtr& operator+=(ValuePtr& a, const ValuePtr& b) {
    a = a + b;
    return a;
}

// ValuePtr -= ValuePtr
inline ValuePtr& operator-=(ValuePtr& a, const ValuePtr& b) {
    a = a - b;
    return a;
}

// ValuePtr *= ValuePtr
inline ValuePtr& operator*=(ValuePtr& a, const ValuePtr& b) {
    a = a * b;
    return a;
}

// ValuePtr /= ValuePtr
inline ValuePtr& operator/=(ValuePtr& a, const ValuePtr& b) {
    a = a / b;
    return a;
}


// ValuePtr += scalar
inline ValuePtr& operator+=(ValuePtr& a, f64 b) {
    a = a + b;
    return a;
}

// ValuePtr -= scalar
inline ValuePtr& operator-=(ValuePtr& a, f64 b) {
    a = a - b;
    return a;
}

// ValuePtr *= scalar
inline ValuePtr& operator*=(ValuePtr& a, f64 b) {
    a = a * b;
    return a;
}

// ValuePtr /= scalar
inline ValuePtr& operator/=(ValuePtr& a, f64 b) {
    a = a / b;
    return a;
}

// PairPtr += PairPtr
inline PairPtr& operator+=(PairPtr& a, const PairPtr& b) {
    a = a + b;
    return a;
}

// PairPtr -= PairPtr
inline PairPtr& operator-=(PairPtr& a, const PairPtr& b) {
    a = a - b;
    return a;
}

// PairPtr *= scalar
inline PairPtr& operator*=(PairPtr& a, f64 scalar) {
    a = a * scalar;
    return a;
}

// PairPtr *= ValuePtr
inline PairPtr& operator*=(PairPtr& a, const ValuePtr& v) {
    a = a * v;
    return a;
}

// PairPtr /= scalar
inline PairPtr& operator/=(PairPtr& a, f64 scalar) {
    a = a / scalar;
    return a;
}

// PairPtr /= ValuePtr
inline PairPtr& operator/=(PairPtr& a, const ValuePtr& v) {
    a = a / v;
    return a;
}

}  // namespace Clockwork::Autograd
