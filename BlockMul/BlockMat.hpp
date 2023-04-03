#ifndef BLOCK
#define BLOCK

#include "../MiniFloat/MiniFloat.hpp"

template<int N, int E, int M> struct BlockMat;  // Represents a NxN block of MiniFloats.
template<int N, int W, int F> struct BlockAcc;  // Represents a NxN block of accumulators.


template<int N, int E, int M> struct BlockMat{

    MiniFloat<E,M> [N][N] data;
    ap_int<8>             bias;

    BlockAcc<N,WfromEM(E,M)+CLOG2(N),FfromEM(E,M)> operator *(BlockMat<N,E,M> &op);  // Multiply a pair of blocks.
};


template<int N, int W, int F> struct BlockAcc{

    KulischAcc<W,F> [N][N] data;
    ap_int<8>              bias;

    BlockAcc<N,W,F> operator +(BlockAcc<N,W,F> &op);  // Add a pair of matrix products with loss of precision, take bias into account.

    template<int E, int M>
    operator BlockMat<N,E,M>() const;        // Normalize a block of accumulators and convert to BMF.
};


// Include implementations of block multiplication and normalization.
#include "DotProduct.hpp"
#include "BlockAdd.hpp"
#include "Normalize.hpp"

#endif
