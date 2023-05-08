#include <iostream>
#include <cassert>

#include "../MiniFloat.hpp"
#include "../DotPrd2/DotPrd2.hpp"

#include "MFMulTB.hpp"
#include "IAMulTB.hpp"
#include "IAAddTB.hpp"
#include "DP2TB.hpp"


int main(){
    try{

		ia_mul_tb<2,0>();
		ia_mul_tb<3,0>();
		ia_mul_tb<4,0>();
		ia_mul_tb<5,0>();
		ia_mul_tb<6,0>();
		ia_mul_tb<4,-2>();
		ia_mul_tb<4,-1>();
		ia_mul_tb<4,1>();
		ia_mul_tb<4,2>();
		ia_mul_tb<4,3>();
		ia_mul_tb<4,4>();

		ia_mul_tb_rand<7,0>();
		ia_mul_tb_rand<8,0>();
		ia_mul_tb_rand<9,0>();
		ia_mul_tb_rand<10,0>();
		ia_mul_tb_rand<11,0>();
		ia_mul_tb_rand<4,-6>();
		ia_mul_tb_rand<4,-4>();
		ia_mul_tb_rand<4,6>();
		ia_mul_tb_rand<4,7>();
		ia_mul_tb_rand<4,9>();


    }catch(int e){
        std::cout << "Scalar Operator TB Failed.\n";
    }

    return 0;
}

