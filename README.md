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

This HLS library contains implementations of arithmetic units to perform matrix operations using block minifloat (BMF), logarithmic block minifloat and block floating point (BFP) formats. This library was developed and tested using Vivado HLS. The `ap_int` library is required.

Refer to the report under `docs/HLS_for_FPGA_based_BMF.pdf` for more details about the implementation.

Refer to [this paper][BMF_link] for information about the BMF format.

[BMF_link]: https://openreview.net/pdf?id=6zaTwpNSsQ2

<a name="usage"></a>
# 2 Usage

## 2.1 Custom Primitive Types

These primitive types represent individual data elements in BMF or BFP blocks.

### 2.1.1 MiniFloat

The `MiniFloat` type represents a single minifloat number with sign, exponent and optional mantissa. The following declaration shows the members of this type. Parameters `E` and `M` represent the widths of the exponent and mantissa and the valid ranges of these parameters are shown below. If `M` is set to 0, this type represents a value using the logarithmic minifloat type.

* $2 \leq E \leq 6$

* $0 \leq M \leq 15$


```cpp
template <int E, int M> struct MiniFloat{

    ap_uint<1+E+M> data;

    IntAcc<WPRD(E,M), FPRD(E,M)> operator *(const MiniFloat<E,M> &op);

    operator  float() const;

    operator double() const;
}
```

**Multiplication**

A pair of `MiniFloat` type variables with the same parameters (`E`, `M`) can be multiplied to produce an output of type `IntAcc`. The parameters of this output type (`W` and `F`) depend on the parameters of the original `MiniFloat` variables and can be cauculated using the macros `WPRD(E, M)` and `FPRD(E, M)`. This multiplication is error-free.

**Conversions to C++ types**

Any `MiniFloat` variable can be converted to a built-in `float` or `double` in C++. This conversion will assume that the implicit bias of the `MiniFloat` number is 0. If rounding is performed, lower bits will be truncated.



### 2.1.2 IntAcc

The `IntAcc` type represents an integer value in 2's complement encoding. The parameter `W` represents the bit width of the representation, and `F` represents the negative scale factor (Value $= acc*2^{-F}$).

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

A pair of `IntAcc` variables with the same parameters (`W`, `F`) can be multiplied to produce an output of the same type with parameters (`2*W`, `2*F`). This multiplication is error-free.

**Addition**

A pair of `IntAcc` variables (with any widths, but equal `F` parameter) can be added. The output width is calculated by taking the larger of the 2 widths and incrementing it. This is done using the `MAX(A,B)` macro. The order of additions (tree adder vs. linear reduction) does not matter as the HLS tools optimise the order into whichever is ideal.

**Change width**

The width of the representation can be changed. If the new width is smaller, higher bits are removed.

**Conversions to C++ types**

Any `IntAcc` variable can be converted to a built-in `float`, `double` or `long double` in C++. If rounding is performed, lower bits are truncated.


## 2.2 Blocks

These types represent BMF and BFP formats, which contain data in a primitive format along with an 8-bit shared exponent.

### 2.2.1 BlockMF

This type represents a single block represented in the BMF format. This class has parameters `N`, `E` and `M`, which represent block size, exponent size and mantissa size respectively.

```cpp
template<int N, int E, int M> struct BlockMF{

    MiniFloat<E,M> data [N][N];
    ap_int<8>      bias;

    BlockFP<N,WPRD(E,M)+CLOG2(N),FPRD(E,M)> operator *(BlockMF<N,E,M> &op);

    operator BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2>() const;


    BlockMF<N,E,M>(const BlockMF<N,E,M> &op);

    BlockMF<N,E,M> &operator=(const BlockMF<N,E,M>& rhs);
}
```

**Matrix Matrix Multiplication**

Matrix multiplication can be performed between a pair of blocks represented with the same `BlockMF` format with the same parameters. The output will be of type `BlockFP`, this can be added to other blocks in `BlockFP`, or can be converted back to `BlockMF`. This operation is error free. The output width takes the adder tree into account by adding `CLOG(N)` ($\lceil log_2(N) \rceil$) bits to the product width.

**Conversion**

A converter is provided for converting `BlockMF` to `BlockFP`. This is used in the implementation of a BMF adder. The BMF blocks are converted to BFP, added, then converted back to BMF. This BMF to BFP conversion operation is error-free. The parameters of the output `BlockFP` block are calculated using `WPRD(E,M)/2` and `WPRD(E,M)/2`. This is possible because these macros calculate the parameters `W` and `F` to represent the __product__ of `MiniFloat<E,M>` types. Halving these parameters gives the values required to represent a `MiniFloat<E,M>` number.

**Copy and Assignment Constructors**

These constructors are overidden with fully unrolled versions, which copy over the data matrix combinatorially in most cases. If the input block is not fully partitioned, this may take more time.

### 2.2.2 BlockFP

Can be used to represent the BFP data type, as well as intermediate values while processing BMF blocks. A block of signed integers with a signed 8-bit shared exponent. Some operations could incur loss in precision such as addition or conversions, a user specified rounding mode can be used in these cases. This class can be used to represent BMF when the `F` parameter is set to zero.

```cpp
template<int N, int W, int F> struct BlockFP{

    IntAcc<W,F>      data [N][N];
    ap_int<8>        bias;

    BlockFP<N,2*W+CLOG2(N),2*F> operator *(BlockFP<N,W,F> &op);

    BlockFP<N,W+3+1,F+3> operator +(BlockFP<N,W,F> &op);

    void normalise();

    template<int E, int M, rnd_mode_t RMF>
    BlockMF<N,E,M> toBMF();

    template<int Wo, int Fo, rnd_mode_t RFP>
    BlockMF<N,E,M> toBFP();


    BlockFP<N,W,F>(const BlockFP<N,W,F> &op);

    BlockFP<N,W,F> &operator=(const BlockFP<N,W,F>& rhs);
};
```

**Multiplication**

A pair of BFP blocks can be multiplied using the overloaded multiplication operator. This operation is error free and the output type is larger than the input type. This can be rounded down to the original type using the BFP to BFP conversion if desired.

**Addition**

Adds BMF blocks together, uses the rounding method associated with this object. Important in supporting arbitrary matrix multiplication because it allows the results of multiple block multiplications to be accumulated. The output is 4 bits larger than the input, 1 extra at the high end, and 3 extra at the low end. The 3 low end bits are used as `GRS` bits for the `RNE` rounding mode if converted back to original width.

**Conversions**

A BFP block can be converted to any valid BMF format with parameters `E` and `M`, or another BFP format with parameters `Wo` and `Fo`. The block size `N` stays the same in both cases. These operations could require rounding, in which case the rounding mode parameter specified the rounding mode used.


## 2.3 Rounding Methods

The conversions in the `BlockFP` class require rounding because of the loss of precision. The rounding mode is specified as a parameter value to the conversion function. Some rounding modes are provided in the `BlockMat/Rounding.hpp` header file. The parameter specifies a value of type `rnd_mode_t`, the definition of this user-defined type is shown below (from `BlockMat/Rounding.hpp`).

```cpp
enum rnd_mode_t{   // Rounding modes enum.
    RTZ,        // Round nearest towards zero.
    RAZ,        // Round nearest away from zero.
    RNI,        // Round towards negative infinity.
    RPI,        // Round towards positive infinity.
    RNE,        // Round to nearest even.
    STOCHASTIC  // Stochastic Rounding
};
```

 This rounding mode parameter is used by the conversion functions in `BlockMat/BlockConv.hpp`, where a case statement chooses between rounding functions which implement the different rounding modes. The snippet below shows the declaration for the `RTZ` rounding mode. The parameters `N`, `W` and `F` are the parameters of the input to the conversion function, and `Wo` is the desired output width for a particular conversion. It is passed as a function parameter, however, HLS tools recognise it as a constant due to the `INLINE` directive in the conversion functions.

```cpp
template<int N, int W, int F>
ap_int<W> rnd_RTZ(ap_int<W> op, int Wo);
```

In order to add custom rounding modes, there are three steps:

1. Add a name for the custom rounding mode in the `enum` in `Rounding.hpp`.

2. Add the definition for the custom rounding mode in `Rounding.hpp`.

3. Add an extra case in the case structure in `BlockConv.hpp`.

For verification of this rounding mode, the `BlockMatTB/BlockConvTB.hpp` file needs to be modified, and a C++ model of the custom rounding mode needs to be added to `BlockMatTB\helperTB.hpp`.


## 2.4 Helper Functions/Macros

**CLOG2(N)**

A macro to calculate $\lceil log_2(N) \rceil$, works for values in the range $1 \leq n < 2^{16}$.

**MAX(A,B)**

Macro to find the maximum of two inputs in a way that they can be used as constants when calculating parameter values.

**WPRD(E,M) & FPRD(E,M)**

Given the exponent width `E` and mantissa width `M` of a `MiniFloat` format, calculates the parameters `W` and `F` of an `IntAcc` object which can represent all values of the __product__ of two `MiniFloat`s of the input format. Refer to the report in `docs/HLS_for_FPGA_based_BMF.pdf` for more details.

## 2.5 Testbenches

Testbenches for these blocks can be found in their respective folders (`MiniFloat/MiniFloatTB` and `BlockMat/BlockMatTB`). The main `*TB.cpp` file can be used as the testbench with Vivado HLS, it imports testbenches for components depending on the configurations specified in the Python script in the same directory. A python file is used to generate the C++ testbench file. Specific formats can be added to the configuration lists in the python file in order to test them.



<a name="examples"></a>
# 3 Examples

## 3.1 2x1 Dot Product

This example can be found in `MiniFloat/DotPrd2`. It is a circuit that computes the dot product of a pair of 2x1 vectors. The parameters in `DotPrd2.hpp` can be tweaked to change the length of the vector as well as the format used.

## 3.2 Block Matrix Multiplier

A Block Matrix Multiplier performs matrix multiplication with a pair of blocks of `BMF` or `BFP` values. This example can be found in `BlockMat/SubMatMul`. The parameters in `SubMatMul.hpp` can be tweaked to change the parameters of the number representation and the format itself (`BMF`/`BFP`). Other parameters are also exposed here such as whether normalisation is done after the operation, and the degree of unrolling in different components.

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

