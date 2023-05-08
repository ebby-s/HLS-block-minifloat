

template <int E, int M>
int mf_mul_tb(){    // Test MiniFloat multiplier by exhaustive search.

    MiniFloat<E,M> op0, op1;                 // op0, op1 -> test multiplier inputs.
    IntAcc<WPRD(E,M), FPRD(E,M)> prd;  // prd = op0 * op1, test multiplier output.

    // Log parameters.
    std::cout << "[INFO] MiniFloat Mul, Parameters: E=" << E << " M=" << M << " W=" << WPRD(E,M) << " F=" << FPRD(E,M) << '\n';

    for(int i=0; i<pow(2,E+M+1); i++){
        for(int j=0; j<pow(2,E+M+1); j++){

            op0.data = i;   // Generate op0.

            op1.data = j;   // Generate op1.

            prd = op0 * op1;   // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((float(op0) * float(op1)) != double(prd)){
                std::cout << '\n' << float(op0) << " * " << float(op1) << " = " << double(prd) << '\n';
                std::cout << "op0: " << op0.data << " op1: " << op1.data << '\n';
                std::cout << "prd: " << prd.acc << '\n';

                printf("[ERROR] FAILED\n");
                throw 1;
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}


template <int E, int M>
int mf_mul_tb_rand(){    // Test MiniFloat multiplier by exhaustive search.

    MiniFloat<E,M> op0, op1;                 // op0, op1 -> test multiplier inputs.
    IntAcc<WPRD(E,M), FPRD(E,M)> prd;  // prd = op0 * op1, test multiplier output.

    // Log parameters.
    std::cout << "[INFO] MiniFloat Mul (Rand), Parameters: E=" << E << " M=" << M << " W=" << WPRD(E,M) << " F=" << FPRD(E,M) << '\n';

    for(int i=0; i<pow(2,9); i++){
        for(int j=0; j<pow(2,9); j++){

            op0.data = rand();   // Generate op0.

            op1.data = rand();   // Generate op1.

            prd = op0 * op1;   // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((double(op0) * double(op1)) != double(prd)){
                std::cout << '\n' << double(op0) << " * " << double(op1) << " = " << double(prd) << '\n';
                std::cout << "op0: " << op0.data << " op1: " << op1.data << '\n';
                std::cout << "prd: " << prd.acc << '\n';

                printf("[ERROR] FAILED\n");
                throw 1;
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}

