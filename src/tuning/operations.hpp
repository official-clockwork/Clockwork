#pragma once

#include "util/types.hpp"

namespace Clockwork::Autograd {

enum class OpType : u8 {
    // --- Leaf / Special ---
    None,
    Parameter,  // Created from a global parameter
    Input,      // Created manually (e.g. from data)

    // --- Value Binary Ops ---
    Add,
    Sub,
    Mul,
    Div,
    Pow,

    // --- Value Unary Ops ---
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

    // --- Pair Ops ---
    PairCreate,  // (val, val) -> pair
    PairAdd,
    PairSub,
    PairNeg,

    // --- Pair/Scalar Ops ---
    PairMulScalar,
    PairDivScalar,
    ScalarDivPair,

    // --- Pair/Value Ops ---
    PairMulValue,
    ValueMulPair,  // Commutative wrapper usually, but distinct op code helps
    PairDivValue,
    ValueDivPair,

    // --- Phasing ---
    Phase,  // Pair -> Value via alpha

    // --- Reduction ---
    Sum  // Sum of a vector of values
};

// A single node in the compute tape.
// Designed to be compact and fit in cache lines.
struct Node {
    OpType type;

    u32 output_idx;  // Index in the respective arena (Value or Pair)
    u32 lhs_idx;     // Index of first operand
    u32 rhs_idx;     // Index of second operand (if applicable)

    // Auxiliary data for scalar ops, constants, or specific parameters
    f64 scalar_data;

    // Helper to handle the "Reduction" case (Sum) where we have >2 inputs.
    // In a pure tape, we might handle Sum by chaining Adds, or storing an index
    // to a side-table of indices. For simplicity/speed in this specific codebase,
    // we can implement Sum as a sequence of Adds or use a special range.
    // For now, we will implement Sum as a chain of binary adds in the graph
    // builder to keep the Node struct fixed-size and simple.
};

}  // namespace Clockwork::Autograd
