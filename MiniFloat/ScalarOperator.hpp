#ifndef SCALAR_OP
#define SCALAR_OP


// Multiply a pair of MiniFloats, produce error-free output. No NaN/Inf representation.
template <int E, int M>
inline IntAcc<WPRD(E,M), FPRD(E,M)> MiniFloat<E,M>::operator *(const MiniFloat<E,M> &op){

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
    IntAcc<WPRD(E,M), FPRD(E,M)> prd_fi;

    prd_fi.acc = prd_man;

    prd_fi.acc <<= (prd_exp - 2);

    if(prd_sgn)
        prd_fi.acc *= -1;

    return prd_fi;
}

// Multiply a pair of fixed-point values, produce error-free output.
template <int W, int F>
inline IntAcc<2*W,2*F> IntAcc<W,F>::operator *(const IntAcc<W,F> &op){

    IntAcc<2*W,2*F> out;

    out.acc = acc * op.acc;

    return out;
}



// // Sum a pair of fixed point numbers of equal width.
// template <int W, int F>
// IntAcc<W+1,F> IntAcc<W,F>::operator +(const IntAcc<W,F> &op){
//     IntAcc<W+1,F> out;
//     out.acc = acc;
//     out.acc += op.acc;
//     return out;
// }

// Sum a pair of fixed point numbers of arbitrary widths.
template <int W, int F>
template <int W2>
inline IntAcc<MAX(W,W2)+1,F> IntAcc<W,F>::operator +(const IntAcc<W2,F> &op){

    IntAcc<MAX(W,W2)+1,F> out;

    out.acc = acc + op.acc;

    return out;
}

#endif
