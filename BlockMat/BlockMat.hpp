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

    BlockMF<N,E,M>(){}

    BlockMF<N,E,M>(const BlockMF<N,E,M> &op){

        bias = op.bias;

        bmf_copy_r_loop:
        for(int i=0; i<N; i++){
            #pragma HLS UNROLL
            bmf_copy_c_loop:
            for(int j=0; j<N; j++){
                #pragma HLS UNROLL

                data[i][j] = op.data[i][j];
            }
        }
    }

    BlockMF<N,E,M> &operator=(const BlockMF<N,E,M>& rhs){

        if(&rhs == this)
            return *this;

        bias = rhs.bias;

        bmf_assign_r_loop:
        for(int i=0; i<N; i++){
            #pragma HLS UNROLL
            bmf_assign_c_loop:
            for(int j=0; j<N; j++){
                #pragma HLS UNROLL

                data[i][j] = rhs.data[i][j];
            }
        }

        return *this; 
    }

};


template<int N, int W, int F> struct BlockFP{

    IntAcc<W,F>      data [N][N];
    ap_int<8>        bias;

    BlockFP<N,2*W+CLOG2(N),2*F> operator *(BlockFP<N,W,F> &op);  // Multiply a pair of BFP blocks.

    BlockFP<N,W+3+1,F+3> operator +(BlockFP<N,W,F> &op);  // Add a pair of matrix products with loss of precision, take bias into account.

    void normalise();  // Normalise block.

    template<int E, int M>
    operator BlockMF<N,E,M>() const;        // Normalize a block of accumulators and convert to BMF.

    template<int Wo, int Fo>
    operator BlockFP<N,Wo,Fo>() const;      // Round down a BFP block and change frac bits.

    BlockFP<N,W,F>(){}

    BlockFP<N,W,F>(const BlockFP<N,W,F> &op){

        bias = op.bias;

        bfp_copy_r_loop:
        for(int i=0; i<N; i++){
            #pragma HLS UNROLL
            bfp_copy_c_loop:
            for(int j=0; j<N; j++){
                #pragma HLS UNROLL

                data[i][j] = op.data[i][j];
            }
        }
    }

    BlockFP<N,W,F> &operator=(const BlockFP<N,W,F>& rhs){

        if(&rhs == this)
            return *this;

        bias = rhs.bias;

        bfp_assign_r_loop:
        for(int i=0; i<N; i++){
            #pragma HLS UNROLL
            bfp_assign_c_loop:
            for(int j=0; j<N; j++){
                #pragma HLS UNROLL

                data[i][j] = rhs.data[i][j];
            }
        }

        return *this; 
    }
};


// Include implementations of block multiplication and normalization.
#include "BlockMul.hpp"
#include "BlockAdd.hpp"
#include "BlockConv.hpp"
#include "Normalise.hpp"

#endif
