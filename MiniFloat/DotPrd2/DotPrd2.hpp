#ifndef DP2
#define DP2

#include "../MiniFloat.hpp"

#define Ni 32
#define Ei 3
#define Mi 4

#define Wi 4
#define Fi 0

#define Wp WPRD(Ei,Mi)
#define Fp FPRD(Ei,Mi)
#define Cp CLOG2(Ni)
// #define Wp (2*Wi)
// #define Fp (2*Fi)


IntAcc<Wp+Cp,Fp> DotPrd2(
    MiniFloat<Ei,Mi> op0[Ni],
    MiniFloat<Ei,Mi> op1[Ni]
);


// IntAcc<Wp+Cp,Fp> DotPrd2(
//     IntAcc<Wi,Fi> op0[Ni],
//     IntAcc<Wi,Fi> op1[Ni]
// );

#endif
