#ifndef SCALAR_OP
#define SCALAR_OP

#include "../MiniFloat/MiniFloat.hpp"


// Multiply a pair of MiniFloats, produce error-free output. No NaN/Inf representation.
template <int E, int M>
KulischAcc<((1<<E)+M)*2, -2*(M-1)> MiniFloat<E,M>::operator *(const MiniFloat<E,M> &op){

    // Status signals.
    bool op0_is_dnrm;
    bool op1_is_dnrm;
    bool op0_is_zero;
    bool op1_is_zero;

    op0_is_dnrm = (exp == 0) && (man != 0);
    op1_is_dnrm = (op.exp == 0) && (op.man != 0);

    op0_is_zero = (exp == 0) && (man == 0);
    op1_is_zero = (op.exp == 0) && (op.man == 0);

    // Append implicit 1 to mantissa if operand is not denormal.
    ap_uint<M+1> op0_man_prime;
    ap_uint<M+1> op1_man_prime;

    op0_man_prime = ((!op0_is_dnrm) << M) | man;
    op1_man_prime = ((!op1_is_dnrm) << M) | op.man;

    // Multiply operands.
    ap_uint<1>       prd_sgn;
    ap_uint<E+1>     prd_exp;
    ap_uint<(M+1)*2> prd_man;

    prd_sgn = sgn ^ op.sgn;

    if(op0_is_zero || op1_is_zero){
        prd_man = 0;
        prd_exp = 0;
    }else{
        prd_man = op0_man_prime * op1_man_prime;
        prd_exp = exp;
        prd_exp += op.exp;
        prd_exp += op0_is_dnrm;
        prd_exp += op1_is_dnrm;
        prd_exp -= 2;
    }

    // Form fixed-point output.
    KulischAcc<((1<<E)+M)*2, -2*(M-1)> prd_fi = prd_man;

    prd_fi = prd_man;

    prd_fi <<= prd_exp;

    if(prd_sgn){
        prd_fi *= -1;
    }

    return prd_fi;
}


// Multiply a pair of LogMiniFloats, produce error-free output. No NaN/Inf representation.
template <int E>
KulischAcc<(1<<E)*2, 2> LogMiniFloat<E>::operator *(const LogMiniFloat<E> &op){

    // Status signals.
    bool op0_is_zero;
    bool op1_is_zero;

    op0_is_zero = (exp == 0);
    op1_is_zero = (op.exp == 0);

    // Multiply operands.
    ap_uint<1>    prd_sgn;
    ap_uint<E+1>  prd_exp;

    prd_sgn = sgn ^ op.sgn;

    if(op0_is_zero || op1_is_zero){
        prd_exp = 0;
    }else{
        prd_exp = exp;
        prd_exp += op.exp;
        prd_exp -= 2;
    }

    // Form fixed-point output.
    KulischAcc<(1<<E)*2, 2> prd_fi = 0;

    if(!(op0_is_zero || op1_is_zero)) prd_fi = 1 << prd_exp;

    if(prd_sgn){
        prd_fi *= -1;
    }

    return prd_fi;
}


// Sum a pair of fixed point numbers without errors.
template <int N, int F>
KulischAcc<N+1,F> KulischAcc<N,F>::operator +(const KulischAcc<N,F> &op){

    KulischAcc<N+1,F> out;
    out.acc = acc;
    out.acc += op.acc;
    return out;
}

#endif
