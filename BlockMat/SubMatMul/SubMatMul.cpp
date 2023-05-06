#include "SubMatMul.hpp"


BlockMF<Ni,Eo,Mo> SubMatMul(
    BlockMF<Ni,Ei,Mi> (&op0)[At],
    BlockMF<Ni,Ei,Mi> (&op1)[At]
){

    BlockFP<Ni,Wp+Cp,Fp> prd[At];
    BlockFP<Ni,Ws,Fs> sum;


    for(int i=0; i<At; i++){
        prd[i] = op0[i] * op1[i];
    }

    for(int i=0; i<At; i++){
        sum = prd[i];
    }

    return BlockMF<Ni,Eo,Mo>(sum);
}

