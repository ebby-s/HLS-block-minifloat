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
        }
    }

    return out;
}



#endif
