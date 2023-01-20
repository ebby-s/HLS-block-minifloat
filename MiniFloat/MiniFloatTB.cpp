#include <iostream>
#include <cassert>

#include "../MiniFloat/MiniFloat.hpp"


template <int E, int M>
int mul_tb(){    // Test multiplier by exhaustive search.

    MiniFloat<E,M> op0, op1;                 // op0 and op1 are inputs to multiplier.
    KulischAcc<((1<<E)+M)*2, 2*(M-1)> prd;  // prd = op0 * op1, output of multiplier.

    for(int i=0; i<pow(2,E+M+1); i++){
        for(int j=0; j<pow(2,E+M+1); j++){

            op0 = i;   // Generate op0.

            op1 = j;   // Generate op1.

            prd = op0 * op1;        // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((float(op0) * float(op1)) != float(prd)){
                std::cout << '\n';
                std::cout << "op0: " << float(op0) << '\n';
                //std::cout << "op0: " << op0.man << ' ' << op0.exp << '\n';
                std::cout << "op1: " << float(op1) << '\n';
                //std::cout << "op1: " << op1.man << ' ' << op1.exp << '\n';
                std::cout << "prd: " << float(prd) << '\n';
                //std::cout << "prd: " << prd.acc << '\n';
                std::cout << '\n';

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

    KulischAcc<((1<<E)+M)*2,   2*(M-1)> op0, op1;  // op0 and op1 are inputs to multiplier.
    KulischAcc<((1<<E)+M)*2+1, 2*(M-1)> sum;       // sum = op0 + op1, output of multiplier.

    for(int i=0; i<pow(2,((1<<E)+M)*2); i++){
        for(int j=0; j<pow(2,((1<<E)+M)*2); j++){

            op0 = i;   // Generate op0.

            op1 = j;   // Generate op1.

            sum = op0 + op1;        // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((float(op0) + float(op1)) != float(sum)){
                std::cout << '\n';
                std::cout << "op0: " << float(op0) << '\n';
                //std::cout << "op0: " << op0.acc << '\n';
                std::cout << "op1: " << float(op1) << '\n';
                //std::cout << "op1: " << op1.acc << '\n';
                std::cout << "sum: " << float(sum) << '\n';
                //std::cout << "sum: " << sum.acc << '\n';
                std::cout << '\n';

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

    }catch(int e){
        std::cout << "Scalar Operator TB Failed.\n";
    }

    return 0;
}

