#ifndef ROUND
#define ROUND


template <int W_i, int W_o> class Rounding;
template <int W_i, int W_o> class ToNearestEven : private Rounding;
template <int W_i, int W_o> class ToZero        : private Rounding;
template <int W_i, int W_o> class ToNegInf      : private Rounding;
template <int W_i, int W_o> class ToPosInf      : private Rounding;
template <int W_i, int W_o> class Stochastic    : private Rounding;


template <int W_i, int W_o> class Rounding{
    public:
    virtual ap_uint<W_o> round(ap_uint<W_i> op);
};

template <int W_i, int W_o> class ToNearestEven : private Rounding{
    public:
    ap_uint<W_o> round(ap_uint<W_i> op);
};

template <int W_i, int W_o> class ToZero : private Rounding{
    public:
    ap_uint<W_o> round(ap_uint<W_i> op){
        return op >> (W_i - W_o);
    }
};

template <int W_i, int W_o> class ToNegInf : private Rounding{
    public:
    ap_uint<W_o> round(ap_uint<W_i> op);
};

template <int W_i, int W_o> class ToPosInf : private Rounding{
    public:
    ap_uint<W_o> round(ap_uint<W_i> op);
};

template <int W_i, int W_o> class Stochastic : private Rounding{
    public:
    ap_uint<W_o> round(ap_uint<W_i> op);
};

#endif
