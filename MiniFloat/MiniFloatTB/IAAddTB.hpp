

template<int W, int F>
int ia_add_tb(){    // Test adder by exhaustive search.

    IntAcc<W,   F> op0, op1;  // op0, op1 -> test adder inputs.
    IntAcc<W+1, F> sum;       // sum = op0 + op1, test adder output.

    // Log parameters.
    std::cout << "[INFO] IntAcc Add, Parameters: W=" << W << " F=" << F << '\n';

    // Abort if W is large, use random testing instead.
    if(W > 13){
        printf("[ERROR] W is large, use random adder test.\n");
        throw 1;
    }

    for(int i=0; i<pow(2,W); i++){
        for(int j=0; j<pow(2,W); j++){

            op0.acc = i;   // Generate op0.

            op1.acc = j;   // Generate op1.

            sum = op0 + op1;    // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((float(op0) + float(op1)) != float(sum)){
                std::cout << '\n' << float(op0) << " + " << float(op1) << " = " << float(sum) << '\n';
                //std::cout << "op0: " << op0.acc << "op1: " << op1.acc << "sum: " << sum.acc << '\n';

                printf("[ERROR] FAILED\n");
                throw 1;
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}

template<int W, int F>
int ia_add_tb_rand(){    // Test adder by random search.

    IntAcc<W,   F> op0, op1;  // op0, op1 -> test adder inputs.
    IntAcc<W+1, F> sum;       // sum = op0 + op1, test adder output.

    // Log parameters.
    std::cout << "[INFO] IntAcc Add (Rand), Parameters: W=" << W << " F=" << F << '\n';

    for(int i=0; i<pow(2,10); i++){
        for(int j=0; j<pow(2,10); j++){

            op0.acc = rand() + (rand() << 32);   // Generate op0.

            op1.acc = rand() + (rand() << 32);   // Generate op1.

            sum = op0 + op1;    // Get result from DUT.

            // Compare DUT result to reference done in double(f64).
            // Assuming f64 has sufficient precision to represent mul_out_t without error.
            if((double(op0) + double(op1)) != double(sum)){
                std::cout << '\n' << double(op0) << " + " << double(op1) << " = " << double(sum) << '\n';
                //std::cout << "op0: " << op0.acc << "op1: " << op1.acc << "sum: " << sum.acc << '\n';

                printf("[ERROR] FAILED\n");
                throw 1;
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}

