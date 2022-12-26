#ifndef SCALAR_OP
#define SCALAR_OP

#include "../MiniFloat/MiniFloat.hpp"


// Used in multiplier, mantissa with implicit 1 appended as MSB.
typedef ap_uint<M+1> man_prime_t;

// Multiply a pair of minifloats, produce error-free output.
mul_out_t operator *(minifloat_t op0, minifloat_t op1);

// Sum a pair of error-free multiplier outputs using Kulisch Accumulator.
ap_uint<W_A> operator +(mul_out_t op0, mul_out_t op1);

// Sum Kulisch Accumulators without errors.
template <int N>
ap_uint<N+1> operator +(ap_uint<N> op0, ap_uint<N> op1);



#endif
