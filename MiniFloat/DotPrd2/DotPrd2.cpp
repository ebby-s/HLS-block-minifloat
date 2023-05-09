#include "DotPrd2.hpp"


IntAcc<Wp+Cp,Fp> DotPrd2(
    MiniFloat<Ei,Mi> op0[Ni],
    MiniFloat<Ei,Mi> op1[Ni]
){
    return MFProcElem<Ni,Ei,Mi>(op0,op1);
}


// IntAcc<Wp+Cp,Fp> DotPrd2(
//     IntAcc<Wi,Fi> op0[Ni],
//     IntAcc<Wi,Fi> op1[Ni]
// ){
//     return IAProcElem<Ni,Wi,Fi>(op0,op1);
// }
