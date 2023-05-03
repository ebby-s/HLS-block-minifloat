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

		bfp_to_bmf_tb<1,4,-1,2,0>();
		bfp_to_bmf_tb<1,4,-1,2,1>();
		bfp_to_bmf_tb<1,4,-1,3,0>();
		bfp_to_bmf_tb<1,4,-1,4,4>();
		bfp_to_bmf_tb<1,4,-1,4,6>();
		bfp_to_bmf_tb<1,4,0,2,0>();
		bfp_to_bmf_tb<1,4,0,2,1>();
		bfp_to_bmf_tb<1,4,0,3,0>();
		bfp_to_bmf_tb<1,4,0,4,4>();
		bfp_to_bmf_tb<1,4,0,4,6>();
		bfp_to_bmf_tb<1,4,2,2,0>();
		bfp_to_bmf_tb<1,4,2,2,1>();
		bfp_to_bmf_tb<1,4,2,3,0>();
		bfp_to_bmf_tb<1,4,2,4,4>();
		bfp_to_bmf_tb<1,4,2,4,6>();

		bfp_to_bmf_tb<2,4,-1,2,0>();
		bfp_to_bmf_tb<2,4,-1,2,1>();
		bfp_to_bmf_tb<2,4,-1,3,0>();
		bfp_to_bmf_tb<2,4,-1,4,4>();
		bfp_to_bmf_tb<2,4,-1,4,6>();
		bfp_to_bmf_tb<2,4,0,2,0>();
		bfp_to_bmf_tb<2,4,0,2,1>();
		bfp_to_bmf_tb<2,4,0,3,0>();
		bfp_to_bmf_tb<2,4,0,4,4>();
		bfp_to_bmf_tb<2,4,0,4,6>();
		bfp_to_bmf_tb<2,4,2,2,0>();
		bfp_to_bmf_tb<2,4,2,2,1>();
		bfp_to_bmf_tb<2,4,2,3,0>();
		bfp_to_bmf_tb<2,4,2,4,4>();
		bfp_to_bmf_tb<2,4,2,4,6>();

		bfp_to_bmf_tb<3,4,-1,2,0>();
		bfp_to_bmf_tb<3,4,-1,2,1>();
		bfp_to_bmf_tb<3,4,-1,3,0>();
		bfp_to_bmf_tb<3,4,-1,4,4>();
		bfp_to_bmf_tb<3,4,-1,4,6>();
		bfp_to_bmf_tb<3,4,0,2,0>();
		bfp_to_bmf_tb<3,4,0,2,1>();
		bfp_to_bmf_tb<3,4,0,3,0>();
		bfp_to_bmf_tb<3,4,0,4,4>();
		bfp_to_bmf_tb<3,4,0,4,6>();
		bfp_to_bmf_tb<3,4,2,2,0>();
		bfp_to_bmf_tb<3,4,2,2,1>();
		bfp_to_bmf_tb<3,4,2,3,0>();
		bfp_to_bmf_tb<3,4,2,4,4>();
		bfp_to_bmf_tb<3,4,2,4,6>();

		bfp_to_bmf_tb<4,4,-1,2,0>();
		bfp_to_bmf_tb<4,4,-1,2,1>();
		bfp_to_bmf_tb<4,4,-1,3,0>();
		bfp_to_bmf_tb<4,4,-1,4,4>();
		bfp_to_bmf_tb<4,4,-1,4,6>();
		bfp_to_bmf_tb<4,4,0,2,0>();
		bfp_to_bmf_tb<4,4,0,2,1>();
		bfp_to_bmf_tb<4,4,0,3,0>();
		bfp_to_bmf_tb<4,4,0,4,4>();
		bfp_to_bmf_tb<4,4,0,4,6>();
		bfp_to_bmf_tb<4,4,2,2,0>();
		bfp_to_bmf_tb<4,4,2,2,1>();
		bfp_to_bmf_tb<4,4,2,3,0>();
		bfp_to_bmf_tb<4,4,2,4,4>();
		bfp_to_bmf_tb<4,4,2,4,6>();

		bfp_to_bmf_tb<15,10,3,3,2>();
		bfp_to_bmf_tb<16,10,3,3,2>();
		bfp_to_bmf_tb<17,10,3,3,2>();
		bfp_to_bmf_tb<32,10,3,3,2>();
		bfp_to_bmf_tb<48,10,3,3,2>();
		bfp_to_bmf_tb<65,10,3,3,2>();


    }catch(int e){
        std::cout << "Block Operator TB Failed.\n";
    }

    return 0;
}

