#ifndef MINIFLOAT
#define MINIFLOAT

#include "ap_int.h"    // Use arbitrary precision types.


template <int E, int M> struct MiniFloat;     // MiniFloat.
template <int N, int F> class  KulischAcc;    // Fixed Point Accumulator.
// N is the accumulator size, the value of the LSB is 2^F.
// Eg. F = -2*(M-1) when representing the product of 2 MiniFloats (assume bias=0).


template <int E, int M> struct MiniFloat{

    // Represents sign, exponent and optional mantissa in that order.
    ap_uint<1+E+M> data;

    // Multiply a pair of MiniFloats and produce a KulischAcc.
    KulischAcc<((1<<E)+M)*2, 2*(M-1)> operator *(const MiniFloat<E,M> &op);

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



template <int N, int F> class KulischAcc{

    protected:
    ap_int<N> acc;

    public:
    KulischAcc(){}

    template <typename T>
    KulischAcc(T op){
        acc = op;
    }

    template <typename U>
    KulischAcc<N,F> operator *=(U op){
        acc *= op;
        return *this;
    }

    template <typename V>
    KulischAcc<N,F> operator <<=(V op){
        acc <<= op;
        return *this;
    }

    KulischAcc<N+1,F> operator +(const KulischAcc<N,F> &op);

    // Methods for verification, not used for synthesis.
    operator float() const{  // Assume f32 has enough precision to convert exactly.
        float out = acc;
        out /= pow(2,F);
        return out;
    }

    friend class KulischAcc<N-1,F>;
};

// Include implementations of scalar multiplication and addition.
#include "ScalarOperator.hpp"

#endif
