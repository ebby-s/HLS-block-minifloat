#include <iostream>
#include <cassert>
#include <cstdlib>
#include <typeinfo>
#include <cmath>

#include "BlockMat.hpp"


template <int N, int E, int M>
int bmul_tb(){    // Test block multiplier.

    BlockMat<N,E,M> op0, op1;                     // op0 and op1 are inputs to multiplier.
    BlockAcc<N, WfromEM(E,M)+CLOG2(N), FfromEM(E,M)> prd;  // prd = op0 * op1, output of multiplier.

    bool fail;
    float ref_prd [N][N];

    for(int l=0; l<(1<<(E+M+1)); l++){

            // Generate op0 & op1.
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k].data = rand() % (1<<(E+M+1));
                    op1.data[j][k].data = rand() % (1<<(E+M+1));
                }
            }
            op0.bias = rand() % (1<<5);
            op1.bias = rand() % (1<<5);

            prd = op0 * op1;        // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent product without error.
            fail = false;

            for(int i=0; i<N; i++){
                for(int j=0; j<N; j++){

                    ref_prd[i][j] = 0;

                    for(int k=0; k<N; k++){
                        ref_prd[i][j] += (float(op0.data[i][k]) * float(op1.data[k][j]));
                    }
                }
            }

            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    if((ref_prd[j][k] * pow(2,op0.bias+op1.bias)) != (float(prd.data[j][k]) * pow(2,prd.bias))){

                        // std::cout << "Ref[" << j << ", " << k << "] = " << ref_prd[j][k] << " * " << pow(2,op0.bias+op1.bias) << "\n";
                        // std::cout << "Ref[" << j << ", " << k << "] = " << (ref_prd[j][k] * pow(2,op0.bias+op1.bias)) << "\n";
                        // std::cout << "Test[" << j << ", " << k << "] = " << (float(prd.data[j][k]) * pow(2,prd.bias)) << "\n";
                        // std::cout << "RefBias " << op0.bias << ", " << op1.bias << "\n";
                        // std::cout << "TestBias " << prd.bias << "\n";
                        // std::cout << "Type: " << WfromEM(E,M) << CLOG2(N) << "\n";

                        fail = true;
                    }
                }
            }

            if(fail){
                // for(int i=0; i<N; i++) for(int j=0; j<N; j++) std::cout << "A[" << i << ", " << j << "] = " << float(op0.data[i][j]) << "\n";
                // for(int i=0; i<N; i++) for(int j=0; j<N; j++) std::cout << "B[" << i << ", " << j << "] = " << float(op1.data[i][j]) << "\n";
                // for(int i=0; i<N; i++) for(int j=0; j<N; j++) std::cout << "Ref[" << i << ", " << j << "] = " << float(ref_prd[i][j]) << "\n";
                // for(int i=0; i<N; i++) for(int j=0; j<N; j++) std::cout << "Test[" << i << ", " << j << "] = " << float(prd.data[i][j]) << "\n";

                printf("Block Multiplier Failed. \n");
                std::cout << "Parameters: N=" << N << " E=" << E << " M=" << M << '\n';
                throw 1;
            }
    }

    printf("Block Multiplier Test Complete. \n");
    std::cout << "Parameters: N=" << N << " E=" << E << " M=" << M << '\n';
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
        // bmul_tb<127,2,0>();

    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}