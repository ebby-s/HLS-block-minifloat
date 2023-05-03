#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "BlockMat.hpp"

#include "HelperTB.hpp"
#include "BlockMulTB.hpp"
#include "BlockConvTB.hpp"


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
		bmf_mul_tb<3,4,3>();

		bmf_mul_tb<4,2,0>();
		bmf_mul_tb<4,2,1>();
		bmf_mul_tb<4,2,2>();
		bmf_mul_tb<4,3,0>();
		bmf_mul_tb<4,3,1>();
		bmf_mul_tb<4,3,2>();
		bmf_mul_tb<4,4,3>();

		bmf_mul_tb<15,2,0>();
		bmf_mul_tb<16,2,0>();
		bmf_mul_tb<17,2,0>();
		bmf_mul_tb<32,2,0>();
		bmf_mul_tb<48,2,0>();
		bmf_mul_tb<64,2,0>();
		bmf_mul_tb<65,2,0>();

		bfp_mul_tb<1,4,0>();
		bfp_mul_tb<1,4,1>();
		bfp_mul_tb<1,4,2>();
		bfp_mul_tb<1,5,0>();
		bfp_mul_tb<1,5,1>();
		bfp_mul_tb<1,5,2>();
		bfp_mul_tb<1,6,3>();

		bfp_mul_tb<2,4,0>();
		bfp_mul_tb<2,4,1>();
		bfp_mul_tb<2,4,2>();
		bfp_mul_tb<2,5,0>();
		bfp_mul_tb<2,5,1>();
		bfp_mul_tb<2,5,2>();
		bfp_mul_tb<2,6,3>();

		bfp_mul_tb<3,4,0>();
		bfp_mul_tb<3,4,1>();
		bfp_mul_tb<3,4,2>();
		bfp_mul_tb<3,5,0>();
		bfp_mul_tb<3,5,1>();
		bfp_mul_tb<3,5,2>();
		bfp_mul_tb<3,6,3>();

		bfp_mul_tb<4,4,0>();
		bfp_mul_tb<4,4,1>();
		bfp_mul_tb<4,4,2>();
		bfp_mul_tb<4,5,0>();
		bfp_mul_tb<4,5,1>();
		bfp_mul_tb<4,5,2>();
		bfp_mul_tb<4,6,3>();

		bfp_mul_tb<15,4,0>();
		bfp_mul_tb<16,4,0>();
		bfp_mul_tb<17,4,0>();
		bfp_mul_tb<32,4,0>();
		bfp_mul_tb<48,4,0>();

		bmf_to_bfp_tb<1,2,0>();
		bmf_to_bfp_tb<1,2,1>();
		bmf_to_bfp_tb<1,2,2>();
		bmf_to_bfp_tb<1,3,0>();
		bmf_to_bfp_tb<1,3,1>();
		bmf_to_bfp_tb<1,3,2>();
		bmf_to_bfp_tb<1,4,3>();

		bmf_to_bfp_tb<2,2,0>();
		bmf_to_bfp_tb<2,2,1>();
		bmf_to_bfp_tb<2,2,2>();
		bmf_to_bfp_tb<2,3,0>();
		bmf_to_bfp_tb<2,3,1>();
		bmf_to_bfp_tb<2,3,2>();
		bmf_to_bfp_tb<2,4,3>();

		bmf_to_bfp_tb<3,2,0>();
		bmf_to_bfp_tb<3,2,1>();
		bmf_to_bfp_tb<3,2,2>();
		bmf_to_bfp_tb<3,3,0>();
		bmf_to_bfp_tb<3,3,1>();
		bmf_to_bfp_tb<3,3,2>();
		bmf_to_bfp_tb<3,4,3>();

		bmf_to_bfp_tb<4,2,0>();
		bmf_to_bfp_tb<4,2,1>();
		bmf_to_bfp_tb<4,2,2>();
		bmf_to_bfp_tb<4,3,0>();
		bmf_to_bfp_tb<4,3,1>();
		bmf_to_bfp_tb<4,3,2>();
		bmf_to_bfp_tb<4,4,3>();

		bmf_to_bfp_tb<15,2,0>();
		bmf_to_bfp_tb<16,2,0>();
		bmf_to_bfp_tb<17,2,0>();
		bmf_to_bfp_tb<32,2,0>();
		bmf_to_bfp_tb<48,2,0>();
		bmf_to_bfp_tb<64,2,0>();
		bmf_to_bfp_tb<65,2,0>();


    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}

