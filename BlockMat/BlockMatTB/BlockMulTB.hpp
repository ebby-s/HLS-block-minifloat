

template <int N, int E, int M>
int bmf_mul_tb(){    // Test BMF multiplier with random stimulus.

    BlockMF<N,E,M> op0, op1;                        // op0, op1 -> inputs to mat mul.
    BlockFP<N, WPRD(E,M)+CLOG2(N), FPRD(E,M)> prd;  // prd = op0 * op1, output of mat mul.

    long ref_prd [N][N];   // Reference product, ignoring shared exponent.
    int  ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] BMF Mul, Parameters: N=" << N << " E=" << E << " M=" << M << " Norm=" << MUL_NORM << '\n';

    for(int l=0; l<(1<<(1+E+M)); l++){    // Test with random inputs.

        // Generate input, corner cases followed by random cases.
        if(l==0){
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].data = 0;
                    op1.data[j][k].data = 0;
                }
            }
            op0.bias = 0;
            op1.bias = 0;

        }else if(l==1){
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].data = rand() % (1<<(E+M+1));
                    op1.data[j][k].data = 0;
                }
            }
            do{
                op0.bias = rand() % (1<<8);
                op1.bias = 0;
            }while(((op0.bias+op1.bias+WPRD(E,M))>=128) || ((op0.bias+op1.bias-WPRD(E,M))<-128));

        }else if(l==2){
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].data = 0;
                    op1.data[j][k].data = rand() % (1<<(E+M+1));
                }
            }
            do{
                op0.bias = 0;
                op1.bias = rand() % (1<<8);
            }while(((op0.bias+op1.bias+WPRD(E,M))>=128) || ((op0.bias+op1.bias-WPRD(E,M))<-128));

        }else{
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].data = rand() % (1<<(E+M+1));
                    op1.data[j][k].data = rand() % (1<<(E+M+1));
                }
            }
            do{
                op0.bias = rand() % (1<<8);
                op1.bias = rand() % (1<<8);
            }while(((op0.bias+op1.bias+WPRD(E,M))>=128) || ((op0.bias+op1.bias-WPRD(E,M))<-128));
        }

        // Get result from DUT.
        prd = op0 * op1;

        // Calculate float64 reference.
        long ref_max_data = 0;    // Keep track of most pos value.
        long ref_min_data = 0;    // Keep track of most neg value.
        bool zero_data = true;    // Set to false if non-zero output.

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){

                ref_prd[i][j] = 0;
                for(int k=0; k<N; k++){
                    ref_prd[i][j] += (double(op0.data[i][k]) * double(op1.data[k][j])) * pow(2,FPRD(E,M));
                }
                ref_max_data = std::max(ref_prd[i][j], ref_max_data);
                ref_min_data = std::min(ref_prd[i][j], ref_min_data);
                zero_data = (ref_prd[i][j] != 0) ? false : zero_data;
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

        // Check if bias was zeroed for cases where all data is zero.
        // if(zero_data && (prd.bias != 0)){
        //     printf("[ERROR] FAILED: Non-zero bias with zero data.\n");
        //     throw 1;
        // }

        // Check if product is normalised.
        bool normalised = false;
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                normalised = (prd.data[j][k].acc[WPRD(E,M)+CLOG2(N)-1] ^ prd.data[j][k].acc[WPRD(E,M)+CLOG2(N)-2]) ? true : normalised;
            }
        }
        if(!zero_data && !normalised && MUL_NORM){
            printf("[ERROR] FAILED: Product not normalised.\n");
            throw 1;
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
                    // std::cout << ref_max_data << ", " << ref_min_data << ", " << ref_ldd << '\n';

                    printf("[ERROR] FAILED: Reference does not match DUT.\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}


template <int N, int W, int F>
int bfp_mul_tb(){    // Test BFP multiplier with random stimulus.

    BlockFP<N,W,F> op0, op1;  // op0, op1 -> inputs to mat mul.
    BlockFP<N,2*W+CLOG2(N),2*F> prd;       // prd = op0 * op1, output of mat mul.

    long ref_prd [N][N];   // Reference product, ignoring shared exponent.
    int  ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] BFP Mul, Parameters: N=" << N << " W=" << W << " F=" << F << '\n';

    for(int l=0; l<(1<<8); l++){    // Test with random inputs.

        // Generate input, corner cases followed by random cases.
        if(l==0){
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].acc = 0;
                    op1.data[j][k].acc = 0;
                }
            }
            op0.bias = 0;
            op1.bias = 0;

        }else if(l==1){
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].acc = rand() % (1<<W);
                    op1.data[j][k].acc = 0;
                }
            }
            do{
                op0.bias = rand() % (1<<8);
                op1.bias = 0;
            }while(((op0.bias+op1.bias+2*W)>=128) || ((op0.bias+op1.bias-2*(W+F))<-128));

        }else if(l==2){
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].acc = 0;
                    op1.data[j][k].acc = rand() % (1<<W);
                }
            }
            do{
                op0.bias = 0;
                op1.bias = rand() % (1<<8);
            }while(((op0.bias+op1.bias+2*W)>=128) || ((op0.bias+op1.bias-2*(W+F))<-128));

        }else{
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].acc = rand() % (1<<W);
                    op1.data[j][k].acc = rand() % (1<<W);
                }
            }
            do{
                op0.bias = rand() % (1<<8);
                op1.bias = rand() % (1<<8);
            }while(((op0.bias+op1.bias+2*W)>=128) || ((op0.bias+op1.bias-2*(W+F))<-128));
        }

        // Get result from DUT.
        prd = op0 * op1;

        // Calculate float64 reference.
        long ref_max_data = 0;    // Keep track of most pos value.
        long ref_min_data = 0;    // Keep track of most neg value.
        bool zero_data = true;    // Set to false if non-zero output.

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){

                ref_prd[i][j] = 0;
                for(int k=0; k<N; k++){
                    ref_prd[i][j] += op0.data[i][k].acc * op1.data[k][j].acc;
                }
                ref_max_data = std::max(ref_prd[i][j], ref_max_data);
                ref_min_data = std::min(ref_prd[i][j], ref_min_data);
                zero_data = (ref_prd[i][j] != 0) ? false : zero_data;
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

        // Check if bias was zeroed for cases where all data is zero.
        // if(zero_data && (prd.bias != 0)){
        //     printf("[ERROR] FAILED: Non-zero bias with zero data.\n");
        //     throw 1;
        // }

        // Check if product is normalised.
        bool normalised = false;
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                normalised = (prd.data[j][k].acc[2*W+CLOG2(N)-1] ^ prd.data[j][k].acc[2*W+CLOG2(N)-2]) ? true : normalised;
            }
        }
        if(!zero_data && !normalised && MUL_NORM){
            printf("[ERROR] FAILED: Product not normalised.\n");
            throw 1;
        }

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((ref_prd[j][k] * pow(2,ref_bias-2*F)) != (double(prd.data[j][k]) * pow(2,prd.bias))){

                    // std::cout << "RefBias " << op0.bias << " + " << op1.bias << " -> " << ref_bias << "\n";
                    // std::cout << "TestBias " << prd.bias << "\n";
                    // std::cout << "Inputs " << double(op0.data[j][k]) << " * " << double(op1.data[j][k]) << "\n";
                    // std::cout << "Ref[" << j << ", " << k << "] = " << double(ref_prd[j][k]) << "\n";
                    // std::cout << "Test[" << j << ", " << k << "] = " << double(prd.data[j][k]) << "\n";
                    // std::cout << "Ref. SAmt: " << ref_shift_amt << ", " << ref_max_data << '\n';

                    printf("[ERROR] FAILED: Reference does not match DUT.\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}

