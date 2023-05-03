
# Configurations to be tested.
bmf_mul_frmts = ['2,0','2,1','2,2','3,0','3,1','3,2','4,3']
bmf_mul_range = [str(n) for n in range(1,5)]
bmf_mul_ext   = ['15','16','17','32','48','64','65']

bfp_mul_frmts = ['4,0','4,1','4,2','5,0','5,1','5,2','6,3']
bfp_mul_range = [str(n) for n in range(1,5)]
bfp_mul_ext   = ['15','16','17','32','48']

bmf_bfp_frmts = ['2,0','2,1','2,2','3,0','3,1','3,2','4,3']
bmf_bfp_range = [str(n) for n in range(1,5)]
bmf_bfp_ext   = ['15','16','17','32','48','64','65']




# Create TB file and generate tests.
tb_file = open('BlockMat/BlockMatTB.cpp', 'w')

tb_file.write(
'''#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "BlockMat.hpp"

#include "HelperTB.hpp"
#include "BlockMulTB.hpp"
#include "BlockConvTB.hpp"


int main(){
    try{

''')


for n in bmf_mul_range:
    for config in bmf_mul_frmts:
        tb_file.write('\t\tbmf_mul_tb<'+n+','+config+'>();\n')
    tb_file.write('\n')

for ext in bmf_mul_ext:
    tb_file.write('\t\tbmf_mul_tb<'+ext+',2,0>();\n')

tb_file.write('\n')

for n in bfp_mul_range:
    for config in bfp_mul_frmts:
        tb_file.write('\t\tbfp_mul_tb<'+n+','+config+'>();\n')
    tb_file.write('\n')

for ext in bfp_mul_ext:
    tb_file.write('\t\tbfp_mul_tb<'+ext+',4,0>();\n')

tb_file.write('\n')

for n in bmf_bfp_range:
    for config in bmf_bfp_frmts:
        tb_file.write('\t\tbmf_to_bfp_tb<'+n+','+config+'>();\n')
    tb_file.write('\n')

for ext in bmf_bfp_ext:
    tb_file.write('\t\tbmf_to_bfp_tb<'+ext+',2,0>();\n')

tb_file.write('\n')



tb_file.write('''
    }catch(int e){
        std::cout << "Block Operator TB Failed.\\n";
    }

    return 0;
}

''')

tb_file.close()

