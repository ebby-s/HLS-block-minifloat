#include <iostream>
#include <cassert>
#include <random>

#include "BlockMat.hpp"


template <int N, int E, int M>
int bmul_tb(){    // Test block multiplier.

    BlockMat<N,E,M> op0, op1;                     // op0 and op1 are inputs to multiplier.
    BlockAcc<N, WfromEM(E,M), FfromEM(E,M)> prd;  // prd = op0 * op1, output of multiplier.

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand_val(0,(1<<(E+M+1))-1);

    bool fail;
    float [N][N] ref_prd;

    for(int l=0; l<(1<<(E+M+1)); l++){

            // Generate op0 & op1.
            for(int j=0; j<N; j++){
                for(int k=0; k<N; k++){
                    op0.data[j][k] = rand_val(rng);
                    op1.data[j][k] = rand_val(rng);
                }
            }

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
                    if(ref_prd[j][k] != float(prd.data[j][k])){
                        fail = true;
                    }
                }
            }

            if(fail){
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

        bmul_tb<2,0>();
        bmul_tb<2,1>();
        bmul_tb<2,2>();
        bmul_tb<3,0>();
        bmul_tb<3,1>();
        bmul_tb<3,2>();
        bmul_tb<4,3>();

    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}