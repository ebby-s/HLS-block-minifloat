# Table of Contents

1. [Overview](#overview)
2. [Usage](#usage)
    *  [Primitives](#overview)
        *  [MiniFloat](#overview)
        *  [IntAcc](#overview)
    *  [Blocks](#overview)
        *  [BlockMF](#overview)
        *  [BlockFP](#overview)
    *  [Rounding Methods](#overview)
    *  [Helper Functions/Macros](#overview)
    *  [Testbenches](#overview)
3. [Examples](#examples)
    *  [MiniFloat 2x1 Dot Product](#overview)
    *  [BMF Matrix Multiplier](#overview)
    *  [BFP Matrix Multiplier](#overview)
4. [Contributors](#contributors)
5. [License](#license)

<a name="overview"></a>
# 1 Overview

This library contains HLS implementations of arithmetic units to perform matrix operations using block minifloat, logarithmic block minifloat and block floating point formats. Tested in Vivado HLS.

Refer to the report under `docs/HLS_for_FPGA_based_BMF.pdf` for more details.

Refer to [this paper][BMF_link] for information about the BMF format.

[BMF_link]: https://openreview.net/pdf?id=6zaTwpNSsQ2

<a name="usage"></a>
# 2 Usage

## 2.1 Primitives

### 2.1.1 MiniFloat

The `MiniFloat` type represents a single minifloat value, with sign, exponent and optional mantissa. The following declaration shows the members of this type. Parameters `E` and `M` represent the widths of the exponent and mantissa and the valid ranges of these parameters are shown below. If `M` is set to 0, this type represents a value using the logarithmic minifloat type.

* 2 ≤ `E` ≤ 7

* 0 ≤ `M` ≤ 5


```cpp
template <int E, int M> struct MiniFloat{

    ap_uint<1+E+M> data;

    IntAcc<WfromEM(E,M), FfromEM(E,M)> operator *(const MiniFloat<E,M> &op);

    operator  float() const;

    operator double() const;
}
```

**Multiplication**

A pair of `MiniFloat` type variables, with the same parameters (`E`, `M`) can be multiplied to produce an output of type `IntAcc`, the parameters of this output type (`W`, `F`) depend on the parameters of the original `MiniFloat` variables and can be cauculated using the macros `WfromEM(E, M)` and `FfromEM(E, M)`. This multiplication is error-free.

**Conversions to C++ types**

Any `MiniFloat` variable can be converted to a built-in `float` or `double` in C++. This conversion will assume that the floating-point bias is 0. If rounding is performed, lower bits will be truncated.



### 2.1.2 IntAcc

The `IntAcc` type represents an integer value in 2's complement encoding. The parameter `W` represents the bit width of the representation, and `F` represents the number of fraction bits in the representation. The parameter `F` can be negative, this would mean that the lowest bit no longer represents $2^0$.

```cpp
template <int W, int F> struct IntAcc{

    ap_int<W> acc;

    IntAcc<2*W,F> operator *(const IntAcc<W,F> &op);

    template<int W2>
    inline IntAcc<MAX(W,W2)+1,F> operator +(const IntAcc<W2,F> &op);

    operator  float() const;

    operator double() const;
}
```

**Multiplication**

A pair of `IntAcc` type variables, with the same parameters (`W`, `F`) can be multiplied to produce an output of the same type with parameters (`2*W`, `F`). This multiplication is error-free.

**Addition**

A pair of `IntAcc` type variables (with any widths, but same `F` parameter) can be added. The output width is calculated by taking the larger of the 2 widths and incrementing it. This is done using the `MAX(A, B)` macro. The order of additions (tree adder vs. linear reduction) does not matter as Vivado HLS optimizes the order.

**Conversions to C++ types**

Any `IntAcc` variable can be converted to a built-in float or double in C++. If rounding is performed, lower bits will be truncated.


## 2.2 Blocks

### 2.2.1 BlockMF

This type represents a single block represented in the block minifloat format. This class has parameters `N`, `E` and `M`, which represent block size, exponent size and mantissa size respectively.

```cpp
template<int N, int E, int M> struct BlockMF{

    MiniFloat<E,M> data [N][N];
    ap_int<8>      bias;

    BlockFP<N,WfromEM(E,M)+CLOG2(N),FfromEM(E,M)> operator *(BlockMF<N,E,M> &op);

    template<int W, int F>
    operator BlockFP<N,W,F>() const;
}
```

**Matrix Multiplication**

Matrix multiplication can be performed on a pair of blocks represented as `BlockMF`. The output will be of type `BlockFP`, this can be added to other blocks in `BlockFP`, or can be converted back to `BlockMF`.

**Conversion**

A converter is provided for converting `BlockMF` to `BlockFP`, this can be used to add a pair of `BlockMF` variables by converting to `BlockFP`, then converting back after the operation.

### 2.2.2 BlockFP

```cpp
template<int N, int W, int F> struct BlockFP{

    IntAcc<W,F>      data [N][N];
    ap_int<8>        bias;
    Rounding<N,W,F> *rnd_method;

    BlockAcc(){
        rnd_method = new ToZero<N,W,F>();
    }

    void setRounding(Rounding<N,W,F> *rnd);

    BlockFP<N,W,F> operator *(BlockFP<N,W,F> &op);

    BlockFP<N,W,F> operator +(BlockFP<N,W,F> &op);

    template<int E, int M>
    operator BlockMF<N,E,M>() const;
};
```


## 2.3 Rounding Methods




## 2.4 Helper Functions/Macros

**CLOG2(N)**



**MAX(A,B)**

**WfromEM(E,M) & FfromEM(E,M)**

## 2.5 Testbenches

<a name="examples"></a>
# 3 Examples

## 3.1 2x1 Dot Product

## 3.2 BMF Matrix Multiplier

## 3.3 BFP Matrix Multiplier

<a name="contributors"></a>
# 4 Contributors

Ebby Samson: ebby.samson19@imperial.ac.uk

Credits to authors of [this paper][BMF_link], especially Philip Boland ([philip.leong@sydney.edu.au][Philip_email]).

Final Year Project Supervisor: George Constantinides ([g.constantinides@imperial.ac.uk][George_email]).


[Philip_email]: philip.leong@sydney.edu.au
[George_email]: g.constantinides@imperial.ac.uk

<a name="license"></a>
# 5 License


