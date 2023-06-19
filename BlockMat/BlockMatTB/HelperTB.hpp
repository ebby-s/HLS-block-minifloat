

// Helper function to calculate the number of bits required
// to represent n using 2's cmpl format.
int clog2l(long n){
    // Decrease magnitude by 1 if number is negative.
    // Take abs value before counting leading zeros.
    n = std::labs((n < 0) ? n+1 : n); 

    for(int i=0; i<(sizeof(long)*8); i++){
        if(!(n>>i)){
            return i+1; // +1 for signed vs. unsigned.
        }
    }
}

// Helper function to calculate the number of bits required
// to represent n, unsigned.
int clog2lu(long n){

    n = std::labs(n); 

    for(int i=0; i<(sizeof(long)*8); i++){
        if(!(n>>i)){
            return i;
        }
    }
}

// Helper function to round BFP to BMF conversions.
long rnd_bmf_ninf_u(long prd, int M, int E, int W){

    int sgn = (prd < 0) ? -1 : 1;

    prd = std::labs(prd);

    int man_lim = clog2lu(prd)-(M+1);
    int rge_lim = W-((1<<E)-1)-M;
    int lgf_lim = (M==0) ? 1 : 0;

    int rnd_bits = std::max(std::max(man_lim, rge_lim), lgf_lim);

    if(rnd_bits > 0)
        prd &= ~((1<<rnd_bits)-1);

    return sgn*prd;
}

// Helper functions to round BFP to BFP Conversions.
long rnd_bfp_RTZ(long prd, int W, int Wo){

    int rnd_bits = W - Wo;
    bool rnd_up;

    if(rnd_bits > 0){

        rnd_up = ((prd & ((1<<rnd_bits)-1)) != 0);

        prd &= ~((1<<rnd_bits)-1);

        if(prd < 0)
            prd += long(rnd_up) << rnd_bits;

        if(prd == (1<<(W-1))){
            prd -= 1 << rnd_bits;
        }
    }

    return prd;
}
long rnd_bfp_RAZ(long prd, int W, int Wo){

    int rnd_bits = W - Wo;
    bool rnd_up;

    if(rnd_bits > 0){

        rnd_up = ((prd & ((1<<rnd_bits)-1)) != 0);

        prd &= ~((1<<rnd_bits)-1);

        if(prd > 0)
            prd += long(rnd_up) << rnd_bits;

        if(prd == (1<<(W-1))){
            prd -= 1 << rnd_bits;
        }
    }

    return prd;
}
long rnd_bfp_RNI(long prd, int W, int Wo){

    int rnd_bits = W - Wo;

    if(rnd_bits > 0)
        prd &= ~((1<<rnd_bits)-1);

    return prd;
}
long rnd_bfp_RPI(long prd, int W, int Wo){

    int rnd_bits = W - Wo;
    bool rnd_up;

    if(rnd_bits > 0){

        rnd_up = ((prd & ((1<<rnd_bits)-1)) != 0);

        prd &= ~((1<<rnd_bits)-1);

        prd += long(rnd_up) << rnd_bits;

        if(prd == (1<<(W-1))){
            prd -= 1 << rnd_bits;
        }
    }

    return prd;
}

// Helper functions to round Additions.
long rnd_add_ninf(long op0, long op1, int bias0, int bias1, int W){

    long op_a, op_b;
    int  shift;
    bool sticky;

    if(bias0 > bias1){
        op_a = op0 << 3;
        op_b = op1 << 3;
        shift = bias0 - bias1;
    }else{
        op_a = op1 << 3;
        op_b = op0 << 3;
        shift = bias1 - bias0;
    }

    if(shift < (W+3)){
        sticky = op_b & ((1<<shift)-1);
    }else{
        sticky = op_b;
    }

    op_b = (op_b >> std::min(shift,int(sizeof(long)*8-1))) | sticky;

    return op_a + op_b;
}

