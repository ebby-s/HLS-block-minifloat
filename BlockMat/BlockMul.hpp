#ifndef DOT_PRD
#define DOT_PRD

#define CHECK_OF_UF false

template <int N, int E, int M>
BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> BlockMF<N,E,M>::operator *(BlockMF<N,E,M> &op){

    // Declare output BFP.
    BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> out;

    //////----Calculations for bias.----//////
    // ldz/ldo   -> leading zeroes/ones.
    // ldd       -> leading digits = max(ldz,ldo).
    // shift_amt -> initialize to max value.
    // zero_data -> if true and shift_amt is unchanged, set data to zero.
    ap_uint<CLOG2(WPRD(E,M)+CLOG2(N)+1)> ldz, ldo, ldd, shift_amt = (WPRD(E,M)+CLOG2(N)-1);
    bool zero_data = true;

    //////----Do matrix multiplication on minifloats and store results.----//////
    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    for (int i=0; i<N; i++) {
        #pragma HLS unroll

        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            out.data[i][j].acc = 0;

            for (int k=0; k<N; k++) {
                #pragma HLS unroll

                IntAcc<WPRD(E,M)+CLOG2(N), FPRD(E,M)> current, prd, sum;

                current = out.data[i][j];

                prd = data[i][k] * op.data[k][j];

                sum = current + prd;

                out.data[i][j] = sum;
            }

            // Update shift_amt based on leading 0s or 1s.
            ldz = 0;
            ldo = 0;

            for(int k=WPRD(E,M)+CLOG2(N)-1; ((k>=0) && out.data[i][j].acc[k]); k--)
                ldo++;

            for(int k=WPRD(E,M)+CLOG2(N)-1; ((k>=0) && !out.data[i][j].acc[k]); k--)
                ldz++;

            ldd = (ldo == 0) ? ldz : ldo;

            if((ldd-1) < shift_amt){
                shift_amt = (ldd-1);
            }

            if((shift_amt == (WPRD(E,M)+CLOG2(N)-1)) && (ldz == 0)){
                zero_data = false;
            }
        }
    }


    // Calculate and adjust output bias based on shift_amt.
    if(CHECK_OF_UF){
        ap_int<10> bias_sum = bias + op.bias;

        // Check if data is all zero, adjust bias_sum.
        if(shift_amt == (WPRD(E,M)+CLOG2(N)-1) && zero_data){
            shift_amt = WPRD(E,M)+CLOG2(N);
            bias_sum = 0;
        }else{
            bias_sum = bias_sum - shift_amt;
        }

        if(bias_sum >= (1 << (8-1))){
            bias_sum = (1 << (8-1)) - 1;
        }else if(bias_sum < -(1 << (8-1))){
            bias_sum = 0;
            shift_amt = WPRD(E,M)+CLOG2(N);
        }

        out.bias = bias_sum;

    }else{
        ap_int<8> bias_sum = bias + op.bias;

        // Check if data is all zero, adjust bias_sum.
        if(shift_amt == (WPRD(E,M)+CLOG2(N)-1) && zero_data){
            shift_amt = WPRD(E,M)+CLOG2(N);
            bias_sum = 0;
        }else{
            bias_sum = bias_sum - shift_amt;
        }

        out.bias = bias_sum;
    }

    // Apply shift to data.
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            out.data[i][j].acc = out.data[i][j].acc << shift_amt;
        }
    }

    return out;
}



template <int N, int W, int F>
BlockFP<N,W,F> BlockFP<N,W,F>::operator *(BlockFP<N,W,F> &op){

    // Declare intermediate and output BFP.
    BlockFP<N,2*W,F> prd;
    BlockFP<N,W,F>   out;

    //////----Calculations for bias.----//////
    // ldz/ldo   -> leading zeroes/ones.
    // ldd       -> leading digits = max(ldz,ldo).
    // shift_amt -> initialize to max value.
    // zero_data -> if true and shift_amt is unchanged, set data to zero.
    ap_uint<CLOG2(2*W+CLOG2(N)+1)> ldz, ldo, ldd, shift_amt = (2*W+CLOG2(N)-1);
    bool zero_data = true;

    //////----Do matrix multiplication on minifloats and store results.----//////
    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=prd.data dim=0 complete

    for (int i=0; i<N; i++) {
        #pragma HLS unroll

        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            prd.data[i][j].acc = 0;

            for (int k=0; k<N; k++) {
                #pragma HLS unroll

                IntAcc<2*W+CLOG2(N), F> current, prd_int, sum;

                current = prd.data[i][j];

                prd_int = data[i][k] * op.data[k][j];

                sum = current + prd_int;

                prd.data[i][j] = sum;
            }

            // Update shift_amt based on leading 0s or 1s.
            ldz = 0;
            ldo = 0;

            for(int k=2*W+CLOG2(N)-1; ((k>=0) && prd.data[i][j].acc[k]); k--)
                ldo++;

            for(int k=2*W+CLOG2(N)-1; ((k>=0) && !prd.data[i][j].acc[k]); k--)
                ldz++;

            ldd = (ldo == 0) ? ldz : ldo;

            if((ldd-1) < shift_amt){
                shift_amt = (ldd-1);
            }

            if((shift_amt == (2*W+CLOG2(N)-1)) && (ldz == 0)){
                zero_data = false;
            }
        }
    }


    // Calculate and adjust output bias based on shift_amt.
    if(CHECK_OF_UF){
        ap_int<10> bias_sum = bias + op.bias;

        // Check if data is all zero, adjust bias_sum.
        if(shift_amt == (2*W+CLOG2(N)-1) && zero_data){
            shift_amt = 2*W+CLOG2(N);
            bias_sum = 0;
        }else{
            bias_sum = bias_sum - shift_amt;
        }

        if(bias_sum >= (1 << (8-1))){
            bias_sum = (1 << (8-1)) - 1;
        }else if(bias_sum < -(1 << (8-1))){
            bias_sum = 0;
            shift_amt = 2*W+CLOG2(N);
        }

        prd.bias = bias_sum;

    }else{
        ap_int<8> bias_sum = bias + op.bias;

        // Check if data is all zero, adjust bias_sum.
        if(shift_amt == (2*W+CLOG2(N)-1) && zero_data){
            shift_amt = 2*W+CLOG2(N);
            bias_sum = 0;
        }else{
            bias_sum = bias_sum + W - shift_amt;
        }

        prd.bias = bias_sum;
    }

    // Apply shift to data.
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            prd.data[i][j].acc = prd.data[i][j].acc << shift_amt;
        }
    }

    // Round product to required width.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll

        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            out.data[i][j].acc = rnd_method->rnd_mul(prd.data[i][j].acc);
        }
    }

    out.bias = prd.bias;

    return out;
}




#endif