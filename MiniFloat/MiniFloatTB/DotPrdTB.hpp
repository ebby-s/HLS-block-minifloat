

template<int N, int E, int M>
int mf_dp_tb(){    // Test dot product circuit by exhaustive search.

    MiniFloat<E,M>                       op0[N], op1[N];   // op0, op1 -> test inputs to DP circuit.
    IntAcc<WPRD(E,M)+CLOG2(N),FPRD(E,M)> dot_prd;          // dot_prd = op0 * op1, test output of DP circuit.

    // Log parameters.
    std::cout << "[INFO] MF DP Parameters: N=" << N << " E=" << E << " M=" << M << " W=" << WPRD(E,M) << " F=" << FPRD(E,M) << '\n';

    for(int l=0; l<pow(2,12); l++){

        // Generate inputs.
        for(int i=0; i<N; i++){
            op0[i].data = rand();
            op1[i].data = rand();
        }

        // Get result from DUT.
        dot_prd = MFDotPrd<N,E,M>(op0,op1);

        // Generate reference.
        long double ref_dp = 0;
        for(int i=0; i<N; i++){
            ref_dp += double(op0[i]) * double(op1[i]);
        }

        // Compare DUT result to f32 reference.
        if((long double)(dot_prd) != ref_dp){
            std::cout << ref_dp << " != " << (long double)(dot_prd) << '\n';

            printf("[ERROR] Failed.\n");
            throw 1;
        }
    }
    printf("[INFO] Passed.\n");
    return 0;
}


template<int N, int W, int F>
int ia_dp_tb(){    // Test dot product circuit by exhaustive search.

    IntAcc<W,F>              op0[N], op1[N];   // op0, op1 -> test inputs to DP circuit.
    IntAcc<2*W+CLOG2(N),2*F> dot_prd;          // dot_prd = op0 * op1, test output of DP circuit.

    // Log parameters.
    std::cout << "[INFO] IA DP Parameters: N=" << N << " W=" << W << " F=" << F << '\n';

    for(int l=0; l<pow(2,12); l++){

        // Generate inputs.
        for(int i=0; i<N; i++){
            op0[i].acc = rand() + (long(rand()) << 32);
            op1[i].acc = rand() + (long(rand()) << 32);
        }

        // Get result from DUT.
        dot_prd = IADotPrd<N,W,F>(op0,op1);

        // Generate reference.
        long double ref_dp = 0;
        for(int i=0; i<N; i++){
            ref_dp += double(op0[i]) * double(op1[i]);
        }

        // Compare DUT result to f32 reference.
        if((long double)(dot_prd) != ref_dp){
            std::cout << ref_dp << " != " << (long double)(dot_prd) << '\n';

            printf("[ERROR] Failed.\n");
            throw 1;
        }
    }
    printf("[INFO] Passed.\n");
    return 0;
}

