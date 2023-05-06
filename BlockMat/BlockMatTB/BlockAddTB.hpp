

template <int N, int W, int F>
int bfp_add_tb(){    // Test BFP adder with random stimulus.

    BlockFP<N,W,F> op0, op1;               // op0, op1 -> inputs.
    BlockFP<N,W+3+1,F+3> sum;       // prd = op0 + op1, output

    long ref_sum [N][N];   // Reference sum, ignoring shared exponent.
    int  ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] BFP Add, Parameters: N=" << N << " W=" << W << " F=" << F << '\n';

    for(int l=0; l<(1<<8); l++){    // Test with random inputs.

        // Generate op0 & op1 randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                op0.data[j][k].acc = rand() % (1<<W);
                op1.data[j][k].acc = rand() % (1<<W);
            }
        }
        do{
            op0.bias = rand() % (1<<8);
            op1.bias = rand() % (1<<8);
        }while(((std::max(op0.bias,op1.bias)-W+3+1)>=128) || ((std::max(op0.bias,op1.bias)-W+3+1)<-128));

        // Get result from DUT.
        sum = op0 + op1;

        // Calculate float64 reference.
        // Keep track of most positive/negative values.
        long ref_max_data = 0;
        long ref_min_data = 0;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){

                ref_sum[i][j] = rnd_add_ninf(op0.data[i][j].acc,op1.data[i][j].acc,op0.bias,op1.bias,W);

                ref_max_data = std::max(ref_sum[i][j], ref_max_data);
                ref_min_data = std::min(ref_sum[i][j], ref_min_data);
            }
        }

        // Model normalization step.
        long ref_ldd;
        long ref_shift_amt;

        ref_ldd = std::max(clog2l(ref_max_data), clog2l(ref_min_data));
        ref_shift_amt = W+3+1 - ref_ldd;
        ref_bias = std::max(op0.bias,op1.bias) - ref_shift_amt;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                ref_sum[i][j] *= pow(2, ref_shift_amt);
            }
        }

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((ref_sum[j][k] * pow(2,ref_bias-F-3)) != (double(sum.data[j][k]) * pow(2,sum.bias))){

                    std::cout << "RefBias " << op0.bias << ", " << op1.bias << " -> " << ref_bias << "\n";
                    std::cout << "TestBias " << sum.bias << "\n";
                    std::cout << "Inputs " << double(op0.data[j][k]) << " + " << double(op1.data[j][k]) << "\n";
                    std::cout << "Ref[" << j << ", " << k << "] = " << double(ref_sum[j][k]) << "\n";
                    std::cout << "Test[" << j << ", " << k << "] = " << double(sum.data[j][k]) << "\n";
                    std::cout << "Ref. SAmt: " << ref_shift_amt << ", " << ref_max_data << '\n';

                    printf("[ERROR] FAILED\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}

