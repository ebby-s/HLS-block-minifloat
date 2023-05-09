#ifndef DP
#define DP


template <int N, int E, int M>
IntAcc<WPRD(E,M)+CLOG2(N),FPRD(E,M)> MFDotPrd(MiniFloat<E,M> op0[N], MiniFloat<E,M> op1[N]){
    #pragma HLS ARRAY_PARTITION variable=op0 dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op1 dim=1 complete

    IntAcc<WPRD(E,M)+CLOG2(N),FPRD(E,M)> sum;

    sum.acc = 0;
    for(int i=0; i<N; i++){
        #pragma HLS UNROLL

        sum  = sum + (op0[i] * op1[i]);
    }

    return sum;
}

template <int N, int W, int F>
IntAcc<2*W+CLOG2(N),2*F> IADotPrd(IntAcc<W,F> op0[N], IntAcc<W,F> op1[N]){
    #pragma HLS ARRAY_PARTITION variable=op0 dim=1 complete
    #pragma HLS ARRAY_PARTITION variable=op1 dim=1 complete

    IntAcc<2*W+CLOG2(N),2*F> sum;

    sum.acc = 0;
    for(int i=0; i<N; i++){
        #pragma HLS UNROLL

        sum  = sum + (op0[i] * op1[i]);
    }

    return sum;
}

#endif
