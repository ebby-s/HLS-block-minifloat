#include <iostream>
#include <cassert>

#include "MiniFloat.hpp"
#include "DotPrd2.hpp"

template <int E, int M>
int MiniFloat_mul_tb(){    // Test MiniFloat multiplier by exhaustive search.

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
            if((float(op0) * float(op1)) != float(prd)){
                std::cout << '\n' << float(op0) << " * " << float(op1) << " = " << float(prd) << '\n';
                std::cout << "op0: " << op0.data << "op1: " << op1.data << '\n';
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
int IntAcc_mul_tb(){    // Test IntAcc multiplier by exhaustive search.

    IntAcc<W,   F> op0, op1;  // op0, op1 -> test multiplier inputs.
    IntAcc<2*W, F> prd;       // prd = op0 * op1, test multiplier output.

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

template<int W, int F>
int IntAcc_add_tb(){    // Test adder by exhaustive search.

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
int IntAcc_add_tb_rand(){    // Test adder by random search.

    IntAcc<W,   F> op0, op1;  // op0, op1 -> test adder inputs.
    IntAcc<W+1, F> sum;       // sum = op0 + op1, test adder output.

    // Log parameters.
    std::cout << "[INFO] IntAcc Add (Rand), Parameters: W=" << W << " F=" << F << '\n';

    for(int i=0; i<pow(2,13); i++){
        for(int j=0; j<pow(2,13); j++){

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


int main(){

    try{

        MiniFloat_mul_tb<2,0>();
        MiniFloat_mul_tb<2,1>();
        MiniFloat_mul_tb<2,2>();
        MiniFloat_mul_tb<3,0>();
        MiniFloat_mul_tb<3,1>();
        MiniFloat_mul_tb<3,2>();
        MiniFloat_mul_tb<4,3>();

        IntAcc_mul_tb<2,0>();
        IntAcc_mul_tb<3,0>();
        IntAcc_mul_tb<4,0>();
        IntAcc_mul_tb<5,0>();
        IntAcc_mul_tb<8,0>();
        IntAcc_mul_tb<9,0>();

        IntAcc_add_tb<WPRD(2,0), FPRD(2,0)>();
        IntAcc_add_tb<WPRD(2,1), FPRD(2,1)>();
        IntAcc_add_tb<WPRD(2,2), FPRD(2,2)>();

        IntAcc_add_tb_rand<WPRD(3,0), FPRD(3,0)>();
        IntAcc_add_tb_rand<WPRD(3,1), FPRD(3,1)>();
        IntAcc_add_tb_rand<WPRD(3,2), FPRD(3,2)>();
        IntAcc_add_tb_rand<WPRD(4,3), FPRD(4,3)>();

        DP2_tb<Et,Mt>();

    }catch(int e){
        std::cout << "Scalar Operator TB Failed.\n";
    }

    return 0;
}

