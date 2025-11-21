#pragma once

#include "util/types.hpp"

namespace Clockwork::Autograd {

enum class OpType : u8 {
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

    // Pair-Scalar Ops
    PairMulScalar,
    PairDivScalar,
    ScalarDivPair,

    // Pair-Value Ops
    PairMulValue,
    ValueMulPair,
    PairDivValue,
    ValueDivPair,

    // Phasing
    Phase,  // Pair -> Value via alpha

    // Reduction (TODO: optimize this)
    Sum  // Sum of a vector of values
};

// A single node in the compute tape. Probably can be rewritten more compactly.
struct Node {
    OpType type; // This tells us which arenas to look at and how to interpret lhs/rhs

    u32 output_idx;  // Index in the respective arena (Value or Pair)
    u32 lhs_idx;     // Index of first operand
    u32 rhs_idx;     // Index of second operand (if applicable)

    // Auxiliary data for scalar ops, constants, or specific parameters.
    f64 scalar_data;
};

}  // namespace Clockwork::Autograd
