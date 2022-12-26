#ifndef MINIFLOAT
#define MINIFLOAT

#include "ap_int.h"    // Use arbitrary precision types.

// MiniFloat field widths.
#define E 2             // Exponent width for minifloat representation. 2 <= E <= 8
#define M 2             // Mantissa width for minifloat representation. 0 <= M <= 6

#define LOG_FRMT (M == 0)   // If M is set to 0, use logarithmic BMF format.

// Size of error-free multiplier output.
#define E_PRD  E + 1
#define M_PRD (M + 1) * 2

// Size of first K. Accumulator.
// w.l.g assume bias = 0.
// If converted to fixed point without error,
// LSB has value 2^-(M-1), MSB has value 2^(2^E -1).
// Size of fi representation is (2^E -1) + (M-1) + 1 bits.
// Eg. M=5, E=3.
// Only MSB is set when: exponent = 7, mantissa = 0.
// Value is: 2^7
// Only LSB is set when: exponent = 0, mantissa = 1.
// Value is: 2^-4
// Fi representation uses 12 bits.
// 2^3 - 1 + 5-1 + 1 = 12.
// Add a bit for 2's compl.
// w_fi = 2^E-1 + M-1 + 1 + 1
// w_a = Fi_W*2 + 1
#define W_PRD ((1<<E) + M)*2 + 1
#define W_A   ((1<<E) + M)*2 + 1


typedef ap_uint<1>         sgn_t;
typedef ap_uint<E>         exp_t;
typedef ap_uint<M>         man_t;
typedef ap_uint<E_PRD> prd_exp_t;
typedef ap_uint<M_PRD> prd_man_t;


// MiniFloat.
struct minifloat_t{
    sgn_t sgn;
    exp_t exp;
    man_t man;
};

// Error-free multiplier output.
struct mul_out_t{
    sgn_t     sgn;
    prd_exp_t exp;
    prd_man_t man;
};


// Conversion to float type, used only for verification.
// All assume bias is 0.
float mnf_to_f32(minifloat_t op);

float mul_to_f32(mul_out_t mul_out);

template <int N>
float fip_to_f32(ap_int<N> op){

    float out;

    out = op;

    out /= pow(2, M);

    return out;
}

// Overload operator to allow sets of mul_out_t.
bool operator <(const mul_out_t &op0, const mul_out_t &op1);


#endif
