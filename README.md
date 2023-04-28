# Table of Contents

* [Overview](#Overview)
* [Usage](#Usage)
* [Examples](#Examples)
* [Contributors](#Contributors)
* [License](#license)

<a name="overview"></a>
# 1 Overview

This library contains HLS implementations of arithmetic units to perform matrix operations using block minifloat, logarithmic block minifloat and block floating point formats. Tested in Vivado HLS.

Refer to the report under `docs/HLS for FPGA-based BMF.pdf` for more details.

Refer to [this paper][BMF_link] for information about the BMF format.

[BMF_link]: https://openreview.net/pdf?id=6zaTwpNSsQ2


# 2 Usage

## 2.1 Primitives

### 2.1.1 MiniFloat

The MiniFloat class represents a single minifloat value, with sign, exponent and optional mantissa.

### 2.1.2 IntAcc


## 2.2 Blocks

### 2.2.1 BlockMF

### 2.2.2 BlockFP


## 2.3 Helper Functions/Macros


# 3 Examples

## 3.1 2x1 Dot Product

## 3.2 BMF Matrix Multiplier

## 3.3 BFP Matrix Multiplier


# 4 Contributors

Ebby Samson: ebby.samson19@imperial.ac.uk

Credits to authors of [this paper][BMF_link], especially Philip Boland ([philip.leong@sydney.edu.au][Philip_email]).

Final Year Project Supervisor: George Constantinides ([g.constantinides@imperial.ac.uk][George_email]).


[Philip_email]: philip.leong@sydney.edu.au
[George_email]: g.constantinides@imperial.ac.uk

<a name="license"></a>
# 5 License


