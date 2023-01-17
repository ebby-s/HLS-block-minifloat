#ifndef BLOCK
#define BLOCK

#include "../ScalarOperations/ScalarOperations.hpp"



template<int N, int E, int M> class BlockMat;  // Represents a NxN block of MiniFloats.
template<int N, int W, int F> class BlockAcc;  // Represents a NxN block of accumulators.


template<int N, int E, int M> class BlockMat{

    public:
    MiniFloat<E,M> [N][N] data;
    ap_int<8>             bias;

    public:
    template <int W, int F>
    BlockAcc<N,KulischAcc<W,F>> operator *(BlockMat<N,E,M> &op);  // Multiply a pair of blocks.
};


template<int N, int W, int F> class BlockAcc{

    public:
    KulischAcc<W,F> [N][N] data;
    ap_int<8>              bias;

    public:
    BlockAcc<N,W,F> operator +(BlockAcc<N,W,F> &op);  // Add a pair of matrix products with loss of precision, take bias into account.

    template<int E, int M>
    operator BlockMat<N,E,M>() const;        // Normalize a block of accumulators and convert to BMF.
};



#endif
