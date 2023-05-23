#ifndef DP2
#define DP2

#include "../MiniFloat.hpp"

// #define DP_MiniFloat   // Remove this to use IntAcc instead of MiniFloat for inputs.

// #define DP_ProcElem    // Remove for O(N) area, O(1) time, keep for O(1) area, O(N) time.

#define Ni 4            // Width of input vectors.

#ifdef DP_MiniFloat
    #define Ei 3       // Parameters of input and product types.
    #define Mi 4
    #define Wp WPRD(Ei,Mi)
    #define Fp FPRD(Ei,Mi)

    typedef MiniFloat<Ei,Mi> inType;
#else
    #define Wi 4       // Parameters of input and product types.
    #define Fi 0
    #define Wp (2*Wi)
    #define Fp (2*Fi)

    typedef IntAcc<Wi,Fi> inType;
#endif

#define Cp CLOG2(Ni)    // Widen output accumulator depending on vector size.


IntAcc<Wp+Cp,Fp> DotPrd2(
    inType op0[Ni],
    inType op1[Ni]
);


#endif
