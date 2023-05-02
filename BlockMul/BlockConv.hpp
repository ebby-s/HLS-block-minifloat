#ifndef CONV
#define CONV


template<int N, int E, int M>
template<int W, int F>
BlockMF<N,E,M>::operator BlockFP<N,W,F>() const{

    // Declare intermediate and output.
    BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2> conv;
    BlockFP<N,W,F> out;

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

            conv_int.acc <<= (exp - 1);

            if(sgn) conv_int.acc *= -1;

            conv.data[i][j] = conv_int;

            // Update shift_amt based on leading 0s or 1s.
            ldz = 0;
            ldo = 0;

            for(int k=WPRD(E,M)/2-1; ((k>=0) && conv.data[i][j].acc[k]); k--)
                ldo++;

            for(int k=WPRD(E,M)/2-1; ((k>=0) && !conv.data[i][j].acc[k]); k--)
                ldz++;

            ldd = (ldo == 0) ? ldz : ldo;

            if((ldd-1) < shift_amt){
                shift_amt = (ldd-1);
            }

            if((shift_amt == (WPRD(E,M)/2-1)) && (ldz == 0)){
                zero_data = false;
            }
        }
    }

    // Calculate and adjust output bias based on shift_amt.
    // Check if data is all zero, adjust bias_sum.
    if(shift_amt == (WPRD(E,M)/2-1) && zero_data){
        shift_amt = WPRD(E,M)/2;
        conv.bias = 0;
    }else{
        conv.bias = bias - shift_amt;
    }

    // Apply shift to data.
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            conv.data[i][j].acc = conv.data[i][j].acc << shift_amt;
        }
    }

    // Convert widths to desired output width.
    if(W >= (WPRD(E,M)/2)){

        for (int i=0; i<N; i++) {
            #pragma HLS unroll

            for (int j=0; j<N; j++) {
                #pragma HLS unroll

                IntAcc<W,F> out_int;
                out_int.acc = conv.data[i][j].acc;
                out_int.acc <<= (W - WPRD(E,M)/2);
                out.data[i][j] = out_int;
            }
        }

        out.bias = conv.bias + (F - (FPRD(E,M)/2)) - (W - (WPRD(E,M)/2));

    // If desired width is short, add rounding (round towards zero).
    }else{

        for (int i=0; i<N; i++) {
            #pragma HLS unroll

            for (int j=0; j<N; j++) {
                #pragma HLS unroll

                IntAcc<W,F> out_int;
                conv.data[i][j].acc >>= (WPRD(E,M)/2 - W);
                out_int.acc = conv.data[i][j].acc;
                out.data[i][j] = out_int;
            }
        }

        out.bias = conv.bias + (F - (FPRD(E,M)/2)) - (W - (WPRD(E,M)/2));
    }

    return out;
}



// Convert BFP to BMF.
template <int N, int W, int F>
template <int E, int M>
BlockFP<N,W,F>::operator BlockMF<N,E,M>() const{

    BlockMF<N,E,M> out;

    for (int i=0; i<N; i++) {
        #pragma HLS unroll

        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            MiniFloat<E,M> out_mf;

            out_mf.data = 0;

            // Round data to maximum width representable by output format.
            ap_int<WPRD(E,M)> data_rnd = rnd_method->round(data[i][j].acc);

            // Extract sign.
            out_mf.data |= (data_rnd < 0) << (E+M);

            ap_uint<WPRD(E,M)> data_u = std::abs(data_rnd);

            // Count leading zeros, remove leading zeros from data.
            ap_uint<CLOG2(WPRD(E,M)+1)> ldz = 0;

            for(int k=WPRD(E,M)-1; ((k>=0) && !data_u[k]); k--)
                ldz++;
            
            ap_uint<WPRD(E,M)> data_shift = data_u << ldz;

            // Calculate mantissa and exponent.
            ap_uint<M+1> man_ext = rnd_method->round(data_shift);



            out.data[i][j] = out_mf;
        }
    }

    return out;
}



#endif
