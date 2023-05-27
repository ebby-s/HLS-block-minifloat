#ifndef NORMALISE
#define NORMALISE

template <int N, int W, int F>
void BlockFP<N,W,F>::normalise(){

    #pragma HLS ARRAY_PARTITION variable=data dim=0 complete

    // Normalise data, calculate bias.
    ap_uint<CLOG2(W+1)> ldd, shift_amt = (W-1);
    samt_r_loop:
    for(int i=0; i<N; i++){
        #pragma HLS PIPELINE II=1
        samt_c_loop:
        for(int j=0; j<N; j++){
            #pragma HLS UNROLL
            #pragma HLS INLINE

            ap_int<W> ldd_data;
            if(data[i][j].acc[W-1]){
                ldd_data = ~data[i][j].acc;
            }else{
                ldd_data =  data[i][j].acc;
            }

            ldd = CtLZ<W>(ldd_data);

            if((ldd-1) < shift_amt)
                shift_amt = ldd-1;
        }
    }
    bias -= shift_amt;

    shift_r_loop:  // Apply shift to data.
    for(int i=0; i<N; i++){
        #pragma HLS UNROLL
        shift_c_loop:
        for(int j=0; j<N; j++){
            #pragma HLS UNROLL

            data[i][j].acc = data[i][j].acc << shift_amt;
        }
    }
}


#endif