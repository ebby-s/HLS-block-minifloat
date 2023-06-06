#ifndef B_ADD
#define B_ADD

#ifndef ADD_UNROLL
#define ADD_UNROLL 1
#endif

template<int N, int W, int F>
BlockFP<N,W+3+1,F+3> BlockFP<N,W,F>::operator +(BlockFP<N,W,F> &op){

    const int Weff = (W+3+1);

    BlockFP<N,Weff,F+3> out;

    #if (ADD_UNROLL == 0)
        #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
        #pragma HLS ARRAY_PARTITION variable=op.data  dim=1 complete
        #pragma HLS ARRAY_PARTITION variable=out.data dim=1 complete
    #elif (ADD_UNROLL != 1)
        DO_PRAGMA(HLS ARRAY_PARTITION variable=data     dim=1 cyclic factor=ADD_UNROLL)
        DO_PRAGMA(HLS ARRAY_PARTITION variable=op.data  dim=1 cyclic factor=ADD_UNROLL)
        DO_PRAGMA(HLS ARRAY_PARTITION variable=out.data dim=1 cyclic factor=ADD_UNROLL)
    #endif

    #pragma HLS ARRAY_PARTITION variable=data     dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=2 complete

    ////////////////////////////
    //         Addition Loops //
    ////////////////////////////

    bool op0_lrg = (bias > op.bias);  // True if op0 has a larger bias than op1.

    ap_int<9> shift = op0_lrg ? (bias - op.bias) : (op.bias - bias);  // Shift to align each element.

    // Shift and add data.
    add_r_loop:
    for (int i=0; i<N; i++){

        #if (ADD_UNROLL == 0)
            #pragma HLS UNROLL
        #elif (ADD_UNROLL == 1)
            #pragma HLS PIPELINE II=1
        #else
            DO_PRAGMA(HLS UNROLL factor=ADD_UNROLL)
        #endif

        add_c_loop:
        for (int j=0; j<N; j++){
            #pragma HLS UNROLL

            ap_int<W> op_lrg, op_sml;
            ap_int<W+3> op0_int, op1_int;
            bool sticky;

            op_lrg = op0_lrg ? data[i][j].acc : op.data[i][j].acc;
            op_sml = op0_lrg ? op.data[i][j].acc : data[i][j].acc;

            if(shift <= 3){
                sticky = 0;
            }else if(shift < (W+3)){
                sticky = op_sml & ((1<<(shift-3))-1);
            }else{
                sticky = op_sml;
            }

            op0_int = ap_int<W+3>(op_lrg) << 3;

            op1_int = ap_int<W+3>(op_sml) << 3;
            op1_int = op1_int >> ((shift < (W+3-1)) ? ap_uint<CLOG2(W+3)>(shift) : ap_uint<CLOG2(W+3)>(W+3-1));
            op1_int |= sticky;

            out.data[i][j].acc = op0_int + op1_int;
        }
    }

    // Larger bias is output bias.
    out.bias = op0_lrg ? bias : op.bias;

    return out;
}


#endif