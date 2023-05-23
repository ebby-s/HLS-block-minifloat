#include "DotPrd2.hpp"


IntAcc<Wp+Cp,Fp> DotPrd2(
    inType op0[Ni],
    inType op1[Ni]
){
    #ifdef DP_MiniFloat
        #ifdef DP_ProcElem
            return MFProcElem<Ni,Ei,Mi>(op0,op1);
        #else
            return MFDotPrd<Ni,Ei,Mi>(op0,op1);
        #endif
    #else
        #ifdef DP_ProcElem
            return IAProcElem<Ni,Wi,Fi>(op0,op1);
        #else
            return IADotPrd<Ni,Wi,Fi>(op0,op1);
        #endif
    #endif
}


