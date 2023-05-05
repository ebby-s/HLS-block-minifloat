#ifndef B_ADD
#define B_ADD


template<int N, int W, int F>
BlockFP<N,W,F> BlockFP<N,W,F>::operator +(BlockFP<N,W,F> &op){

    // Declare output.
    BlockFP<N,W+3+1,F+3> out;

    // Sort input blocks, calculate shift required to align operands.
    BlockFP<N,W,F> op_a, op_b;
    ap_int<8> shift;

    if(bias > op.bias){
        op_a.data = data;
        op_b.data = op.data;
        shift = bias - op.bias;
    }else{
        op_a.data = op.data;
        op_b.data = data;
        shift = op.bias - bias;
    }

    // Shift and add data.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll
        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            IntAcc<W+3,F+3> op0_int, op1_int;
            IntAcc<W+3+1,F+3> sum_int;

            // Align operands
            op0_int.acc = data[i][j].acc << 3;
            op1_int.acc = op.data[i][j].acc << 3;

            if(bias > op.bias){
                op1_int.acc >>= shift;
            }else{
                op0_int.acc >>= shift;
            }

            sum_int = op0_int + op1_int;

        }
    }
}





#endif