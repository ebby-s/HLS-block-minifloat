#ifndef SYSTOL_ARR
#define SYSTOL_ARR

#include "BlockMat.hpp"

template <int N, int E, int M, int W, int F>
BlockAcc<N,KulischAcc<W,F>> BlockMat<N,E,M>::operator *(BlockMat<N,E,M> &op){}








#endif
