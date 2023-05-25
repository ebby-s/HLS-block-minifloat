#ifndef B_ADD
#define B_ADD


template<int N, int W, int F>
BlockFP<N,W+3+1,F+3> BlockFP<N,W,F>::operator +(BlockFP<N,W,F> &op){

    // Declare output.
    BlockFP<N,W+3+1,F+3> out;

    //////----Calculations for bias.----//////
    // ldz/ldo   -> leading zeroes/ones.
    // ldd       -> leading digits = max(ldz,ldo).
    // shift_amt -> initialize to max value.
    // zero_data -> if true and shift_amt is unchanged, set data to zero.
    ap_uint<CLOG2(W+3+1+1)> ldz, ldo, ldd, shift_amt = (W+3+1-1);
    bool zero_data = true;

    // Sort input blocks, calculate shift required to align operands.
    // op_a has a larger bias than op_b.
    BlockFP<N,W,F> *op_a, *op_b;
    ap_int<9> shift;

    if(bias > op.bias){
        op_a = this;
        op_b = &op;
        shift = bias - op.bias;
    }else{
        op_a = &op;
        op_b = this;
        shift = op.bias - bias;
    }

    // Shift and add data.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll
        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            IntAcc<W+3,F+3> op0_int, op1_int;
            IntAcc<W+3+1,F+3> sum_int;

            bool sticky;

            if(shift <= 3){
                sticky = 0;
            }else if(shift < (W+3)){
                sticky = op_b->data[i][j].acc & ((1<<(shift-3))-1);
            }else{
                sticky = op_b->data[i][j].acc;
            }

            op0_int.acc = ap_int<W+3>(op_a->data[i][j].acc) << 3;

            op1_int.acc = ap_int<W+3>(op_b->data[i][j].acc) << 3;
            op1_int.acc = int(op1_int.acc) >> std::min(int(shift), int(W+3-1));
            op1_int.acc |= sticky;

            sum_int.acc = op0_int.acc + op1_int.acc;

            out.data[i][j] = sum_int;

            // Update shift_amt based on leading 0s or 1s.
            ldz = 0;
            ldo = 0;

            for(int k=W+3+1-1; ((k>=0) && out.data[i][j].acc[k]); k--)
                ldo++;

            for(int k=W+3+1-1; ((k>=0) && !out.data[i][j].acc[k]); k--)
                ldz++;

            ldd = (ldo == 0) ? ldz : ldo;

            if((ldd-1) < shift_amt)
                shift_amt = (ldd-1);

            if((shift_amt == (W+3+1-1)) && (ldz == 0))
                zero_data = false;
        }
    }

    // Calculate and adjust output bias based on shift_amt.
    // Check if data is all zero, adjust bias.
    if(shift_amt == (W+3+1-1) && zero_data){
        shift_amt = W+3+1;
        out.bias = 0;
    }else{
        out.bias = op_a->bias - shift_amt;
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