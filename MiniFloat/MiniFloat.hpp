#ifndef MINIFLOAT
#define MINIFLOAT

#include "ap_int.h"    // Use arbitrary precision types.


template <int E, int M> class MiniFloat;     // MiniFloat.
template <int N, int F> class KulischAcc;    // Fixed Point Accumulator.


template <int E, int M> class MiniFloat{

    protected:
    ap_uint<1> sgn;
    ap_uint<E> exp;
    ap_uint<M> man;

    public:
    MiniFloat(){}

    KulischAcc<((1<<E)+M)*2, 2*(M-1)> operator *(const MiniFloat<E,M> &op);

    // Methods for verification, not used for synthesis.
    MiniFloat(int op){
        sgn = op >> (M+E);
        exp = op >>  M;
        if(M != 0)
            man = op;
    }

    operator float() const{  // Assume f32 has enough precision to convert exactly, and no bias.
        float out = 0;
        if(M != 0)
            out = man;
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



// N is the accumulator size, the value of the LSB is 2^F.
// Eg. F = -2*(M-1) when representing the product of 2 MiniFloat numbers.
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
    }

    template <typename V>
    KulischAcc<N,F> operator <<=(V op){
        acc <<= op;
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

#endif
