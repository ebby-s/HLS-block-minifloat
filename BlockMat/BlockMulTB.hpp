

template <int N, int E, int M>
int bmf_mul_tb(bool test_of_uf = false){    // Test BMF multiplier with random stimulus.

    BlockMF<N,E,M> op0, op1;                        // op0, op1 -> inputs to mat mul.
    BlockFP<N, WPRD(E,M)+CLOG2(N), FPRD(E,M)> prd;  // prd = op0 * op1, output of mat mul.

    long ref_prd [N][N];   // Reference product, ignoring shared exponent.
    int  ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] BMF Mul, Parameters: N=" << N << " E=" << E << " M=" << M << '\n';

    // Bias mask, test overflow/underflow with 0xff, disable with 0x3f.
    int bias_mask = test_of_uf ? (1<<8) : (1<<6);

    for(int l=0; l<(1<<(1+E+M)); l++){    // Test with random inputs.

        // Generate op0 & op1 randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                op0.data[j][k].data = rand() % (1<<(E+M+1));
                op1.data[j][k].data = rand() % (1<<(E+M+1));
            }
        }
        op0.bias = rand() % bias_mask;
        op1.bias = rand() % bias_mask;

        // Get result from DUT.
        prd = op0 * op1;

        // Calculate float64 reference.
        // Keep track of most positive/negative values.
        long ref_max_data = 0;
        long ref_min_data = 0;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){

                ref_prd[i][j] = 0;
                for(int k=0; k<N; k++){
                    ref_prd[i][j] += (double(op0.data[i][k]) * double(op1.data[k][j])) * pow(2,FPRD(E,M));
                }
                ref_max_data = std::max(ref_prd[i][j], ref_max_data);
                ref_min_data = std::min(ref_prd[i][j], ref_min_data);
            }
        }

        // Model normalization step.
        long ref_ldd;
        long ref_shift_amt;

        ref_ldd = std::max(clog2l(ref_max_data), clog2l(ref_min_data));

        ref_shift_amt = WPRD(E,M)+CLOG2(N) - ref_ldd;

        ref_bias = op0.bias + op1.bias - ref_shift_amt;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                ref_prd[i][j] *= pow(2, ref_shift_amt);
            }
        }

        // Saturate reference bias in case of overflow.
        if(ref_bias >= (1 << (8-1))){
            ref_bias = (1 << (8-1)) -1;
        // Set reference to zero in case of underflow.
        }else if(ref_bias < -(1 << (8-1))){
            for(int i=0; i<N; i++){
                for(int j=0; j<N; j++){
                    ref_prd[i][j] = 0;
                }
            }
        }

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((ref_prd[j][k] * pow(2,ref_bias-FPRD(E,M))) != (double(prd.data[j][k]) * pow(2,prd.bias))){

                    // std::cout << "RefBias " << op0.bias << " + " << op1.bias << " -> " << ref_bias << "\n";
                    // std::cout << "TestBias " << prd.bias << "\n";
                    // std::cout << "W, cl2(N): " << WPRD(E,M) << ", " << CLOG2(N) << "\n";
                    // std::cout << "Ref[" << j << ", " << k << "] = " << double(ref_prd[j][k]) << "\n";
                    // std::cout << "Test[" << j << ", " << k << "] = " << double(prd.data[j][k]) << "\n";
                    // std::cout << "Ref. SAmt: " << ref_shift_amt << ", " << ref_max_data << '\n';
                    // std::cout << ref_max_data_int << ", " << ref_min_data_int << ", " << ref_ldd << '\n';

                    printf("[ERROR] FAILED\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}


template <int N, int W, int F>
int bfp_mul_tb(bool test_of_uf = false){    // Test BFP multiplier with random stimulus.

    BlockFP<N,W,F> op0, op1;  // op0, op1 -> inputs to mat mul.
    BlockFP<N,2*W+CLOG2(N),2*F> prd;       // prd = op0 * op1, output of mat mul.

    long ref_prd [N][N];   // Reference product, ignoring shared exponent.
    int  ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] BFP Mul, Parameters: N=" << N << " W=" << W << " F=" << F << '\n';

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
        }while((!test_of_uf) && (((op0.bias+op1.bias+2*W)>=128) || ((op0.bias+op1.bias-2*(W+F))<-128)));

        // Get result from DUT.
        prd = op0 * op1;

        // Calculate float64 reference.
        // Keep track of most positive/negative values.
        long ref_max_data = 0;
        long ref_min_data = 0;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){

                ref_prd[i][j] = 0;
                for(int k=0; k<N; k++){
                    ref_prd[i][j] += op0.data[i][k].acc * op1.data[k][j].acc;
                }
                ref_max_data = std::max(ref_prd[i][j], ref_max_data);
                ref_min_data = std::min(ref_prd[i][j], ref_min_data);
            }
        }

        // Model normalization step.
        long ref_ldd;
        long ref_shift_amt;

        ref_ldd = std::max(clog2l(ref_max_data), clog2l(ref_min_data));

        ref_shift_amt = 2*W+CLOG2(N) - ref_ldd;

        ref_bias = op0.bias + op1.bias - ref_shift_amt;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                ref_prd[i][j] *= pow(2, ref_shift_amt);
            }
        }

        // Saturate reference bias in case of overflow.
        if(ref_bias >= (1 << (8-1))){
            ref_bias = (1 << (8-1)) -1;
        // Set reference to zero in case of underflow.
        }else if(ref_bias < -(1 << (8-1))){
            for(int i=0; i<N; i++){
                for(int j=0; j<N; j++){
                    ref_prd[i][j] = 0;
                }
            }
        }

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((ref_prd[j][k] * pow(2,ref_bias-2*F)) != (double(prd.data[j][k]) * pow(2,prd.bias))){

                    std::cout << "RefBias " << op0.bias << " + " << op1.bias << " -> " << ref_bias << "\n";
                    std::cout << "TestBias " << prd.bias << "\n";
                    std::cout << "Inputs " << double(op0.data[j][k]) << " * " << double(op1.data[j][k]) << "\n";
                    std::cout << "Ref[" << j << ", " << k << "] = " << double(ref_prd[j][k]) << "\n";
                    std::cout << "Test[" << j << ", " << k << "] = " << double(prd.data[j][k]) << "\n";
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

