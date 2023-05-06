#ifndef SUB_MUL
#define SUB_MUL

#include "../BlockMat.hpp"

#define At 1

#define Ni 1
#define Ei 2
#define Mi 0

#define Eo 2
#define Mo 0

#define Wp WPRD(Ei,Mi)
#define Fp FPRD(Ei,Mi)
#define Cp CLOG2(Ni)

#define Ws Wp+Cp+3+1
#define Fs Fp+3

// BlockFP<Ni,Wp+Cp,Fp> SubMatMul(
//     BlockMF<Ni,Ei,Mi> (&op0)[At],
//     BlockMF<Ni,Ei,Mi> (&op1)[At]
// );

BlockMF<Ni,Eo,Mo> SubMatMul(
    BlockMF<Ni,Ei,Mi> (&op0)[At],
    BlockMF<Ni,Ei,Mi> (&op1)[At]
);


#endif