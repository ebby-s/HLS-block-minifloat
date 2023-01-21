#include "DotPrd2.hpp"



KulischAcc<Nt+1,Ft> DotPrd2(
    MiniFloat<Et,Mt> op0,
    MiniFloat<Et,Mt> op1,
    MiniFloat<Et,Mt> op2,
    MiniFloat<Et,Mt> op3
){

    KulischAcc<Nt,Ft> prd0;
    KulischAcc<Nt,Ft> prd1;

    // #pragma HLS data_pack variable=op0
    // #pragma HLS data_pack variable=op1
    // #pragma HLS data_pack variable=op2
    // #pragma HLS data_pack variable=op3

    prd0 = op0 * op1;
    prd1 = op2 * op3;

    return prd0 + prd1;

}
