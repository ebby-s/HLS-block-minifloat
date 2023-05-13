#ifndef DOT_PRD
#define DOT_PRD

#include "CountLZ.hpp"

template <int N, int E, int M>
BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> BlockMF<N,E,M>::operator *(BlockMF<N,E,M> &op){

    #define Weff (WPRD(E,M)+CLOG2(N))

    BlockFP<N,Weff,FPRD(E,M)> out;     // Output array.

    // ap_uint<CLOG2(Weff+1)> ldd[N][N];  // Leading digit count, for each output element.

    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete
    // #pragma HLS ARRAY_PARTITION variable=ldd      dim=0 complete

    zero_r_loop:  // Initialise output to zero.
    for (int i=0; i<N; i++){
        #pragma HLS UNROLL
        zero_c_loop:
        for (int j=0; j<N; j++){
            #pragma HLS UNROLL

            out.data[i][j].acc = 0;
        }
    }

    mm_dp_loop:  // MatMul by Systolic Array.
    for (int k=0; k<N; k++){
        #pragma HLS PIPELINE II=1
        #pragma HLS LOOP_TRIPCOUNT min=8 max=8
        mm_r_loop:
        for (int i=0; i<N; i++){
            #pragma HLS UNROLL
            mm_c_loop:
            for (int j=0; j<N; j++){
                #pragma HLS UNROLL

                out.data[i][j] = out.data[i][j] + (data[i][k] * op.data[k][j]);
            }
        }
    }

    // Determine shift_amt, bias calculations.
    ap_uint<CLOG2(Weff+1)> ldd, shift_amt = (Weff-1);
    samt_r_loop:
    for(int i=0; i<N; i++){
        #pragma HLS unroll
        samt_c_loop:
        for(int j=0; j<N; j++){
            #pragma HLS unroll
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
    out.bias = bias + op.bias - shift_amt;

    shift_r_loop:  // Apply shift to data.
    for(int i=0; i<N; i++){
        #pragma HLS unroll
        shift_c_loop:
        for(int j=0; j<N; j++){
            #pragma HLS unroll

            out.data[i][j].acc = out.data[i][j].acc << shift_amt;
        }
    }

    return out;
}



template <int N, int W, int F>
BlockFP<N,2*W+CLOG2(N),2*F> BlockFP<N,W,F>::operator *(BlockFP<N,W,F> &op){

    // Declare output BFP.
    BlockFP<N,2*W+CLOG2(N),2*F> out;

    #define Weff (2*W+CLOG2(N))
    // Calculations for bias.
    // shift_amt -> shift needed to normalise data.
    // zero_data -> if true and shift_amt is max value, zero all data.
    ap_uint<CLOG2(Weff+1)> shift_amt = (Weff-1);
    bool zero_data = true;

    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    for (int i=0; i<N; i++){
        #pragma HLS unroll
        for (int j=0; j<N; j++){
            #pragma HLS unroll

            out.data[i][j].acc = 0;
            for (int k=0; k<N; k++){
                out.data[i][j] = out.data[i][j] + (data[i][k] * op.data[k][j]);
            }

            zero_data = (out.data[i][j].acc != 0) ? false : zero_data;
            // Update shift_amt based on leading digits.
            for(int k=0; k<Weff; k++){
                if(!(out.data[i][j].acc >> k) || !((~out.data[i][j].acc) >> k)){
                    if((Weff-k-1) < shift_amt)
                        shift_amt = Weff-k-1;
                    break;
                }
            }
        }
    }

    // Bias calculations, check if data is all zero.
    if((shift_amt == (Weff-1)) && zero_data){
        out.bias = 0;
    }else{
        out.bias = bias + op.bias - shift_amt;
    }

    // Apply shift to data.
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            out.data[i][j].acc = out.data[i][j].acc << shift_amt;
        }
    }

    return out;
}


#endif
