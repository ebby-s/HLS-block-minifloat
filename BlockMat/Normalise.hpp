#ifndef NORMALISE
#define NORMALISE

#ifndef NORM_II
#define NORM_II 1
#endif

template <int N, int W, int F>
void BlockFP<N,W,F>::normalise(){ // Normalise a BFP block.

    #pragma HLS ARRAY_PARTITION variable=data dim=0 complete

    // Calculate minimum LDD.
    ap_uint<CLOG2(W+1)> ldd, shift_amt = (W-1);
    samt_r_loop:
    for(int i=0; i<N; i++){
        DO_PRAGMA(HLS PIPELINE II=NORM_II)
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

    bias -= shift_amt;   // Adjust bias

    shift_r_loop:        // Apply shift to data.
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