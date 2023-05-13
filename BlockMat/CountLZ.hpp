
#ifndef CUSTOM_CLZ
#define CUSTOM_CLZ

#include "ap_int.h"

template <int W>
ap_uint<CLOG2(W+1)> CtLZ(ap_uint<W> data){

    if(W <= 32){
        if(data == 0){
            return W;
        }else{
            return __builtin_clz(data) - (32-W);
        }
    }else if(W <= 64){
        if(data == 0){
            return W;
        }else{
            return __builtin_clzl(data) - (64-W);
        }
    }else{
        if(data == 0){
            return W;
        }else if(data(W-1, 64) == 0){
            return __builtin_clzl(data(63, 0)) + (W-64);
        }else{
            return __builtin_clzl(data(W-1, 64)) - (128-W);
        }
    }

    // ap_uint<CLOG2(W+1)> out;
    // clz_loop:
    // for(int k=CLOG2(W+1)-1; k>=0; k--){
    //     #pragma HLS UNROLL
    //     if(data(W-1, W-(1<<k)) == 0){
    //         out[k] = 1;
    //         data <<= (1<<k);
    //     }else{
    //         out[k] = 0;
    //     }
    // }
    // return out;
}


#endif
