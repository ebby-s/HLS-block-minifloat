#include "ScalarOperator.hpp"


// Multiplies a pair of minifloats and produces error-free output.
// No NaN/Inf, output is saturated in cases where those develop.
mul_out_t operator *(minifloat_t op0, minifloat_t op1){

    // Status signals.
    bool op0_is_dnrm;
    bool op0_is_zero;

    bool op1_is_dnrm;
    bool op1_is_zero;

    op0_is_dnrm = (op0.exp == 0) && (op0.man != 0);
    op1_is_dnrm = (op1.exp == 0) && (op1.man != 0);

    op0_is_zero = (op0.exp == 0) && (op0.man == 0);
    op1_is_zero = (op1.exp == 0) && (op1.man == 0);

    // Add MSB 1 if not denormal number.
    man_prime_t op0_man_prime;
    man_prime_t op1_man_prime;

    op0_man_prime = ((!op0_is_dnrm) << M) | op0.man;
    op1_man_prime = ((!op1_is_dnrm) << M) | op1.man;

    // Assign output.
    mul_out_t prd;

    prd.sgn = op0.sgn ^ op1.sgn;

    if(op0_is_zero || op1_is_zero){
        prd.man = 0;
        prd.exp = 0;
    }else{
        prd.man = op0_man_prime * op1_man_prime;
        prd.exp = op0.exp;
        prd.exp += op1.exp;
        prd.exp += op0_is_dnrm;
        prd.exp += op1_is_dnrm;
    }

    return prd;
};



ap_uint<W_A> operator +(mul_out_t op0, mul_out_t op1){

    ap_uint<W_PRD> op0_fi;
    ap_uint<W_PRD> op1_fi;

    op0_fi = op0.man << (op0.exp -2);

    op1_fi = op1.man << (op1.exp -2);

    if(op0.sgn){
        op0_fi *= -1;
    }

    if(op1.sgn){
        op1_fi *= -1;
    }

    ap_uint<W_A> acc;

    acc = op0_fi + op1_fi;

    return acc;
};


template <int N>
ap_uint<N+1> operator +(ap_uint<N> op0, ap_uint<N> op1){

    ap_uint<N+1> acc;

    acc = op0 + op1;

    return acc;
    
};






