#ifndef BLOCK
#define BLOCK

#include "../MiniFloat/MiniFloat.hpp"
#include "Rounding.hpp"

template<int N, int E, int M> struct BlockMF;  // Represents a NxN block of MiniFloats.
template<int N, int W, int F> struct BlockFP;  // Represents a NxN block of accumulators.


template<int N, int E, int M> struct BlockMF{

    MiniFloat<E,M> data [N][N];
    ap_int<8>      bias;

    BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> operator *(BlockMF<N,E,M> &op);  // Multiply a pair of blocks.

    // Convert a BMF block to BFP.
    template<int W, int F>
    operator BlockFP<N,W,F>() const;
};


template<int N, int W, int F> struct BlockFP{

    IntAcc<W,F>      data [N][N];
    ap_int<8>        bias;
    Rounding<N,W,F> *rnd_method;

    BlockFP(){
        rnd_method = new ToZero<N,W,F>();
    }

    void setRounding(Rounding<N,W,F> &rnd){
        *rnd_method = rnd;
    }

    BlockFP<N,W,F> operator *(BlockFP<N,W,F> &op);

    BlockFP<N,W,F> operator +(BlockFP<N,W,F> &op);  // Add a pair of matrix products with loss of precision, take bias into account.

    template<int E, int M>
    operator BlockMF<N,E,M>() const;        // Normalize a block of accumulators and convert to BMF.
};


// Include implementations of block multiplication and normalization.
#include "BlockMul.hpp"
//#include "BlockAdd.hpp"
#include "BlockConv.hpp"

#endif
