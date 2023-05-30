#ifndef DOT_PRD
#define DOT_PRD

#include "CountLZ.hpp"


template <int N, int E, int M>
BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> BlockMF<N,E,M>::operator *(BlockMF<N,E,M> &op){

    const int Weff = (WPRD(E,M)+CLOG2(N));

    BlockFP<N,Weff,FPRD(E,M)> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    ////////////////////////////
    //  Matrix multiplication //
    ////////////////////////////

    bmf_zero_r_loop:  // Initialise output to zero.
    for (int i=0; i<N; i++){
        #pragma HLS UNROLL
        bmf_zero_c_loop:
        for (int j=0; j<N; j++){
            #pragma HLS UNROLL

            out.data[i][j].acc = 0;
        }
    }

    bmf_mm_dp_loop:  // MatMul by Systolic Array.
    for (int k=0; k<N; k++){
        #pragma HLS PIPELINE II=1
        bmf_mm_r_loop:
        for (int i=0; i<N; i++){
            #pragma HLS UNROLL
            bmf_mm_c_loop:
            for (int j=0; j<N; j++){
                #pragma HLS UNROLL

                out.data[i][j] = out.data[i][j] + (data[i][k] * op.data[k][j]);
            }
        }
    }

    // Add biases.
    out.bias = bias + op.bias;

    return out;
}


template <int N, int W, int F>
BlockFP<N,2*W+CLOG2(N),2*F> BlockFP<N,W,F>::operator *(BlockFP<N,W,F> &op){

    const int Weff = (2*W+CLOG2(N));

    BlockFP<N,Weff,2*F> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    ////////////////////////////
    //  Matrix multiplication //
    ////////////////////////////

    bfp_zero_r_loop:  // Initialise output to zero.
    for (int i=0; i<N; i++){
        #pragma HLS UNROLL
        bfp_zero_c_loop:
        for (int j=0; j<N; j++){
            #pragma HLS UNROLL

            out.data[i][j].acc = 0;
        }
    }

    bfp_mm_dp_loop:  // MatMul by Systolic Array.
    for (int k=0; k<N; k++){
        #pragma HLS PIPELINE II=1
        bfp_mm_r_loop:
        for (int i=0; i<N; i++){
            #pragma HLS UNROLL
            bfp_mm_c_loop:
            for (int j=0; j<N; j++){
                #pragma HLS UNROLL

                out.data[i][j] = out.data[i][j] + (data[i][k] * op.data[k][j]);
            }
        }
    }

    // Add biases.
    out.bias = bias + op.bias;

    return out;
}


#endif
