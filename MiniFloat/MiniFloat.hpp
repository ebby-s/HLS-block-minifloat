#ifndef MINIFLOAT
#define MINIFLOAT

#include "ap_int.h"    // Use arbitrary precision types.

// #define CL2_mask(i,n) (n & ((1<<i)-1))
// #define CL2_i1(n) (((n) >= 1<<1)  ? 1 + |CL2_mask(1,n) : 0)
// #define CL2_i2(n) (((n) >= 1<<2)  ? (2 + CL2_i1((n)>>2) + |CL2_mask(2,n)) : CL2_i1(n))
// #define CL2_i4(n) (((n) >= 1<<4)  ? (4 + CL2_i2((n)>>4) + |CL2_mask(4,n)) : CL2_i2(n))
// #define CLOG2(n)  (((n) >= 1<<8)  ? (8 + CL2_i4((n)>>8) + |CL2_mask(8,n)) : CL2_i4(n))

#define WfromEM(E,M) (((1<<E)+M)*2)
#define FfromEM(E,M) (2*(M-1))

template <int A, int B> struct max_value{
    static const int value = A > B ? A : B;
};


template <int E, int M> struct MiniFloat;     // MiniFloat.
template <int W, int F> struct KulischAcc;    // Fixed Point Accumulator.
// W is the accumulator size, the value of the LSB is 2^F.
// Eg. F = -2*(M-1) when representing the product of 2 MiniFloats (assume bias=0).


template <int E, int M> struct MiniFloat{

    // Represents sign, exponent and optional mantissa in that order.
    ap_uint<1+E+M> data;

    // Multiply a pair of MiniFloats and produce a KulischAcc.
    KulischAcc<WfromEM(E,M), FfromEM(E,M)> operator *(const MiniFloat<E,M> &op);

    //////----Methods for verification, do not synthesize.----//////
    MiniFloat(){}

    // Constructor to initialize data before tests.
    MiniFloat(int op){
        data = op;
    }

    // Convert to cpp float. Assume bias=0.
    operator float() const{

        ap_uint<1> sgn = data >> (E+M);
        ap_uint<E> exp = (data >> M) & ((1<<E)-1);

        float out = data & ((1<<M)-1);

        out /= (1 << M);
        if(exp){
            out += 1;
        }else{
            out *= 2;
        }
        out *= (1 << exp);
        if(sgn) out *= -1;
        return out;
    }
};



template <int W, int F> struct KulischAcc{

    // Represents a signed fixed point number.
    ap_int<W> acc;

    // // Add a pair of fi numbers with equal width.
    // KulischAcc<W+1,F> operator +(const KulischAcc<W,F> &op);

    // Add a pair of fi numbers with arbitrary widths.
    template<int W2>
    inline KulischAcc<max_value<W,W2>::value+1,F> operator +(const KulischAcc<W2,F> &op);

    //////----Methods for verification, do not synthesize.----//////
    KulischAcc(){}

    // Constructor to initialize data before tests.
    template <typename T>
    KulischAcc(T op){
        acc = op;
    }

    // Convert to cpp float. Assume bias=0.
    operator float() const{
        float out = acc;
        out /= pow(2,F);
        return out;
    }
};

// Include implementations of scalar multiplication and addition.
#include "ScalarOperator.hpp"

#endif
