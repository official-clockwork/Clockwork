#pragma once

#include "util/types.hpp"
#include "value.hpp"
#include <cassert>
#include <variant>

namespace Clockwork::Autograd {

enum class OpType : u32 {
    // Leaf nodes
    None,
    Parameter,  // Created from a global parameter
    Input,      // Created manually (e.g. from data)

    // Binary Ops
    Add,
    Sub,
    Mul,
    Div,
    Pow,

    // Unary Ops
    Exp,
    Log,
    Sigmoid,
    Neg,
    PowConst,      // x ^ scalar
    AddScalar,     // x + scalar
    SubScalarVal,  // scalar - x
    ValSubScalar,  // x - scalar
    MulScalar,     // x * scalar
    DivScalarVal,  // scalar / x
    ValDivScalar,  // x / scalar

    // Pair Ops
    PairCreate,
    PairAdd,
    PairSub,
    PairNeg,
    PairSigmoid,

    // Pair-Scalar Ops
    PairMulScalar,
    PairDivScalar,
    ScalarDivPair,

    // Pair-Value Ops
    PairMulValue,
    ValueMulPair,
    PairDivValue,
    ValueDivPair,

    // Pair-Pair Ops
    PairMulPair,

    // Phasing
    Phase,  // Pair -> Value via alpha

    // Reduction (TODO: optimize this)
    Sum  // Sum of a vector of values
};


// Node layout (16 bytes):
// [0..3]  : type (1), pad (1), lhs_offset (2)   -> 4 bytes
// [4..7]  : output_idx                            -> 4 bytes
// [8..15] : union { struct { u16 rhs_offset; u16 pad2; u32 pad3; } ; double scalar; } -> 8 bytes
struct alignas(16) Node {

    OpType type;  // u32
    u32    lhs_idx;
    u32    output_idx;

    union U {
        u32 rhs_idx;      // for unary/binary ops
        f32 scalar_data;  // for scalar ops

        constexpr U() :
            rhs_idx(0) {
        }
        constexpr U(u16 rhs_idx) :
            rhs_idx(rhs_idx) {
        }
        constexpr U(f64 scalar) :
            scalar_data(static_cast<f32>(scalar)) {
        }
    } u;

    static constexpr Node make_binary(OpType t, u32 output_idx, u32 lhs_idx, u32 rhs_idx) {
        Node n{};
        n.type = t;

        // lhs & rhs indices are guaranteed to be <= out
        n.lhs_idx    = lhs_idx;
        n.output_idx = output_idx;
        n.u.rhs_idx  = rhs_idx;

        return n;
    }

    static constexpr Node make_scalar(OpType t, u32 output_idx, u32 lhs_idx, f64 scalar) {
        Node n{};
        n.type          = t;
        n.lhs_idx       = lhs_idx;
        n.output_idx    = output_idx;
        n.u.scalar_data = static_cast<f32>(scalar);
        return n;
    }

    constexpr u32 lhs() const noexcept {
        return lhs_idx;
    }

    constexpr u32 rhs() const noexcept {
        return u.rhs_idx;
    }

    constexpr u32 out() const noexcept {
        return output_idx;
    }

    constexpr f64 scalar() const noexcept {
        return u.scalar_data;
    }
};

static_assert(sizeof(Node) == 16, "Node must be exactly 16 bytes");
static_assert(alignof(Node) == 16, "Node alignment must match double alignment (8 bytes)");

}  // namespace Clockwork::Autograd
