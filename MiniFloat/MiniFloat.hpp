#ifndef MINIFLOAT
#define MINIFLOAT

#include "ap_int.h"    // Use arbitrary precision types.

// Macro for ceil(log2(n)) in 1 <= n < 2^16. Use CLOG2(n).
#define CLG_1(n) (((n) >= 1<<1) ? 1 : 0)
#define CLG_2(n) (((n) >= 1<<2) ? (2 + CLG_1((n)>>2)) : CLG_1(n))
#define CLG_4(n) (((n) >= 1<<4) ? (4 + CLG_2((n)>>4)) : CLG_2(n))
#define CLG_8(n) (((n) >= 1<<8) ? (8 + CLG_4((n)>>8)) : CLG_4(n))
#define CLOG2(n) ((n > (1<<CLG_8(n))) ? (CLG_8(n)+1) : CLG_8(n))

// Macro for max(a,b).
#define MAX(A,B) ((A > B) ? A : B)

// Macros for calculating W and F from E and M, without loss of precision.
// F = -2*(M-1) when representing the product of 2 MiniFloats (assume bias=0).
#define WPRD(E,M) (2*((1<<E)+M))
#define FPRD(E,M) (2*(M-1))


template <int E, int M> struct MiniFloat;     // MiniFloat.
template <int W, int F> struct IntAcc;    // Fixed Point Accumulator.


template <int E, int M> struct MiniFloat{

    // Represents sign, exponent and optional mantissa in that order.
    ap_uint<1+E+M> data;

    // Multiply a pair of MiniFloats and produce an IntAcc.
    inline IntAcc<WPRD(E,M), FPRD(E,M)> operator *(const MiniFloat<E,M> &op);

    //////----Methods for verification, do not synthesize.----//////
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

    // Convert to cpp double. Assume bias=0.
    operator double() const{

        ap_uint<1> sgn = data >> (E+M);
        ap_uint<E> exp = (data >> M) & ((1<<E)-1);

        double out = data & ((1<<M)-1);

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


template <int W, int F> struct IntAcc{

    // Represents a signed fixed point number.
    ap_int<W> acc;

    // Convert to different width.
    template<int Wo>
    inline operator IntAcc<Wo,F>() const{
        IntAcc<Wo,F> out;
        out.acc = acc;
        return out;
    }

    inline IntAcc<2*W,2*F> operator *(const IntAcc<W,F> &op);

    // Add a pair of fi numbers with arbitrary widths.
    template<int W2>
    inline IntAcc<MAX(W,W2)+1,F> operator +(const IntAcc<W2,F> &op);

    // // Add a pair of fi numbers with equal width.
    // IntAcc<W+1,F> operator +(const IntAcc<W,F> &op);

    //////----Methods for verification, do not synthesize.----//////
    // Convert to cpp float. Assume bias=0.
    operator float() const{
        float out = acc;
        out /= pow(2,F);
        return out;
    }

    // Convert to cpp double. Assume bias=0.
    operator double() const{
        double out = acc;
        out /= pow(2,F);
        return out;
    }
};

// Include implementations of scalar multiplication and addition.
#include "ScalarOperator.hpp"

#endif
