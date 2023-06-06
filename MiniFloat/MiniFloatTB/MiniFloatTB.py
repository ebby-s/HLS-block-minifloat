
# Control which components are tested here:


mf_mul_en = False   # MiniFloat Multiplier
ia_mul_en = False   # IntAcc Multiplier
ia_add_en = False   # IntAcc Adder
mf_dp_en  = False   # MiniFloat dot product circuits
ia_dp_en  = True   # IntAcc dot product circuits


# Control which configurations are tested:

# frmts -> Input formats to be tested. 'E,M' for MiniFloat, 'W,F' for IntAcc.
# rand  -> Postfix indicates random testing, not exhaustive. For large formats.
# range -> Range of parameter N to be tested, repeat for all formats specified.

mf_mul_frmts       = ['2,0','3,0','4,0','5,0','6,0','2,1','2,2','2,3','2,4']
mf_mul_frmts_rand  = ['2,5','2,6','2,7','2,8','2,9','2,10','2,11','2,12','2,13','2,14','2,15']
mf_mul_frmts_rand += ['4,2','4,3','4,4','4,5','4,6','4,8','4,11','4,12','4,15']

ia_mul_frmts       = ['2,0','3,0','4,0','5,0','6,0','4,-2','4,-1','4,1','4,2','4,3','4,4']
ia_mul_frmts_rand  = ['7,0','8,0','9,0','10,0','11,0','4,-6','4,-4','4,6','4,7','4,9']
ia_mul_frmts_rand += ['16,0','24,0','32,0','48,0','50,0']

ia_add_frmts       = ['2,0','3,0','4,0','5,0','6,0','4,-2','4,-1','4,1','4,2','4,3','4,4']
ia_add_frmts_rand  = ['7,0','8,0','9,0','10,0','11,0','4,-6','4,-4','4,6','4,7','4,9']
ia_add_frmts_rand += ['16,0','24,0','32,0','48,0','50,0']

mf_dp_frmts_rand  = ['2,0','3,0','4,0','5,0','2,1','2,2','2,3','2,4']
mf_dp_frmts_rand += ['2,5','2,6','2,7','2,8','2,9','2,10','2,11','2,12','2,13','2,14','2,15']
mf_dp_frmts_rand += ['4,2','4,3','4,4','4,5','4,6','4,8','4,11','4,12','4,15']
mf_dp_range  = [str(x) for x in range(1,5)]

ia_dp_frmts_rand  = ['2,0','3,0','4,0','5,0','6,0','4,-2','4,-1','4,1','4,2','4,3','4,4']
ia_dp_frmts_rand += ['7,0','8,0','9,0','10,0','11,0','4,-6','4,-4','4,6','4,7','4,9']
ia_dp_frmts_rand += ['16,0','24,0']
ia_dp_range  = [str(x) for x in range(1,5)]



# Create TB file
tb_file = open('MiniFloat/MiniFloatTB/MiniFloatTB.cpp', 'w')

# Add includes.
tb_file.write(
'''#include <iostream>
#include <cassert>

#include "../MiniFloat.hpp"

#include "MFMulTB.hpp"
#include "IAMulTB.hpp"
#include "IAAddTB.hpp"
#include "DotPrdTB.hpp"


int main(){
    try{

''')

# Generate tests.
if mf_mul_en:
    for config in mf_mul_frmts:
        tb_file.write('\t\tmf_mul_tb<'+config+'>();\n')
    tb_file.write('\n')

    for config in mf_mul_frmts_rand:
        tb_file.write('\t\tmf_mul_tb_rand<'+config+'>();\n')
    tb_file.write('\n')

if ia_mul_en:
    for config in ia_mul_frmts:
        tb_file.write('\t\tia_mul_tb<'+config+'>();\n')
    tb_file.write('\n')

    for config in ia_mul_frmts_rand:
        tb_file.write('\t\tia_mul_tb_rand<'+config+'>();\n')
    tb_file.write('\n')

if ia_add_en:
    for config in ia_add_frmts:
        tb_file.write('\t\tia_add_tb<'+config+'>();\n')
    tb_file.write('\n')

    for config in ia_add_frmts_rand:
        tb_file.write('\t\tia_add_tb_rand<'+config+'>();\n')
    tb_file.write('\n')

if mf_dp_en:
    for n in mf_dp_range:
        for config in mf_dp_frmts_rand:
            tb_file.write('\t\tmf_dp_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')

if ia_dp_en:
    for n in ia_dp_range:
        for config in ia_dp_frmts_rand:
            tb_file.write('\t\tia_dp_tb<'+n+','+config+'>();\n')
        tb_file.write('\n')


tb_file.write('''
    }catch(int e){
        std::cout << "Scalar Operator TB Failed.\\n";
    }

    return 0;
}

''')

tb_file.close()

