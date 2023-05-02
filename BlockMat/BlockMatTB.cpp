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
int bmf_mul_tb(bool test_of_uf = false){    // Test BMF multiplier with random stimulus.

    BlockMF<N,E,M> op0, op1;                        // op0, op1 -> inputs to mat mul.
    BlockFP<N, WPRD(E,M)+CLOG2(N), FPRD(E,M)> prd;  // prd = op0 * op1, output of mat mul.

    double ref_prd [N][N];   // Reference product, ignoring shared exponent.
    int    ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] BMF Mul, Parameters: N=" << N << " E=" << E << " M=" << M << '\n';

    // Bias mask, test overflow/underflow with 0xff, disable with 0x3f.
    int bias_mask = test_of_uf ? (1<<8) : (1<<6);

    for(int l=0; l<(1<<(E+M+1)); l++){    // Test with random inputs.

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

        ref_max_data_int = ref_max_data * pow(2,FPRD(E,M));  // Convert to int.
        ref_min_data_int = ref_min_data * pow(2,FPRD(E,M));

        ref_ldd = std::max(clog2l(ref_max_data_int), clog2l(ref_min_data_int));

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

    BlockMF<N,W,F> op0, op1;  // op0, op1 -> inputs to mat mul.
    BlockFP<N,W,F> prd;       // prd = op0 * op1, output of mat mul.

    double ref_prd [N][N];   // Reference product, ignoring shared exponent.
    int    ref_bias;         // Reference shared exponent.

    // Log parameters.
    std::cout << "[INFO] BFP Mul, Parameters: N=" << N << " W=" << W << " F=" << F << '\n';

    // Bias mask, test overflow/underflow with 0xff, disable with 0x3f.
    int bias_mask = test_of_uf ? (1<<8) : (1<<6);

    for(int l=0; l<(1<<W); l++){    // Test with random inputs.

        // Generate op0 & op1 randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                op0.data[j][k].acc = rand() % (1<<W);
                op1.data[j][k].acc = rand() % (1<<W);
            }
        }
        op0.bias = rand() % bias_mask;
        op1.bias = rand() % bias_mask;

        // Get result from DUT.
        prd = op0 * op1;

        // Calculate float64 reference.
        // Keep track of most positive/negative values.
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

        ref_max_data_int = ref_max_data * pow(2,FPRD(E,M));  // Convert to int.
        ref_min_data_int = ref_min_data * pow(2,FPRD(E,M));

        ref_ldd = std::max(clog2l(ref_max_data_int), clog2l(ref_min_data_int));

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

                    printf("[ERROR] FAILED\n");
                    throw 1;
                }
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}


template <int N, int E, int M, int W, int F>
int bmf_to_bfp_tb(){    // Test BMP to BFP conversion with random stimulus.

    BlockMF<N,E,M> bmf_in;    // BMF input.
    BlockFP<N,W,F> bfp_out;   // BFP output.

    // Log parameters.
    std::cout << "[INFO] BMF to BFP, Parameters: N=" << N << " E=" << E << " M=" << M << " W=" << W << " F=" << F << '\n';

    for(int l=0; l<(1<<(E+M+1)); l++){    // Test with random inputs.

        // Generate input randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                bmf_in.data[j][k].data = rand() % (1<<(E+M+1));
            }
        }
        bmf_in.bias = rand() % (1<<8);

        // Get result from DUT.
        bfp_out = bmf_in;

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){

                if((double(bmf_in.data[j][k]) * pow(2,bmf_in.bias)) != (double(bfp_out.data[j][k]) * pow(2,bfp_out.bias))){

                    std::cout << "Bias (in, out): " << bmf_in.bias << ", " << bfp_out.bias << '\n';
                    std::cout << "MF (in, out): " << double(bmf_in.data[j][k]) << ", " << double(bfp_out.data[j][k]) << '\n';
                    std::cout << "Raw (in, out): " << bmf_in.data[j][k].data << ", " << bfp_out.data[j][k].acc << '\n';
                    std::cout << W << ' ' << WPRD(E,M)/2 << '\n';
                    std::cout << F << ' ' << FPRD(E,M)/2 << '\n';

                    printf("[ERROR] FAILED\n");
                    throw 1;
                }
            }
        }
    }

    printf("[INFO] Pass\n");
    return 0;
}


int main(){

    try{

        bmf_mul_tb<1,2,0>();
        bmf_mul_tb<1,2,1>();
        bmf_mul_tb<1,2,2>();
        bmf_mul_tb<1,3,0>();
        bmf_mul_tb<1,3,1>();
        bmf_mul_tb<1,3,2>();
        bmf_mul_tb<1,4,3>();

        bmf_mul_tb<2,2,0>();
        bmf_mul_tb<2,2,1>();
        bmf_mul_tb<2,2,2>();
        bmf_mul_tb<2,3,0>();
        bmf_mul_tb<2,3,1>();
        bmf_mul_tb<2,3,2>();
        bmf_mul_tb<2,4,3>();

        bmf_mul_tb<3,2,0>();
        bmf_mul_tb<3,2,1>();
        bmf_mul_tb<3,2,2>();
        bmf_mul_tb<3,3,0>();
        bmf_mul_tb<3,3,1>();
        bmf_mul_tb<3,3,2>();

        bmf_mul_tb<4,2,0>();
        bmf_mul_tb<4,2,1>();
        bmf_mul_tb<4,2,2>();
        bmf_mul_tb<4,3,0>();
        bmf_mul_tb<4,3,1>();
        bmf_mul_tb<4,3,2>();

        bmf_mul_tb<5,2,0>();
        bmf_mul_tb<5,2,1>();
        bmf_mul_tb<5,2,2>();
        bmf_mul_tb<5,3,0>();
        bmf_mul_tb<5,3,1>();
        bmf_mul_tb<5,3,2>();

        bmf_mul_tb<15,2,0>();
        bmf_mul_tb<16,2,0>();
        bmf_mul_tb<17,2,0>();
        bmf_mul_tb<32,2,0>();
        bmf_mul_tb<48,2,0>();
        bmf_mul_tb<64,2,0>();
        bmf_mul_tb<65,2,0>();

        bmf_to_bfp_tb<1,2,0,WPRD(2,0)/2,FPRD(2,0)/2>();
        bmf_to_bfp_tb<1,2,0,WPRD(2,0)/2,2>();
        bmf_to_bfp_tb<1,2,0,WPRD(2,0)/2,-4>();
        bmf_to_bfp_tb<1,2,0,8,2>();

    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}