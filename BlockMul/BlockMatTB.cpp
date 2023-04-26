#include <iostream>
#include <cassert>
#include <cstdlib>
#include <typeinfo>
#include <cmath>

#include "BlockMat.hpp"


// Helper function to calculate the number of bits required
// to represent n using 2's cmpl format.
int clog2l(long n){

    int count = 0;

    n = (n < 0) ? n+1 : n; // Decrease magnitude by 1 if number is negative.

    n = std::labs(n);  // Take abs value before counting leading zeros.

    for(int i=(sizeof(long)*8)-1; i>=0; i--){
        if(n >> i){
            break;
        }else{
            count++;
        }
    }

    return (sizeof(long)*8) - count + 1;
}


template <int N, int E, int M>
int bmul_tb(){    // Test block multiplier with random stimulus.

    BlockMat<N,E,M> op0, op1;                              // op0, op1 -> inputs to mat mul.
    BlockAcc<N, WfromEM(E,M)+CLOG2(N), FfromEM(E,M)> prd;  // prd = op0 * op1, output of mat mul.

    double ref_prd [N][N];   // Reference product, ignoring shared exponent.
    int    ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] Testing Parameters: N=" << N << " E=" << E << " M=" << M << '\n';

    for(int l=0; l<(1<<(E+M+1)); l++){    // Test with random inputs.

        // Generate op0 & op1 randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                op0.data[j][k].data = rand() % (1<<(E+M+1));
                op1.data[j][k].data = rand() % (1<<(E+M+1));
            }
        }
        op0.bias = rand() % (1<<8);
        op1.bias = rand() % (1<<8);

        // Get result from DUT.
        prd = op0 * op1;

        // Calculate float64 reference.
        // Keep track of most positive/most negative values.
        double ref_max_data = 0;
        double ref_min_data = 0;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){

                ref_prd[i][j] = 0;

                for(int k=0; k<N; k++){
                    ref_prd[i][j] += (double(op0.data[i][k]) * double(op1.data[k][j]));
                }

                ref_max_data = std::max(ref_prd[i][j], ref_max_data);
                ref_min_data = std::min(ref_prd[i][j], ref_min_data);
            }
        }


        // Model normalization step.
        long ref_max_data_int;
        long ref_min_data_int;
        long ref_ldd;
        long ref_shift_amt;

        ref_max_data_int = ref_max_data * pow(2,FfromEM(E,M));  // Convert to int.
        ref_min_data_int = ref_min_data * pow(2,FfromEM(E,M));

        ref_ldd = std::max(clog2l(ref_max_data_int), clog2l(ref_min_data_int));

        ref_shift_amt = WfromEM(E,M)+CLOG2(N) - ref_ldd;

        ref_bias = op0.bias + op1.bias - ref_shift_amt;

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                ref_prd[i][j] *= pow(2, ref_shift_amt);
            }
        }

        // Saturate reference bias in case of overflow.
        if(ref_bias >= (1 << (8-1))){
            ref_bias = (1 << (8-1)) -1;
        }

        // Set reference to zero in case of underflow.
        if(ref_bias < -(1 << (8-1))){
            for(int i=0; i<N; i++){
                for(int j=0; j<N; j++){
                    ref_prd[i][j] = 0;
                }
            }
        }

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){

                if((ref_prd[j][k] * pow(2,ref_bias)) != (double(prd.data[j][k]) * pow(2,prd.bias))){

                    // std::cout << "RefBias " << op0.bias << " + " << op1.bias << " -> " << ref_bias << "\n";
                    // std::cout << "TestBias " << prd.bias << "\n";
                    // std::cout << "W, cl2(N): " << WfromEM(E,M) << ", " << CLOG2(N) << "\n";

                    // std::cout << "Ref[" << j << ", " << k << "] = " << double(ref_prd[j][k]) << "\n";
                    // std::cout << "Test[" << j << ", " << k << "] = " << double(prd.data[j][k]) << "\n";
                    // std::cout << "Ref. SAmt: " << ref_shift_amt << ", " << ref_max_data << '\n';
                    // std::cout << ref_max_data_int << ", " << ref_min_data_int << ", " << ref_ldd << '\n';

                    printf("[ERROR] Block Multiplier Failed.\n");
                    throw 1;
                }
            }
        }
    }

    printf("[INFO] Block Multiplier Test Complete.\n");
    return 0;
}


int main(){

    try{

        bmul_tb<1,2,0>();
        bmul_tb<1,2,1>();
        bmul_tb<1,2,2>();
        bmul_tb<1,3,0>();
        bmul_tb<1,3,1>();
        bmul_tb<1,3,2>();
        bmul_tb<1,4,3>();

        bmul_tb<2,2,0>();
        bmul_tb<2,2,1>();
        bmul_tb<2,2,2>();
        bmul_tb<2,3,0>();
        bmul_tb<2,3,1>();
        bmul_tb<2,3,2>();
        bmul_tb<2,4,3>();

        bmul_tb<3,2,0>();
        bmul_tb<3,2,1>();
        bmul_tb<3,2,2>();
        bmul_tb<3,3,0>();
        bmul_tb<3,3,1>();
        bmul_tb<3,3,2>();

        bmul_tb<4,2,0>();
        bmul_tb<4,2,1>();
        bmul_tb<4,2,2>();
        bmul_tb<4,3,0>();
        bmul_tb<4,3,1>();
        bmul_tb<4,3,2>();

        bmul_tb<5,2,0>();
        bmul_tb<5,2,1>();
        bmul_tb<5,2,2>();
        bmul_tb<5,3,0>();
        bmul_tb<5,3,1>();
        bmul_tb<5,3,2>();

        bmul_tb<15,2,0>();
        bmul_tb<16,2,0>();
        bmul_tb<17,2,0>();
        bmul_tb<32,2,0>();
        bmul_tb<48,2,0>();
        bmul_tb<64,2,0>();
        bmul_tb<65,2,0>();

    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}