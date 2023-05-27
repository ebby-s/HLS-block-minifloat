#ifndef B_ADD
#define B_ADD

#define ADD_SAMT_II 1

#ifndef ADD_UNROLL
#define ADD_UNROLL false
#endif

#ifndef ADD_POST_NORM
#define ADD_POST_NORM false
#endif

template<int N, int W, int F>
BlockFP<N,W+3+1,F+3> BlockFP<N,W,F>::operator +(BlockFP<N,W,F> &op){

    const int Weff = (W+3+1);

    const bool post_norm_en = ADD_POST_NORM;

    BlockFP<N,Weff,F+3> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=0 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=0 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    ////////////////////////////
    //         Addition Loops //
    ////////////////////////////

    bool op0_lrg = (bias > op.bias);  // True if op0 has a larger bias than op1.

    ap_int<9> shift = op0_lrg ? (bias - op.bias) : (op.bias - bias);  // Shift to align each element.

    // Shift and add data.
    add_r_loop:
    for (int i=0; i<N; i++){
        #if ADD_UNROLL
            #pragma HLS UNROLL
        #else
            DO_PRAGMA(HLS PIPELINE II=ADD_SAMT_II)
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

    ////////////////////////////
    // Optional Normalisation //
    ////////////////////////////

    if(post_norm_en){
        // Normalise data, calculate bias.
        ap_uint<CLOG2(Weff+1)> ldd, shift_amt = (Weff-1);
        samt_r_loop:
        for(int i=0; i<N; i++){
            DO_PRAGMA(HLS PIPELINE II=MUL_SAMT_II)
            samt_c_loop:
            for(int j=0; j<N; j++){
                #pragma HLS UNROLL
                #pragma HLS INLINE

                ap_int<Weff> ldd_data;
                if(out.data[i][j].acc[Weff-1]){
                    ldd_data = ~out.data[i][j].acc;
                }else{
                    ldd_data =  out.data[i][j].acc;
                }

                ldd = CtLZ<Weff>(ldd_data);

                if((ldd-1) < shift_amt)
                    shift_amt = ldd-1;
            }
        }
        out.bias = (op0_lrg ? bias : op.bias) - shift_amt;

        shift_r_loop:  // Apply shift to data.
        for(int i=0; i<N; i++){
            #pragma HLS UNROLL
            shift_c_loop:
            for(int j=0; j<N; j++){
                #pragma HLS UNROLL

                out.data[i][j].acc = out.data[i][j].acc << shift_amt;
            }
        }
    }else{
        out.bias = op0_lrg ? bias : op.bias;
    }

    return out;
}


#endif