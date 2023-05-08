

template<int E, int M>
int DP2_tb(){    // Test dot product circuit by exhaustive search.

    MiniFloat<E,M> op0, op1, op2, op3;               // [op0, op2], [op1, op3] -> test inputs to DP circuit.
    IntAcc<WPRD(E,M)+1, 2*(M-1)> dot_prd;     // dot_prd = op0*op1 + op2*op3, test output of DP circuit.

    // Log parameters.
    std::cout << "[INFO] Testing Parameters: E=" << E << " M=" << M << " W=" << WPRD(E,M) << " F=" << FPRD(E,M) << '\n';

    for(int i=0; i<pow(2,E+M+1); i++){
        for(int j=0; j<pow(2,E+M+1); j++){
            for(int k=0; k<pow(2,E+M+1); k++){
                for(int l=0; l<pow(2,E+M+1); l++){

                    op0.data = i;   // Generate op0.
                    op1.data = j;   // Generate op1.
                    op2.data = k;   // Generate op2.
                    op3.data = l;   // Generate op3.

                    dot_prd = DotPrd2(op0,op1,op2,op3);    // Get result from DUT.

                    // Compare DUT result to reference done in float(f32).
                    // Assuming f32 has sufficient precision to represent mul_out_t without error.
                    if(((float(op0) * float(op1)) + (float(op2) * float(op3))) != float(dot_prd)){
                        std::cout << '\n'<<float(op0)<<" * "<<float(op1)<<" + "<<float(op2)<<" * "<<float(op3)<<" = "<<float(dot_prd)<<'\n';

                        printf("[ERROR] DP2 Failed.\n");
                        throw 1;
                    }
                }
            }
        }
    }

    printf("[INFO] DP2 Passed.\n");
    return 0;
}

