#ifndef NORM
#define NORM

#include "BlockMat.hpp"


// Normalize a block of accumulators and convert to BMF.
template<int N, int W, int F, int E, int M>
BlockAcc<N,W,F>::operator BlockMat<N,E,M>() const{}



#endif
