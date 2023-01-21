#ifndef DP
#define DP

#include "MiniFloat.hpp"

#define Et 2
#define Mt 0
#define Nt ((1<<Et)+Mt)*2
#define Ft 2*(Mt-1)

KulischAcc<Nt+1,Ft> DotPrd2(
    MiniFloat<Et,Mt> op0,
    MiniFloat<Et,Mt> op1,
    MiniFloat<Et,Mt> op2,
    MiniFloat<Et,Mt> op3
);

#endif
