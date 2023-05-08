
# Configurations to be tested.
# Working: E=[2,4], M=0
# Working: E=[2,4], M=16
# Working: E=2, M=[0,16]
# Working: E=4, M=[0,16]
mf_mul_en          = False
mf_mul_frmts       = ['2,0','3,0','4,0','5,0','6,0','2,1','2,2','2,3','2,4']
mf_mul_frmts_rand  = ['2,5','2,6','2,7','2,8','2,9','2,10','2,11','2,12','2,13','2,14','2,15']
mf_mul_frmts_rand += ['4,2','4,3','4,4','4,5','4,6','4,8','4,11','4,12','4,15']

ia_mul_en         = True
ia_mul_frmts      = ['2,0','3,0','4,0','5,0','6,0','4,-2','4,-1','4,1','4,2','4,3','4,4']
ia_mul_frmts_rand = ['7,0','8,0','9,0','10,0','11,0','4,-6','4,-4','4,6','4,7','4,9']

ia_add_en         = False
ia_add_frmts      = ['2,0','3,0','4,0','5,0','6,0','4,-2','4,-1','4,1','4,2','4,3','4,4']
ia_add_frmts_rand = ['7,0','8,0','9,0','10,0','11,0','4,-6','4,-4','4,6','4,7','4,9']

dp2_en = False





# Create TB file and generate tests.
tb_file = open('MiniFloat/MiniFloatTB/MiniFloatTB.cpp', 'w')

tb_file.write(
'''#include <iostream>
#include <cassert>

#include "../MiniFloat.hpp"
#include "../DotPrd2/DotPrd2.hpp"

#include "MFMulTB.hpp"
#include "IAMulTB.hpp"
#include "IAAddTB.hpp"
#include "DP2TB.hpp"


int main(){
    try{

''')

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

if dp2_en:
    tb_file.write('\t\tDP2_tb<Et,Mt>();\n')
    tb_file.write('\n')


tb_file.write('''
    }catch(int e){
        std::cout << "Scalar Operator TB Failed.\\n";
    }

    return 0;
}

''')

tb_file.close()

