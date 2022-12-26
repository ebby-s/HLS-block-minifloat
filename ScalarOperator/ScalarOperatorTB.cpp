#include <iostream>
#include <cassert>
#include <set>
#include "ScalarOperator.hpp"

std::set<mul_out_t> mul_out_set;  // Set to store possible values of product.

int mul_tb(){    // Test multiplier by exhaustive search.

    minifloat_t op0, op1;  // op0 and op1 are inputs to multiplier.
    mul_out_t prd;         // prd = op0 * op1, output of multiplier.

    for(int i=0; i<pow(2,E+M+1); i++){
        for(int j=0; j<pow(2,E+M+1); j++){

            op0.sgn = i >> (M+E);   // Generate op0.
            op0.exp = i >>  M;
            op0.man = i;

            op1.sgn = j >> (M+E);   // Generate op1.
            op1.exp = j >>  M;
            op1.man = j;

            prd = op0 * op1;        // Get result from DUT.

            mul_out_set.insert(prd);  // Collect posible values of product, for next test.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent mul_out_t without error.
            if((mnf_to_f32(op0) * mnf_to_f32(op1)) != mul_to_f32(prd)){
                std::cout << '\n';
                std::cout << "op0: " << mnf_to_f32(op0) << '\n';
                std::cout << "op0: " << op0.man << ' ' << op0.exp << '\n';
                std::cout << "op1: " << mnf_to_f32(op1) << '\n';
                std::cout << "op1: " << op1.man << ' ' << op1.exp << '\n';
                std::cout << "prd: " << mul_to_f32(prd) << '\n';
                std::cout << "prd: " << prd.man << ' ' << prd.exp << '\n';
                std::cout << '\n';

                printf("Multiplier Failed. \n");
                return 1;
            }
        }
    }

    printf("Multiplier Test Complete. \n");
    return 0;
}

int add_prd_tb(){

    mul_out_t op0,op1;
    ap_int<W_A> sum;

    // Search seen values of product.
    for(std::set<mul_out_t>::iterator i = mul_out_set.begin(); i != mul_out_set.end(); i++){
        for(std::set<mul_out_t>::iterator j = mul_out_set.begin(); j != mul_out_set.end(); j++){

            op0 = *i;
            op1 = *j;

            sum = op0 + op1;        // Get result from DUT.

            // Compare DUT result to reference done in float(f32).
            // Assuming f32 has sufficient precision to represent sum without error.
            if((mul_to_f32(op0) + mul_to_f32(op1)) != fip_to_f32(sum)){
                std::cout << '\n';
                std::cout << "op0: " << mul_to_f32(op0) << '\n';
                std::cout << "op0: " << op0.man << ' ' << op0.exp << '\n';
                std::cout << "op1: " << mul_to_f32(op1) << '\n';
                std::cout << "op1: " << op1.man << ' ' << op1.exp << '\n';
                std::cout << "sum: " << fip_to_f32(sum) << '\n';
                std::cout << "sum: " << sum << '\n';
                std::cout << '\n';

                printf("Adder Failed. \n");
                return 1;
            }
        }
    }

    printf("Adder Test Complete. \n");
    return 0;
}

int main(){

    assert(mul_tb() == 0);
    assert(add_prd_tb() == 0);

    return 0;
}

