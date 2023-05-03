#ifndef CONV
#define CONV


template<int N, int E, int M>
BlockMF<N,E,M>::operator BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2>() const{

    // Declare output.
    BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2> out;

    //////----Calculations for bias.----//////
    // ldz/ldo   -> leading zeroes/ones.
    // ldd       -> leading digits = max(ldz,ldo).
    // shift_amt -> initialize to max value.
    // zero_data -> if true and shift_amt is unchanged, set data to zero.
    ap_uint<CLOG2(WPRD(E,M)/2+1)> ldz, ldo, ldd, shift_amt = (WPRD(E,M)/2-1);
    bool zero_data = true;

    // Convert to BlockFP with no loss in accuracy.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll
        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            IntAcc<WPRD(E,M)/2,FPRD(E,M)/2> conv_int;

            // Extract sign and exponent.
            ap_uint<1> sgn = data[i][j].data >> (E+M);
            ap_uint<E> exp = (data[i][j].data >> M) & ((1<<E)-1);
            // Calculate mantissa.
            if(M == 0){
                conv_int.acc = (exp != 0);
            }else{
                conv_int.acc = (((exp != 0) << M) | (data[i][j].data & ((1<<M)-1)));
            }
            if(exp) conv_int.acc <<= exp-1;
            if(sgn) conv_int.acc *= -1;

            out.data[i][j] = conv_int;

            // Update shift_amt based on leading 0s or 1s.
            ldz = 0;
            ldo = 0;

            for(int k=WPRD(E,M)/2-1; ((k>=0) && out.data[i][j].acc[k]); k--)
                ldo++;

            for(int k=WPRD(E,M)/2-1; ((k>=0) && !out.data[i][j].acc[k]); k--)
                ldz++;

            ldd = (ldo == 0) ? ldz : ldo;

            if((ldd-1) < shift_amt)
                shift_amt = (ldd-1);

            if((shift_amt == (WPRD(E,M)/2-1)) && (ldz == 0))
                zero_data = false;
        }
    }

    // Calculate and adjust output bias based on shift_amt.
    // Check if data is all zero, adjust bias_sum.
    if(shift_amt == (WPRD(E,M)/2-1) && zero_data){
        shift_amt = WPRD(E,M)/2;
        out.bias = 0;
    }else{
        out.bias = bias - shift_amt;
    }

    // Apply shift to data.
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            out.data[i][j].acc = out.data[i][j].acc << shift_amt;
        }
    }

    return out;
}



// Convert BFP to BMF.
template <int N, int W, int F>
template <int E, int M>
BlockFP<N,W,F>::operator BlockMF<N,E,M>() const{

    BlockMF<N,E,M> out;

    // Find the largest and smallest values.
    // IntAcc<W,F> max_val, min_val;

    // max_val.acc = 0;
    // min_val.acc = 0;
    // for (int i=0; i<N; i++) {
    //     #pragma HLS unroll
    //     for (int j=0; j<N; j++) {
    //         #pragma HLS unroll

    //         max_val.acc = (data[i][j].acc > max_val.acc) ? data[i][j].acc : max_val.acc;
    //         min_val.acc = (data[i][j].acc < min_val.acc) ? data[i][j].acc : min_val.acc;
    //     }
    // }

    // Check if these values can be represented in output format.

    // Adjust data and bias if needed.

    // Convert integers to MiniFloats.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll
        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            MiniFloat<E,M> out_mf;
            ap_uint<W> man_ext;
            ap_uint<CLOG2(W+1)> ldz = 0;

            // Extract sign, take abs value of data.
            out_mf.data = (data[i][j].acc < 0) << (E+M);
            if(data[i][j].acc < 0){
                man_ext = -data[i][j].acc;
            }else{
                man_ext = data[i][j].acc;
            }

            // Count leading zeros, extract exponent.
            for(int k=W-1; ((k>=0) && (!man_ext[k])); k--)
                ldz++;

            if(man_ext != 0){
                out_mf.data |= (W-ldz-1) << M;
            }
            if(ldz == (W-1)){
                man_ext <<= ldz-1;
            }else{
                man_ext <<= ldz;
            }

            // Round integer to fit in mantissa.
            out_mf.data |= (rnd_method->rnd_bmf(man_ext, M+1)) & ((1<<M)-1);

            out.data[i][j] = out_mf;
        }
    }
    out.bias = bias - F;

    return out;
}



#endif
