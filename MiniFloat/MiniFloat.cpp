#include "MiniFloat.hpp"


// Convert minifloat_t to float.
float mnf_to_f32(minifloat_t op){

    float out;

    out = op.man;

    out /= pow(2,M);

    if(op.exp != 0){
        out += 1;
    }else{
        out *= 2;
    }

    out *= pow(2, op.exp);

    if(op.sgn){
        out *= -1;
    }

    return out;
};

// Convert error-free multiplier output (mul_out_t) to float.
float mul_to_f32(mul_out_t mul_out){

    float out;

    out = mul_out.man;

    out *= pow(2, mul_out.exp - M*2);

    if(mul_out.sgn){
        out *= -1;
    }

    return out;
};

// Convert Kulisch Accumulator to float.
template <int N>
float fip_to_f32(ap_uint<N> op){

    float out;

    out /= pow(2, M-1);

    return out;
};




