#ifndef DP
#define DP

#include "MiniFloat.hpp"

#define Et 2
#define Mt 0
#define Wt WPRD(Et,Mt)
#define Ft FPRD(Et,Mt)

IntAcc<Wt+CLOG2(2),Ft> DotPrd2(
    MiniFloat<Et,Mt> op0,
    MiniFloat<Et,Mt> op1,
    MiniFloat<Et,Mt> op2,
    MiniFloat<Et,Mt> op3
);

#endif
