#ifndef NORM
#define NORM


// Normalize a block of accumulators and convert to BMF.
template <int N, int W, int F>
template <int E, int M>
BlockAcc<N,W,F>::operator BlockMat<N,E,M>() const{

    BlockMat<N,E,M> out;

    for (int i=0; i<N; i++) {
        #pragma HLS unroll

        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            out.data[i][j] = 0;

        }
    }
}



#endif
