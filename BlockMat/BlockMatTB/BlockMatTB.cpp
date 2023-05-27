#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

#define ADD_POST_NORM 1
#define MUL_NORM 1

#include "../BlockMat.hpp"

#include "HelperTB.hpp"
#include "BlockMulTB.hpp"
#include "BlockConvTB.hpp"
#include "BlockAddTB.hpp"
#include "NormaliseTB.hpp"

int main(){
    try{

		bfp_norm_tb<1,4,0>();
		bfp_norm_tb<1,4,1>();
		bfp_norm_tb<1,4,2>();
		bfp_norm_tb<1,5,0>();
		bfp_norm_tb<1,5,1>();
		bfp_norm_tb<1,5,2>();
		bfp_norm_tb<1,6,3>();

		bfp_norm_tb<2,4,0>();
		bfp_norm_tb<2,4,1>();
		bfp_norm_tb<2,4,2>();
		bfp_norm_tb<2,5,0>();
		bfp_norm_tb<2,5,1>();
		bfp_norm_tb<2,5,2>();
		bfp_norm_tb<2,6,3>();

		bfp_norm_tb<3,4,0>();
		bfp_norm_tb<3,4,1>();
		bfp_norm_tb<3,4,2>();
		bfp_norm_tb<3,5,0>();
		bfp_norm_tb<3,5,1>();
		bfp_norm_tb<3,5,2>();
		bfp_norm_tb<3,6,3>();

		bfp_norm_tb<4,4,0>();
		bfp_norm_tb<4,4,1>();
		bfp_norm_tb<4,4,2>();
		bfp_norm_tb<4,5,0>();
		bfp_norm_tb<4,5,1>();
		bfp_norm_tb<4,5,2>();
		bfp_norm_tb<4,6,3>();

		bfp_norm_tb<15,4,0>();
		bfp_norm_tb<16,4,0>();
		bfp_norm_tb<17,4,0>();
		bfp_norm_tb<32,4,0>();
		bfp_norm_tb<48,4,0>();


    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}

