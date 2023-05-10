

template <int W, int F>
int ia_mul_tb(){    // Test IntAcc multiplier by exhaustive search.

    IntAcc<  W,  F> op0, op1;  // op0, op1 -> test multiplier inputs.
    IntAcc<2*W,2*F> prd;       // prd = op0 * op1, test multiplier output.

    // Log parameters.
    std::cout << "[INFO] IntAcc Mul, Parameters: W=" << W << " F=" << F << '\n';

    for(int i=0; i<pow(2,W); i++){
        for(int j=0; j<pow(2,W); j++){

            op0.acc = i;   // Generate op0.

            op1.acc = j;   // Generate op1.

            prd = op0 * op1;   // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((float(op0) * float(op1)) != float(prd)){
                std::cout << '\n' << float(op0) << " * " << float(op1) << " = " << float(prd) << '\n';
                std::cout << "op0: " << op0.acc << "op1: " << op1.acc << '\n';
                //std::cout << "prd: " << prd.acc << '\n';

                printf("[ERROR] FAILED\n");
                throw 1;
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}

template <int W, int F>
int ia_mul_tb_rand(){    // Test IntAcc multiplier by exhaustive search.

    IntAcc<  W,  F> op0, op1;  // op0, op1 -> test multiplier inputs.
    IntAcc<2*W,2*F> prd;       // prd = op0 * op1, test multiplier output.

    // Log parameters.
    std::cout << "[INFO] IntAcc Mul (Rand), Parameters: W=" << W << " F=" << F << '\n';

    for(int i=0; i<pow(2,9); i++){
        for(int j=0; j<pow(2,9); j++){

            op0.acc = rand() + (long(rand()) << 32);   // Generate op0.

            op1.acc = rand() + (long(rand()) << 32);   // Generate op1.

            prd = op0 * op1;   // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((double(op0) * double(op1)) != double(prd)){
                std::cout << '\n' << double(op0) << " * " << double(op1) << " = " << double(prd) << '\n';
                std::cout << "op0: " << op0.acc << "op1: " << op1.acc << '\n';
                //std::cout << "prd: " << prd.acc << '\n';

                printf("[ERROR] FAILED\n");
                throw 1;
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}

