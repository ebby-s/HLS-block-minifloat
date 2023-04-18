#ifndef DOT_PRD
#define DOT_PRD

template <int N, int E, int M>
BlockAcc<N,WfromEM(E,M)+CLOG2(N),FfromEM(E,M)> BlockMat<N,E,M>::operator *(BlockMat<N,E,M> &op){

    // Declare output block accumulator.
    BlockAcc<N,WfromEM(E,M)+CLOG2(N),FfromEM(E,M)> out;

    //////----Calculations for bias.----//////
    // ldz/ldo   -> leading zeroes/ones.
    // ldd       -> leading digits = max(ldz,ldo).
    // shift_amt -> initialize to max value.
    // zero_data -> if true and shift_amt is unchanged, set data to zero.
    ap_uint<CLOG2(WfromEM(E,M)+CLOG2(N)+1)> ldz, ldo, ldd, shift_amt = (WfromEM(E,M)+CLOG2(N)-1);
    ap_int<9> bias_sum = bias + op.bias;
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

                KulischAcc<WfromEM(E,M)+CLOG2(N), FfromEM(E,M)> current, prd, sum;

                current = out.data[i][j];

                prd = data[i][k] * op.data[k][j];

                sum = current + prd;

                out.data[i][j] = sum;

                // if((i == 23) && (j == 27)) std::cout << "Int " << k <<": " << float(current) << " + " << float(prd) << " = " << float(sum) << "\n";
            }

            // Update shift_amt based on leading 0s or 1s.
            ldz = 0;
            ldo = 0;

            for(int k=WfromEM(E,M)-1; ((k>=0) && out.data[i][j].acc[k]); k--)
                ldo++;

            for(int k=WfromEM(E,M)-1; ((k>=0) && !out.data[i][j].acc[k]); k--)
                ldz++;

            ldd = (ldo == 0) ? ldz : ldo;

            if((ldd-1) < shift_amt){
                shift_amt = (ldd-1);
            }

            if((shift_amt == (WfromEM(E,M)+CLOG2(N)-1)) && (ldz == 0)){
                zero_data = false;
            }
        }
    }

    // std::cout << "ShiftAmt = " << shift_amt << '\n';
    // std::cout << "ValBefore = " << out.data[0][0].acc << '\n';

    // Calculate output bias, update shift amount.
    if(shift_amt == (WfromEM(E,M)+CLOG2(N)-1) && zero_data){
        out.bias = 0;
        shift_amt = WfromEM(E,M)+CLOG2(N);
    }else if(shift_amt > bias_sum){
        out.bias = 0;
        shift_amt = bias_sum;
    }else{
        out.bias = bias_sum - shift_amt;
    }

    // Shift data values.
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            out.data[i][j].acc = out.data[i][j].acc << shift_amt;
        }
    }

    // std::cout << "ValAfter = " << out.data[0][0].acc << '\n';

    return out;
}



#endif
