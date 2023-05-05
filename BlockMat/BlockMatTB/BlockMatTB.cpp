#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "../BlockMat.hpp"

#include "HelperTB.hpp"
#include "BlockMulTB.hpp"
#include "BlockConvTB.hpp"


int main(){
    try{

		bfp_to_bfp_tb<1,4,0,4,0>();
		bfp_to_bfp_tb<1,4,0,6,2>();
		bfp_to_bfp_tb<1,4,0,10,4>();
		bfp_to_bfp_tb<1,6,2,4,0>();
		bfp_to_bfp_tb<1,6,2,6,2>();
		bfp_to_bfp_tb<1,6,2,10,4>();
		bfp_to_bfp_tb<1,10,4,4,0>();
		bfp_to_bfp_tb<1,10,4,6,2>();
		bfp_to_bfp_tb<1,10,4,10,4>();

		bfp_to_bfp_tb<2,4,0,4,0>();
		bfp_to_bfp_tb<2,4,0,6,2>();
		bfp_to_bfp_tb<2,4,0,10,4>();
		bfp_to_bfp_tb<2,6,2,4,0>();
		bfp_to_bfp_tb<2,6,2,6,2>();
		bfp_to_bfp_tb<2,6,2,10,4>();
		bfp_to_bfp_tb<2,10,4,4,0>();
		bfp_to_bfp_tb<2,10,4,6,2>();
		bfp_to_bfp_tb<2,10,4,10,4>();

		bfp_to_bfp_tb<3,4,0,4,0>();
		bfp_to_bfp_tb<3,4,0,6,2>();
		bfp_to_bfp_tb<3,4,0,10,4>();
		bfp_to_bfp_tb<3,6,2,4,0>();
		bfp_to_bfp_tb<3,6,2,6,2>();
		bfp_to_bfp_tb<3,6,2,10,4>();
		bfp_to_bfp_tb<3,10,4,4,0>();
		bfp_to_bfp_tb<3,10,4,6,2>();
		bfp_to_bfp_tb<3,10,4,10,4>();

		bfp_to_bfp_tb<4,4,0,4,0>();
		bfp_to_bfp_tb<4,4,0,6,2>();
		bfp_to_bfp_tb<4,4,0,10,4>();
		bfp_to_bfp_tb<4,6,2,4,0>();
		bfp_to_bfp_tb<4,6,2,6,2>();
		bfp_to_bfp_tb<4,6,2,10,4>();
		bfp_to_bfp_tb<4,10,4,4,0>();
		bfp_to_bfp_tb<4,10,4,6,2>();
		bfp_to_bfp_tb<4,10,4,10,4>();

		bfp_to_bfp_tb<15,8,4,3,2>();
		bfp_to_bfp_tb<16,8,4,3,2>();
		bfp_to_bfp_tb<17,8,4,3,2>();
		bfp_to_bfp_tb<32,8,4,3,2>();
		bfp_to_bfp_tb<48,8,4,3,2>();
		bfp_to_bfp_tb<65,8,4,3,2>();


    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}

