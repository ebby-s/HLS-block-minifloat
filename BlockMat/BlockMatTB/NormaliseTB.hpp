

template <int N, int W, int F>
int bfp_norm_tb(){    // Test BFP normalisation with random stimulus.

    BlockFP<N,W,F> op, ref_copy;  // input BFP and reference copy.

    // Log parameters.
    std::cout << "[INFO] BFP Norm, Parameters: N=" << N << " W=" << W << " F=" << F << '\n';

    for(int l=0; l<(1<<8); l++){    // Test with random inputs.

        // Generate op0 & op1 randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                op.data[j][k].acc = rand() % (1<<W);
                ref_copy[j][k].acc = op.data[j][k].acc;
            }
        }
        do{
            op.bias = rand() % (1<<8);
            ref_copy.bias = op.bias;
        }while(((op0.bias-W)>=128) || ((op0.bias-W)<-128));

        // Get result from DUT.
        op.normalise();

        // Check if all data is zero.
        bool zero_data = true;    // Set to false if non-zero output.

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){

                zero_data = (op[i][j].acc != 0) ? false : zero_data;
            }
        }

        // Check if sum is normalised.
        bool normalised = false;
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                normalised = (op.data[j][k].acc[W-1] ^ op.data[j][k].acc[W-2]) ? true : normalised;
            }
        }
        if(!zero_data && !normalised){
            printf("[ERROR] FAILED: Result not normalised.\n");
            throw 1;
        }

        // Compare DUT result to reference, should be equal.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((ref_copy[j][k].acc * pow(2,ref_copy.bias)) != (op.data[j][k].acc * pow(2,sum.bias))){

                    printf("[ERROR] FAILED: Reference does not match DUT.\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}

