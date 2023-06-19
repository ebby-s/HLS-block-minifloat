#ifndef ROUND
#define ROUND


// struct prng{
//     ap_uint<32> lfsr;

//     ap_uint<32> rand(){
//         bool b_32 = lfsr(31,31);
//         bool b_22 = lfsr(31,21);
//         bool b_2 = lfsr(31,1);
//         bool b_1 = lfsr(31,0);
//         bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;

//         lfsr >>= 1;
//         lfsr[31] = new_bit;

//         return lfsr;
//     }
// };


enum rnd_mode_t{   // Rounding modes enum.
    RTZ,        // Round nearest towards zero.
    RAZ,        // Round nearest away from zero.
    RNI,        // Round towards negative infinity.
    RPI,        // Round towards positive infinity.
    RNE,        // Round to nearest even.
    STOCHASTIC  // Stochastic Rounding
};

template<int N, int W, int F>
ap_int<W> rnd_RTZ(ap_int<W> op, int Wo){

    ap_int<W> rounded;

    rounded = op >> (W - Wo);

    if(op < 0){
        rounded += (op(W-Wo-1, 0) != 0);
    }

    return rounded;
}

template<int N, int W, int F>
ap_int<W> rnd_RAZ(ap_int<W> op, int Wo){

    ap_int<W> rounded;

    rounded = op >> (W - Wo);

    if(op > 0){
        rounded += (op(W-Wo-1, 0) != 0);
    }

    return rounded;
}

template<int N, int W, int F>
ap_int<W> rnd_RNI(ap_int<W> op, int Wo){
    return op >> (W - Wo);
}

template<int N, int W, int F>
ap_int<W> rnd_RPI(ap_int<W> op, int Wo){

    ap_int<W> rounded;

    rounded = op >> (W - Wo);
    rounded += (op(W-Wo-1, 0) != 0);

    return rounded;
}

template<int N, int W, int F>
ap_int<W> rnd_RNE(ap_int<W> op, int Wo){

    ap_int<W> rounded;
    bool g,r,s;

    if((W - Wo) == 1){
        g = op[0];
        r = 0;
        s = 0;
    }else if((W - Wo) == 2){
        g = op[1];
        r = op[0];
        s = 0;
    }else{
        g = op[W-Wo-1];
        r = op[W-Wo-2];
        s = (op(W-Wo-3,0) != 0);
    }

    rounded = op >> (W - Wo);

    if((g != 0) && (r || s || rounded[0])){
        rounded++;
    }

    return rounded;
}

template<int N, int W, int F>
ap_int<W> rnd_STOCHASTIC(ap_int<W> op, int Wo){

    ap_int<W> rounded;
    bool g,r,s;

    if((W - Wo) == 1){
        g = op[0];
        r = 0;
        s = 0;
    }else if((W - Wo) == 2){
        g = op[1];
        r = op[0];
        s = 0;
    }else{
        g = op[W-Wo-1];
        r = op[W-Wo-2];
        s = (op(W-Wo-3,0) != 0);
    }

    rounded = op >> (W - Wo);

    rounded <<= 3;
    rounded[2] = g;
    rounded[1] = r;
    rounded[0] = s;

    rounded += 0;  // Add 3-bit random number here.

    rounded >>= 3;

    return rounded;
}

#endif
