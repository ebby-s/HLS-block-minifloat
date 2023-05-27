
# Configurations to be tested.
# Working: E=[2,4], M=0
# Working: E=[2,4], M=16
# Working: E=2, M=[0,16]
# Working: E=4, M=[0,16]
bmf_mul_en    = True
bmf_mul_frmts = ['2,0','2,1','2,2','3,0','3,1','3,2','4,3']
bmf_mul_range = [str(n) for n in range(1,5)]
bmf_mul_ext   = ['15','16','17','32','48','64','65']

bfp_mul_en    = True
bfp_mul_frmts = ['4,0','4,1','4,2','5,0','5,1','5,2','6,3']
bfp_mul_range = [str(n) for n in range(1,5)]
bfp_mul_ext   = ['15','16','17','32','48']

bmf_bfp_en    = False
bmf_bfp_frmts = ['2,0','2,1','2,2','3,0','3,1','3,2','4,3']
bmf_bfp_range = [str(n) for n in range(1,5)]
bmf_bfp_ext   = ['15','16','17','32','48','64','65']

bfp_bmf_en       = False
bfp_bmf_frmts    = ['2,0','2,1','3,0','4,4','4,6']
bfp_bmf_frmts_in = ['4,-1','4,0','4,2']
bfp_bmf_range    = [str(n) for n in range(1,5)]
bfp_bmf_ext      = ['15','16','17','32','48','65']

bfp_bfp_en       = False
bfp_bfp_frmts    = ['4,0','6,2','10,4']
bfp_bfp_frmts_in = ['4,0','6,2','10,4']
bfp_bfp_range    = [str(n) for n in range(1,5)]
bfp_bfp_ext      = ['15','16','17','32','48','65']

bfp_add_en    = True
bfp_add_frmts = ['4,0','4,1','4,2','5,0','5,1','5,2','6,3']
bfp_add_range = [str(n) for n in range(1,5)]
bfp_add_ext   = ['15','16','17','32','48']

mul_norm = True   # Generate & test normalisation stage of matrix multipliers.


# Create TB file and generate tests.
tb_file = open('BlockMat/BlockMatTB/BlockMatTB.cpp', 'w')

tb_file.write(
'''#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

''')

tb_file.write("#define MUL_NORM "+str(int(mul_norm))+"\n")

tb_file.write(
'''
#include "../BlockMat.hpp"

#include "HelperTB.hpp"
#include "BlockMulTB.hpp"
#include "BlockConvTB.hpp"
#include "BlockAddTB.hpp"

int main(){
    try{

''')

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

if bfp_add_en:
    for n in bfp_add_range:
        for config in bfp_add_frmts:
            tb_file.write('\t\tbfp_add_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')

    for ext in bfp_add_ext:
        tb_file.write('\t\tbfp_add_tb<'+ext+',4,0>();\n')

    tb_file.write('\n')



tb_file.write('''
    }catch(int e){
        std::cout << "Block Operator TB Failed.\\n";
    }

    return 0;
}

''')

tb_file.close()

