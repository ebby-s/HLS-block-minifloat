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

            if((W-1) >= (1<<E)){
                if(ldz < ((1<<E)-1)){
                    out_mf.data |= (((1<<E)-1)-ldz) << M;
                }
                if(ldz >= ((1<<E)-1)){
                    man_ext <<= ((1<<E)-1)-1;
                }else{
                    man_ext <<= ldz;
                }
            }else{
                if(man_ext != 0){
                    out_mf.data |= (W-ldz-1) << M;
                }
                if(ldz == (W-1)){
                    man_ext <<= ldz-1;
                }else{
                    man_ext <<= ldz;
                }
            }

            // Round integer to fit in mantissa.
            if(W < (M+1)){
                out_mf.data |= (ap_uint<M>(man_ext) << (M+1-W)) & ((1<<M)-1);
            }else{
                out_mf.data |= (rnd_method->rnd_bmf(man_ext, M+1)) & ((1<<M)-1);
            }

            out.data[i][j] = out_mf;
        }
    }

    if((W-1) >= (1<<E)){
        out.bias = bias + W - (1<<E) - F;
    }else{
        out.bias = bias - F;
    }

    return out;
}


// Round down BFP.
template <int N, int W, int F>
template <int Wo, int Fo>
BlockFP<N,W,F>::operator BlockFP<N,Wo,Fo>() const{

    BlockFP<N,Wo+1,Fo> rnd;
    BlockFP<N,Wo  ,Fo> out;
    // Records if any rounding operations overflowed.
    bool rnd_ofl = false;

    // Convert integers to MiniFloats.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll
        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            IntAcc<Wo+1,Fo> rnd_fp;

            rnd_fp.acc = rnd_method->rnd_bfp(data[i][j].acc, Wo);
            rnd_ofl |= (rnd_fp.acc[Wo] ^ rnd_fp.acc[Wo-1]);
            rnd.data[i][j] = rnd_fp;
        }
    }

    // Assign output, shift if rounding caused overflow.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll
        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            IntAcc<Wo,Fo> out_fp;
            out_fp.acc = rnd[i][j].acc >> rnd_ofl;
            out.data[i][j] = out_fp;
        }
    }
    // Adjust bias if rounding caused overflow.
    out.bias = bias + ((W-F)-(Wo-Fo)) + rnd_ofl;

    return out;
}


#endif
