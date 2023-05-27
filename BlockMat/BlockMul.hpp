#ifndef DOT_PRD
#define DOT_PRD

#include "CountLZ.hpp"

#define MUL_SAMT_II 1

#ifndef MUL_NORM
#define MUL_NORM false
#endif

template <int N, int E, int M>
BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> BlockMF<N,E,M>::operator *(BlockMF<N,E,M> &op){

    const int Weff = (WPRD(E,M)+CLOG2(N));

    const bool norm_en = MUL_NORM;

    BlockFP<N,Weff,FPRD(E,M)> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    ////////////////////////////
    //  Matrix multiplication //
    ////////////////////////////

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

    ////////////////////////////
    // Optional normalisation //
    ////////////////////////////

    if(norm_en){
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
        out.bias = bias + op.bias - shift_amt;

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
        out.bias = bias + op.bias;
    }

    return out;
}


template <int N, int W, int F>
BlockFP<N,2*W+CLOG2(N),2*F> BlockFP<N,W,F>::operator *(BlockFP<N,W,F> &op){

    const int Weff = (2*W+CLOG2(N));

    const bool norm_en = MUL_NORM;

    BlockFP<N,Weff,2*F> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    ////////////////////////////
    //  Matrix multiplication //
    ////////////////////////////

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

    ////////////////////////////
    // Optional normalisation //
    ////////////////////////////

    if(norm_en){
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
        out.bias = bias + op.bias - shift_amt;

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
        out.bias = bias + op.bias;
    }

    return out;
}


#endif
