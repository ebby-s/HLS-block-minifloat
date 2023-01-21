#include "MiniFloat.hpp"


#define E 3
#define M 4
#define N ((1<<E)+M)*2
#define F 2*(M-1)


void DotPrd2(MiniFloat<E,M> op0[2], MiniFloat<E,M> op1[2], KulischAcc<N+1,F> *sum){

    KulischAcc<N,F> prd0, prd1;
    #pragma HLS aggregate variable=prd0
  
    prd0 = op0[0] * op0[1];
    prd1 = op1[0] * op1[1];

    *sum = prd0 + prd1;

}