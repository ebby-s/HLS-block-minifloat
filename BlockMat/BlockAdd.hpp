#ifndef B_ADD
#define B_ADD


template<int N, int W, int F>
BlockFP<N,W,F> BlockFP<N,W,F>::operator +(BlockFP<N,W,F> &op){

    // Declare sum and output.
    BlockFP<N,W+1+3,F> sum;
    BlockFP<N,W,F> out;

    // Calculate shift required to align operands.
    ap_int<8> shift;
    shift = (bias > op.bias) ? bias - op.bias : op.bias - bias;

    // Shift and add data.
    for (int i=0; i<N; i++) {
        #pragma HLS unroll

        for (int j=0; j<N; j++) {
            #pragma HLS unroll

            IntAcc<W+3,F> op0_int, op1_int;
            IntAcc<W+3+1,F> sum_int;

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