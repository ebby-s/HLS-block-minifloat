#ifndef BLOCK
#define BLOCK

#include "../MiniFloat/MiniFloat.hpp"
#include "Rounding.hpp"

template<int N, int E, int M> struct BlockMF;  // Represents a NxN block of MiniFloats.
template<int N, int W, int F> struct BlockFP;  // Represents a NxN block of signed integers.


template<int N, int E, int M> struct BlockMF{

    MiniFloat<E,M> data [N][N];
    ap_int<8>      bias;

    BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> operator *(BlockMF<N,E,M> &op);  // Multiply a pair of BMF blocks.

    operator BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2>() const; // Convert a BMF block to BFP.
};


template<int N, int W, int F> struct BlockFP{

    IntAcc<W,F>      data [N][N];
    ap_int<8>        bias;
    // Rounding<N,W,F> *rnd_method;

    BlockFP(){
        // rnd_method = new ToNegInf<N,W,F>();

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                data[i][j].acc = 0;
            }
        }
        bias = 0;
    }

    // void setRounding(Rounding<N,W,F> &rnd){
    //     *rnd_method = rnd;
    // }

    BlockFP<N,2*W+CLOG2(N),2*F> operator *(BlockFP<N,W,F> &op);  // Multiply a pair of BFP blocks.

    BlockFP<N,W+3+1,F+3> operator +(BlockFP<N,W,F> &op);  // Add a pair of matrix products with loss of precision, take bias into account.

    template<int E, int M>
    operator BlockMF<N,E,M>() const;        // Normalize a block of accumulators and convert to BMF.

    template<int Wo, int Fo>
    operator BlockFP<N,Wo,Fo>() const;      // Round down a BFP block and change frac bits.
};


// Include implementations of block multiplication and normalization.
#include "BlockMul.hpp"
#include "BlockConv.hpp"
#include "BlockAdd.hpp"

#endif
