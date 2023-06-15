#ifndef CONV
#define CONV

// Convert BMF to BFP.
template<int N, int E, int M>
BlockMF<N,E,M>::operator BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2>() const{

    const int Weff = (WPRD(E,M)/2);

    BlockFP<N,Weff,FPRD(E,M)/2> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=0 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    // Convert to BlockFP with no loss in accuracy.
    bmf_bfp_r_loop:
    for (int i=0; i<N; i++) {
        #pragma HLS UNROLL
        bmf_bfp_c_loop:
        for (int j=0; j<N; j++) {
            #pragma HLS UNROLL

            // Extract exponent.
            ap_uint<E> exp = (data[i][j].data >> M) & ((1<<E)-1);
            // Calculate mantissa.
            if(M == 0){
                out.data[i][j].acc = (exp != 0);
            }else{
                out.data[i][j].acc = (((exp != 0) << M) | (data[i][j].data & ((1<<M)-1)));
            }
            // Apply exponent and sign.
            if(exp)
                out.data[i][j].acc <<= exp-1;
            if(data[i][j].data >> (E+M))
                out.data[i][j].acc *= -1;
        }
    }

    out.bias = bias;

    return out;
}


// Convert BFP to BMF.
template <int N, int W, int F>
template <int E, int M, rnd_mode_t RMF>
BlockMF<N,E,M> BlockFP<N,W,F>::toBMF(){

    BlockMF<N,E,M> out;
    // Convert integers to MiniFloats.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll
        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            MiniFloat<E,M> out_mf;
            ap_uint<W> man_ext;
            ap_uint<CLOG2(W+1)> ldz = 0;

            // Extract sign, take abs value of data.
            out_mf.data = (data[i][j].acc < 0) << (E+M);
            if(data[i][j].acc < 0){
                man_ext = -data[i][j].acc;
            }else{
                man_ext = data[i][j].acc;
            }

            // Count leading zeros, extract exponent.
            for(int k=W-1; ((k>=0) && (!man_ext[k])); k--)
                ldz++;

            if((W-1) >= (1<<E)){
                if(ldz < ((1<<E)-1)){
                    out_mf.data |= (((1<<E)-1)-ldz) << M;
                }
                if(ldz >= ((1<<E)-1)){
                    man_ext <<= ((1<<E)-1)-1;
                }else{
                    man_ext <<= ldz;
                }
            }else{
                if(man_ext != 0){
                    out_mf.data |= (W-ldz-1) << M;
                }
                if(ldz == (W-1)){
                    man_ext <<= ldz-1;
                }else{
                    man_ext <<= ldz;
                }
            }

            // Round integer to fit in mantissa.
            if(W < (M+1)){
                out_mf.data |= (ap_uint<M+1>(man_ext) << (M+1-W)) & ((1<<M)-1);
            }else{
                if(RFP == RTZ){
                    out_mf.data |= (rnd_RTZ<N,W,F>(man_ext, M+1)) & ((1<<M)-1);
                }else if(RFP == RAZ){
                    out_mf.data |= (rnd_RAZ<N,W,F>(man_ext, M+1)) & ((1<<M)-1);
                }else if(RFP == RNI){
                    out_mf.data |= (rnd_RNI<N,W,F>(man_ext, M+1)) & ((1<<M)-1);
                }else if(RFP == RPI){
                    out_mf.data |= (rnd_RPI<N,W,F>(man_ext, M+1)) & ((1<<M)-1);
                }else if(RFP == RNE){
                    out_mf.data |= (rnd_RNE<N,W,F>(man_ext, M+1)) & ((1<<M)-1);
                }else if(RFP == STOCHASTIC){
                    out_mf.data |= (rnd_STOCHASTIC<N,W,F>(man_ext, M+1)) & ((1<<M)-1);
                }
            }

            out.data[i][j] = out_mf;
        }
    }

    if((W-1) >= (1<<E)){
        out.bias = bias + W - (1<<E) - F;
    }else{
        out.bias = bias - F;
    }

    return out;
}


// Convert BFP to BFP.
template <int N, int W, int F>
template <int Wo, int Fo, rnd_mode_t RFP>
BlockFP<N,Wo,Fo> BlockFP<N,W,F>::toBFP(){

    BlockFP<N,Wo,Fo> out;

    #pragma HLS ARRAY_PARTITION variable=data     dim=0 complete
    #pragma HLS ARRAY_PARTITION variable=out.data dim=0 complete

    // Rounding signals. Only used if W > Wo.
    BlockFP<N,Wo+1,Fo> rnd;
    bool rnd_ofl = false;

    // Round or shift data to new width.
    bfp_bfp_r_loop:
    for (int i=0; i<N; i++) {
        #pragma HLS UNROLL
        bfp_bfp_c_loop:
        for (int j=0; j<N; j++) {
            #pragma HLS UNROLL
            #pragma HLS INLINE

            if(W > Wo){
                IntAcc<Wo+1,Fo> rnd_fp;
                if(RFP == RTZ){
                    rnd_fp.acc = rnd_RTZ<N,W,F>(data[i][j].acc, Wo);
                }else if(RFP == RAZ){
                    rnd_fp.acc = rnd_RAZ<N,W,F>(data[i][j].acc, Wo);
                }else if(RFP == RNI){
                    rnd_fp.acc = rnd_RNI<N,W,F>(data[i][j].acc, Wo);
                }else if(RFP == RPI){
                    rnd_fp.acc = rnd_RPI<N,W,F>(data[i][j].acc, Wo);
                }else if(RFP == RNE){
                    rnd_fp.acc = rnd_RNE<N,W,F>(data[i][j].acc, Wo);
                }else if(RFP == STOCHASTIC){
                    rnd_fp.acc = rnd_STOCHASTIC<N,W,F>(data[i][j].acc, Wo);
                }
                rnd_ofl |= (rnd_fp.acc[Wo] ^ rnd_fp.acc[Wo-1]);
                rnd.data[i][j] = rnd_fp;
            }else{
                out.data[i][j].acc = ap_int<Wo>(data[i][j].acc) << (Wo - W);
            }
        }
    }

    // Assign output, shift if rounding caused overflow.
    if(W > Wo){
        for (int i=0; i<N; i++) {
            #pragma HLS UNROLL
            for (int j=0; j<N; j++) {
                #pragma HLS UNROLL

                IntAcc<Wo,Fo> out_fp;
                out_fp.acc = rnd.data[i][j].acc >> rnd_ofl;
                out.data[i][j] = out_fp;
            }
        }
    }

    // Adjust bias if rounding caused overflow.
    if(W > Wo){
        out.bias = bias + ((W-F)-(Wo-Fo)) + rnd_ofl;
    }else{
        out.bias = bias + ((W-F)-(Wo-Fo));
    }

    return out;
}


#endif
