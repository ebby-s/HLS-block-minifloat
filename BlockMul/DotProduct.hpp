#ifndef DOT_PRD
#define DOT_PRD

template <int N, int E, int M>
BlockAcc<N,WfromEM(E,M)+CLOG2(N),FfromEM(E,M)> BlockMat<N,E,M>::operator *(BlockMat<N,E,M> &op){

    BlockAcc<N,WfromEM(E,M)+CLOG2(N),FfromEM(E,M)> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op.data  dim=2 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    for (int i=0; i<N; i++) {
        #pragma HLS unroll

        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            out.data[i][j] = 0;

            for (int k=0; k<N; k++) {
                #pragma HLS unroll

                out.data[i][j] += data[i][k] * op.data[k][j];
            }
        }
    }

}



#endif
