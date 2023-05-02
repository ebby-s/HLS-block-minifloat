#ifndef ROUND
#define ROUND


// struct prng{
//     ap_uint<32> lfsr;

//     ap_uint<32> rand(){
//         bool b_32 = lfsr.get_bit(32-32);
//         bool b_22 = lfsr.get_bit(32-22);
//         bool b_2 = lfsr.get_bit(32-2);
//         bool b_1 = lfsr.get_bit(32-1);
//         bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;

//         lfsr >>= 1;
//         lfsr.set_bit(31, new_bit);

//         return lfsr;
//     }
// };


template<int N, int W, int F> class Rounding{
    public:
    virtual ap_int<W+1> rnd_mul(ap_int<2*W> op){};
};


template<int N, int W, int F> class ToZero : public Rounding<N,W,F>{
    public:
    ap_int<W+1> rnd_mul(ap_int<2*W> op){
        return op >> W;
    }
};

// template<int W_i, int W_o> class ToNegInf : public Rounding<W_i,W_o>{
//     public:
//     ap_int<W_o+1> round(ap_int<W_i> op){
//         return (op >> (W_i - W_o)) + ((op < 0) ? 1 : 0);
//     }
// };

// template<int W_i, int W_o> class ToPosInf : public Rounding<W_i,W_o>{
//     public:
//     ap_int<W_o+1> round(ap_int<W_i> op){
//         return (op >> (W_i - W_o)) + ((op > 0) ? 1 : 0);
//     }
// };

// template<int W_i, int W_o> class ToNearestEven : public Rounding<W_i,W_o>{
//     public:
//     ap_int<W_o+1> round(ap_int<W_i> op){
//         return 0;
//     }
// };

// template<int W_i, int W_o> class Stochastic : public Rounding<W_i,W_o>{
//     public:
//     ap_int<W_o+1> round(ap_int<W_i> op){
//         return 0;
//     }
// };

#endif
