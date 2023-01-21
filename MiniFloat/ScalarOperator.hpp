#ifndef SCALAR_OP
#define SCALAR_OP


// Multiply a pair of MiniFloats, produce error-free output. No NaN/Inf representation.
template <int E, int M>
KulischAcc<((1<<E)+M)*2, 2*(M-1)> MiniFloat<E,M>::operator *(const MiniFloat<E,M> &op){

    ap_uint<1> sgn = data >> (E+M);
    ap_uint<E> exp = (data >> M) & ((1<<E)-1);

    ap_uint<1> op_sgn = op.data >> (E+M);
    ap_uint<E> op_exp = (op.data >> M) & ((1<<E)-1);

    ap_uint<1>       prd_sgn;
    ap_uint<E+1>     prd_exp;
    ap_uint<(M+1)*2> prd_man;

    // Calculate sign of result.
    prd_sgn = sgn ^ op_sgn;

    // Calculate result exponent.
    prd_exp = exp + op_exp;

    if(M != 0){  // Denormals only exist when M > 0.
        prd_exp += (exp    == 0);
        prd_exp += (op_exp == 0);
    }

    // Calculate result mantissa.
    if(M == 0){
        prd_man = !((exp == 0) || (op_exp == 0));
    }else{
        prd_man = (((exp != 0) << M) | (data & ((1<<M)-1))) * (((op_exp != 0) << M) | (op.data & ((1<<M)-1)));
    }

    // Form fixed-point output.
    KulischAcc<((1<<E)+M)*2, 2*(M-1)> prd_fi;

    prd_fi = prd_man;

    prd_fi <<= (prd_exp - 2);

    if(prd_sgn)
        prd_fi *= -1;

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
