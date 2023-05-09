#include <iostream>
#include <cassert>

#include "../MiniFloat.hpp"

#include "MFMulTB.hpp"
#include "IAMulTB.hpp"
#include "IAAddTB.hpp"
#include "DotPrdTB.hpp"


int main(){
    try{

		ia_dp_tb<1,2,0>();
		ia_dp_tb<1,3,0>();
		ia_dp_tb<1,4,0>();
		ia_dp_tb<1,5,0>();
		ia_dp_tb<1,6,0>();
		ia_dp_tb<1,4,-2>();
		ia_dp_tb<1,4,-1>();
		ia_dp_tb<1,4,1>();
		ia_dp_tb<1,4,2>();
		ia_dp_tb<1,4,3>();
		ia_dp_tb<1,4,4>();
		ia_dp_tb<1,7,0>();
		ia_dp_tb<1,8,0>();
		ia_dp_tb<1,9,0>();
		ia_dp_tb<1,10,0>();
		ia_dp_tb<1,11,0>();
		ia_dp_tb<1,4,-6>();
		ia_dp_tb<1,4,-4>();
		ia_dp_tb<1,4,6>();
		ia_dp_tb<1,4,7>();
		ia_dp_tb<1,4,9>();

		ia_dp_tb<2,2,0>();
		ia_dp_tb<2,3,0>();
		ia_dp_tb<2,4,0>();
		ia_dp_tb<2,5,0>();
		ia_dp_tb<2,6,0>();
		ia_dp_tb<2,4,-2>();
		ia_dp_tb<2,4,-1>();
		ia_dp_tb<2,4,1>();
		ia_dp_tb<2,4,2>();
		ia_dp_tb<2,4,3>();
		ia_dp_tb<2,4,4>();
		ia_dp_tb<2,7,0>();
		ia_dp_tb<2,8,0>();
		ia_dp_tb<2,9,0>();
		ia_dp_tb<2,10,0>();
		ia_dp_tb<2,11,0>();
		ia_dp_tb<2,4,-6>();
		ia_dp_tb<2,4,-4>();
		ia_dp_tb<2,4,6>();
		ia_dp_tb<2,4,7>();
		ia_dp_tb<2,4,9>();

		ia_dp_tb<3,2,0>();
		ia_dp_tb<3,3,0>();
		ia_dp_tb<3,4,0>();
		ia_dp_tb<3,5,0>();
		ia_dp_tb<3,6,0>();
		ia_dp_tb<3,4,-2>();
		ia_dp_tb<3,4,-1>();
		ia_dp_tb<3,4,1>();
		ia_dp_tb<3,4,2>();
		ia_dp_tb<3,4,3>();
		ia_dp_tb<3,4,4>();
		ia_dp_tb<3,7,0>();
		ia_dp_tb<3,8,0>();
		ia_dp_tb<3,9,0>();
		ia_dp_tb<3,10,0>();
		ia_dp_tb<3,11,0>();
		ia_dp_tb<3,4,-6>();
		ia_dp_tb<3,4,-4>();
		ia_dp_tb<3,4,6>();
		ia_dp_tb<3,4,7>();
		ia_dp_tb<3,4,9>();

		ia_dp_tb<4,2,0>();
		ia_dp_tb<4,3,0>();
		ia_dp_tb<4,4,0>();
		ia_dp_tb<4,5,0>();
		ia_dp_tb<4,6,0>();
		ia_dp_tb<4,4,-2>();
		ia_dp_tb<4,4,-1>();
		ia_dp_tb<4,4,1>();
		ia_dp_tb<4,4,2>();
		ia_dp_tb<4,4,3>();
		ia_dp_tb<4,4,4>();
		ia_dp_tb<4,7,0>();
		ia_dp_tb<4,8,0>();
		ia_dp_tb<4,9,0>();
		ia_dp_tb<4,10,0>();
		ia_dp_tb<4,11,0>();
		ia_dp_tb<4,4,-6>();
		ia_dp_tb<4,4,-4>();
		ia_dp_tb<4,4,6>();
		ia_dp_tb<4,4,7>();
		ia_dp_tb<4,4,9>();


    }catch(int e){
        std::cout << "Scalar Operator TB Failed.\n";
    }

    return 0;
}

