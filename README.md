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

This library contains HLS implementations of arithmetic units to perform matrix operations using block minifloat, logarithmic block minifloat and block floating point formats. Tested in Vivado HLS. The `ap_int` library is required.

Refer to the report under `docs/HLS_for_FPGA_based_BMF.pdf` for more details about the implementation.

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

    IntAcc<WPRD(E,M), FPRD(E,M)> operator *(const MiniFloat<E,M> &op);

    operator  float() const;

    operator double() const;
}
```

**Multiplication**

A pair of `MiniFloat` type variables, with the same parameters (`E`, `M`) can be multiplied to produce an output of type `IntAcc`, the parameters of this output type (`W`, `F`) depend on the parameters of the original `MiniFloat` variables and can be cauculated using the macros `WPRD(E, M)` and `FPRD(E, M)`. This multiplication is error-free.

**Conversions to C++ types**

Any `MiniFloat` variable can be converted to a built-in `float` or `double` in C++. This conversion will assume that the floating-point bias is 0. If rounding is performed, lower bits will be truncated.



### 2.1.2 IntAcc

The `IntAcc` type represents an integer value in 2's complement encoding. The parameter `W` represents the bit width of the representation, and `F` represents the number of fraction bits in the representation. The parameter `F` can be negative, this would mean that the lowest bit no longer represents $2^0$.

```cpp
template <int W, int F> struct IntAcc{

    ap_int<W> acc;

    IntAcc<2*W,2*F> operator *(const IntAcc<W,F> &op);

    template<int W2>
    inline IntAcc<MAX(W,W2)+1,F> operator +(const IntAcc<W2,F> &op);

    template<int Wo>
    inline operator IntAcc<Wo,F>() const;

    operator  float() const;

    operator double() const;

    operator long double() const;
}
```

**Multiplication**

A pair of `IntAcc` type variables, with the same parameters (`W`, `F`) can be multiplied to produce an output of the same type with parameters (`2*W`, `2*F`). This multiplication is error-free.

**Addition**

A pair of `IntAcc` type variables (with any widths, but same `F` parameter) can be added. The output width is calculated by taking the larger of the 2 widths and incrementing it. This is done using the `MAX(A,B)` macro. The order of additions (tree adder vs. linear reduction) does not matter as Vivado HLS optimizes the order.

**Change width**
The width of the representation can be changed. If the new width is smaller, higher bits are removed.

**Conversions to C++ types**

Any `IntAcc` variable can be converted to a built-in float or double in C++. If rounding is performed, the default C++ rounding mode is used for converting int to float.


## 2.2 Blocks

### 2.2.1 BlockMF

This type represents a single block represented in the block minifloat format. This class has parameters `N`, `E` and `M`, which represent block size, exponent size and mantissa size respectively.

```cpp
template<int N, int E, int M> struct BlockMF{

    MiniFloat<E,M> data [N][N];
    ap_int<8>      bias;

    BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> operator *(BlockMF<N,E,M> &op);

    operator BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2>() const;
}
```

**Matrix Multiplication**

Matrix multiplication can be performed on a pair of blocks represented as `BlockMF`. The output will be of type `BlockFP`, this can be added to other blocks in `BlockFP`, or can be converted back to `BlockMF`. This operation is error free.

**Conversion**

A converter is provided for converting `BlockMF` to `BlockFP`, this can be used to add a pair of `BlockMF` variables by converting to `BlockFP`, then converting back after the operation. This operation is error free.

### 2.2.2 BlockFP

Represents the BFP data type. A block of signed integers with a shared signed 8-bit exponent. Some operations could incur loss in precision, such as addition or conversions, the `rnd_method` object is used to perform rounding in these cases. The rounding method is 'towards negative infinity' by default, but can be changed using `setRounding(Rounding<N,W,F> *rnd)`.

```cpp
template<int N, int W, int F> struct BlockFP{

    IntAcc<W,F>      data [N][N];
    ap_int<8>        bias;
    Rounding<N,W,F> *rnd_method;

    BlockAcc(){
        rnd_method = new ToNegInf<N,W,F>();
    }

    void setRounding(Rounding<N,W,F> *rnd);

    BlockFP<N,2*W+CLOG2(N),2*F> operator *(BlockFP<N,W,F> &op);

    BlockFP<N,W,F> operator +(BlockFP<N,W,F> &op);

    template<int E, int M>
    operator BlockMF<N,E,M>() const;

    template<int Wo, int Fo>
    operator BlockFP<N,Wo,Fo>() const;
};
```

**Multiplication**

A pair of BFP blocks can be multiplied using the overloaded multiplication operator. This operation is error free and the output type is larger than the input type. This can be rounded down to the original type using the BFP to BFP conversion if desired.

**Addition**

Adds BMF blocks together, uses the rounding method associated with this object. Allows the results of multiple block multiplications to be accumulated.

**Conversions**

A BFP block can be converted to any valid BMF format or another BFP format with different parameters `Wo` and `Fo`. These operations could require rounging, in which case the rounding method associated with this object is used.


## 2.3 Rounding Methods

Some methods of the `BlockFP` class require rounding. Each `BlockFP` object has a `Rounding` object asociated with it, which defines how rounding is performed for these methods. Some rounding modes are provided in `BlockMat/Rounding.hpp`.

```cpp
template<int N, int W, int F> class Rounding{
    public:
    virtual ap_uint<W> rnd_bmf(ap_uint<W> op, int Wo){};
    virtual ap_int<W> rnd_bfp(ap_int<W> op, int Wo){};
};
```




## 2.4 Helper Functions/Macros

**CLOG2(N)**

A macro to calculate `ceil(log2(n))`, works for values in the range $1 \leq n < 2^{16}$.

**MAX(A,B)**

Macro to find the maximum of two inputs. Used for parameters.

**WPRD(E,M) & FPRD(E,M)**

Given the exponent width `E` and mantissa width `M` of a `MiniFloat` format, calculates the parameters `W` and `F` of an `IntAcc` object which can represent all values of the __product__ of two `MiniFloat`s of the input format.

## 2.5 Testbenches

Testbenches for these blocks can be found in their respective folders. Eg. `MiniFloat/MiniFloatTB`. A python file is used to generate the C++ testbench file. Specific formats can be added to the configuration lists in the python file in order to test them.



<a name="examples"></a>
# 3 Examples

## 3.1 2x1 Dot Product

This example can be found in `MiniFloat/DotPrd2`. It is a circuit that computes the dot product of a pair of 2x1 vectors. The parameters in `DotPrd2.hpp` can be tweaked to change the length of the vector as well as the format used.

## 3.2 Block Matrix Multiplier

A Block Matrix Multiplier performs matrix multiplication with a pair of blocks of `BMF` or `BFP` values. This example can be found in `BlockMat/SubMatMul`. The parameters in `SubMstMul.hpp` can be tweaked to change the parameters of the number representation and the format itself (`BMF`/`BFP`).

<a name="contributors"></a>
# 4 Contributors

Ebby Samson: ebby.samson19@imperial.ac.uk

Credits to authors of [this paper][BMF_link], especially Philip Boland ([philip.leong@sydney.edu.au][Philip_email]).

Final Year Project Supervisor: George Constantinides ([g.constantinides@imperial.ac.uk][George_email]).


[Philip_email]: philip.leong@sydney.edu.au
[George_email]: g.constantinides@imperial.ac.uk

<a name="license"></a>
# 5 License

This project is under the MIT License, see `LICENSE` for details.

