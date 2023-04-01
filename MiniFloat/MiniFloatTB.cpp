#include <iostream>
#include <cassert>

#include "MiniFloat.hpp"
#include "DotPrd2.hpp"

template <int E, int M>
int mul_tb(){    // Test multiplier by exhaustive search.

    MiniFloat<E,M> op0, op1;                 // op0 and op1 are inputs to multiplier.
    KulischAcc<WfromEM(E,M), FfromEM(E,M)> prd;  // prd = op0 * op1, output of multiplier.

    for(int i=0; i<pow(2,E+M+1); i++){
        for(int j=0; j<pow(2,E+M+1); j++){

            op0 = i;   // Generate op0.

            op1 = j;   // Generate op1.

            prd = op0 * op1;        // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((float(op0) * float(op1)) != float(prd)){
                std::cout << '\n' << float(op0) << " * " << float(op1) << " = " << float(prd) << '\n';
                std::cout << "op0: " << op0.data << "op1: " << op1.data << '\n';
                //std::cout << "prd: " << prd.acc << '\n';

                printf("Multiplier Failed. \n");
                std::cout << "Parameters: E=" << E << " M=" << M << '\n';
                throw 1;
            }
        }
    }

    printf("Multiplier Test Complete. \n");
    std::cout << "Parameters: E=" << E << " M=" << M << '\n';
    return 0;
}

template<int E, int M>
int add_tb(){    // Test adder by exhaustive search.

    KulischAcc<WfromEM(E,M),   FfromEM(E,M)> op0, op1;  // op0 and op1 are inputs to multiplier.
    KulischAcc<WfromEM(E,M)+1, FfromEM(E,M)> sum;       // sum = op0 + op1, output of multiplier.

    for(int i=0; i<pow(2,WfromEM(E,M)); i++){
        for(int j=0; j<pow(2,WfromEM(E,M)); j++){

            op0 = i;   // Generate op0.

            op1 = j;   // Generate op1.

            sum = op0 + op1;        // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((float(op0) + float(op1)) != float(sum)){
                std::cout << '\n' << float(op0) << " + " << float(op1) << " = " << float(sum) << '\n';
                //std::cout << "op0: " << op0.acc << "op1: " << op1.acc << "sum: " << sum.acc << '\n';

                printf("Adder Failed. \n");
                std::cout << "Parameters: E=" << E << " M=" << M << '\n';
                throw 1;
            }
        }
    }

    printf("Adder Test Complete. \n");
    std::cout << "Parameters: E=" << E << " M=" << M << '\n';
    return 0;
}


template<int E, int M>
int full_tb(){    // Test dot product circuit by exhaustive search.

    MiniFloat<E,M> op0, op1, op2, op3;               // [op0, op2] and [op1, op3] are inputs to DP circuit.
    KulischAcc<WfromEM(E,M)+1, 2*(M-1)> dot_prd;     // Output of DP circuit.

    for(int i=0; i<pow(2,E+M+1); i++){
        for(int j=0; j<pow(2,E+M+1); j++){
            for(int k=0; k<pow(2,E+M+1); k++){
                for(int l=0; l<pow(2,E+M+1); l++){

                    op0 = i;   // Generate op0.
                    op1 = j;   // Generate op1.
                    op2 = k;   // Generate op2.
                    op3 = l;   // Generate op3.

                    dot_prd = DotPrd2(op0,op1,op2,op3);    // Get result from DUT.

                    // Compare DUT result to reference done in float(f32).
                    // Assuming f32 has sufficient precision to represent mul_out_t without error.
                    if(((float(op0) * float(op1)) + (float(op2) * float(op3))) != float(dot_prd)){
                        std::cout << '\n'<<float(op0)<<" * "<<float(op1)<<" + "<<float(op2)<<" * "<<float(op3)<<" = "<<float(dot_prd)<<'\n';

                        printf("DP2 Failed. \n");
                        std::cout << "Parameters: E=" << E << " M=" << M << '\n';
                        throw 1;
                    }
                }
            }
        }
    }

    printf("DP2 Test Complete. \n");
    std::cout << "Parameters: E=" << E << " M=" << M << '\n';
    return 0;
}


int main(){

    try{

        mul_tb<2,0>();
        mul_tb<2,1>();
        mul_tb<2,2>();
        mul_tb<3,0>();
        mul_tb<3,1>();
        mul_tb<3,2>();
        mul_tb<4,3>();

        add_tb<2,0>();
        add_tb<2,1>();
        add_tb<2,2>();

        full_tb<Et,Mt>();

    }catch(int e){
        std::cout << "Scalar Operator TB Failed.\n";
    }

    return 0;
}

