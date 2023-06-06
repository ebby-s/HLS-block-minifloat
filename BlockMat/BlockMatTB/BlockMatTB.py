

# Control which components are tested here:

bmf_mul_en = False   # BMF Matrix Multiplication
bfp_mul_en = False   # BFP Matrix Multiplication
bfp_add_en = False   # BFP Addition

bfp_nrm_en = False   # BFP Normalisation

bmf_bfp_en = True   # BMF to BFP Conversion
bfp_bmf_en = True   # BFP to BMF Conversion
bfp_bfp_en = True   # BFP to BFP Conversion


# Control which configurations are tested:

# Phase 1: Test small block sizes with a range of configurations.
# frmts -> Input formats to be tested, random stimulus. 'E,M' for MiniFloat, 'W,F' for IntAcc.
# range -> Range of parameter N to be tested, repeat for all formats specified.

bmf_mul_frmts = ['2,0','2,1','2,2','3,0','3,1','3,2','4,3']
bmf_mul_range = [str(n) for n in range(1,5)]

bfp_mul_frmts = ['4,0','4,1','4,2','5,0','5,1','5,2','6,3']
bfp_mul_range = [str(n) for n in range(1,5)]

bfp_add_frmts = ['4,0','4,1','4,2','5,0','5,1','5,2','6,3']
bfp_add_range = [str(n) for n in range(1,5)]

bfp_nrm_frmts = ['4,0','4,1','4,2','5,0','5,1','5,2','6,3']
bfp_nrm_range = [str(n) for n in range(1,5)]

bmf_bfp_frmts = ['2,0','2,1','2,2','3,0','3,1','3,2','4,3']
bmf_bfp_range = [str(n) for n in range(1,5)]

# frmts_in -> Input formats for converters, all input formats tested with all output formats.

bfp_bmf_frmts    = ['2,0','2,1','3,0','4,4','4,6']
bfp_bmf_frmts_in = ['4,-1','4,0','4,2']
bfp_bmf_range    = [str(n) for n in range(1,5)]

bfp_bfp_frmts    = ['4,0','6,2','10,4']
bfp_bfp_frmts_in = ['4,0','6,2','10,4']
bfp_bfp_range    = [str(n) for n in range(1,5)]


# Phase 2: Test large block sizes, with a small configuration.
# ext   -> Test large values of N, for a small configuration.

bmf_mul_ext = ['15','16','17','32','48','64','65']
bfp_mul_ext = ['15','16','17','32','48']
bfp_add_ext = ['15','16','17','32','48']
bfp_nrm_ext = ['15','16','17','32','48']
bmf_bfp_ext = ['15','16','17','32','48','64','65']
bfp_bmf_ext = ['15','16','17','32','48','65']
bfp_bfp_ext = ['15','16','17','32','48','65']


# Create TB file.
tb_file = open('BlockMat/BlockMatTB/BlockMatTB.cpp', 'w')

# Add includes.
tb_file.write(
'''#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "../BlockMat.hpp"

#include "HelperTB.hpp"
#include "BlockMulTB.hpp"
#include "BlockConvTB.hpp"
#include "BlockAddTB.hpp"
#include "NormaliseTB.hpp"

int main(){
    try{

''')

# Generate tests.
if bmf_mul_en:
    for n in bmf_mul_range:
        for config in bmf_mul_frmts:
            tb_file.write('\t\tbmf_mul_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')

    for ext in bmf_mul_ext:
        tb_file.write('\t\tbmf_mul_tb<'+ext+',2,0>();\n')

    tb_file.write('\n')

if bfp_mul_en:
    for n in bfp_mul_range:
        for config in bfp_mul_frmts:
            tb_file.write('\t\tbfp_mul_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')

    for ext in bfp_mul_ext:
        tb_file.write('\t\tbfp_mul_tb<'+ext+',4,0>();\n')

    tb_file.write('\n')

if bfp_add_en:
    for n in bfp_add_range:
        for config in bfp_add_frmts:
            tb_file.write('\t\tbfp_add_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')

    for ext in bfp_add_ext:
        tb_file.write('\t\tbfp_add_tb<'+ext+',4,0>();\n')

    tb_file.write('\n')

if bfp_nrm_en:
    for n in bfp_nrm_range:
        for config in bfp_nrm_frmts:
            tb_file.write('\t\tbfp_norm_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')

    for ext in bfp_nrm_ext:
        tb_file.write('\t\tbfp_norm_tb<'+ext+',4,0>();\n')

    tb_file.write('\n')

if bmf_bfp_en:
    for n in bmf_bfp_range:
        for config in bmf_bfp_frmts:
            tb_file.write('\t\tbmf_to_bfp_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')

    for ext in bmf_bfp_ext:
        tb_file.write('\t\tbmf_to_bfp_tb<'+ext+',2,0>();\n')

    tb_file.write('\n')

if bfp_bmf_en:
    for n in bfp_bmf_range:
        for config in bfp_bmf_frmts_in:
            for out_cfg in bfp_bmf_frmts:
                tb_file.write('\t\tbfp_to_bmf_tb<'+n+','+config+','+out_cfg+'>();\n')
        tb_file.write('\n')

    for ext in bfp_bmf_ext:
        tb_file.write('\t\tbfp_to_bmf_tb<'+ext+',10,3,3,2>();\n')

    tb_file.write('\n')

if bfp_bfp_en:
    for n in bfp_bfp_range:
        for in_cfg in bfp_bfp_frmts_in:
            for out_cfg in bfp_bfp_frmts:
                tb_file.write('\t\tbfp_to_bfp_tb<'+n+','+in_cfg+','+out_cfg+'>();\n')
        tb_file.write('\n')

    for ext in bfp_bfp_ext:
        tb_file.write('\t\tbfp_to_bfp_tb<'+ext+',8,4,3,2>();\n')

    tb_file.write('\n')



tb_file.write('''
    }catch(int e){
        std::cout << "Block Operator TB Failed.\\n";
    }

    return 0;
}

''')

tb_file.close()

