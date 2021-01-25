#!/usr/bin/env python3
""" gencmp.ks - Generate instruction identification

Download this as CSV: https://docs.google.com/spreadsheets/d/1qsFr-ZnfAd3_Hjj07Y0Iku6Z6y6NC2hjKmN0Ar4kTpk/edit?usp=sharing

Then run:

$ ./tools/gengmp.py inst_file.csv instfrmt_file.csv

@author: Gregory Croisdale <greg@kscript.org>

"""

import argparse
from datetime import datetime

parser = argparse.ArgumentParser(description='Generate comparison code')
parser.add_argument('inst_file', help='csv file of all instructions and their types')
parser.add_argument('instfrmt_file', help='csv file of instruction format information based on type')

args = parser.parse_args()

table = []
instfrmt = dict()
ops = dict()

# create a bitmask from a left and right bit index
def genmask(left: int, right: int) -> int:
    out = 0
    for i in range(right, left):
        out += 2 ** i
    #print(left, right)
    #print(out, "= {0:b}".format(out, out))
    return out

# find argument format for all defined types
with open(args.instfrmt_file) as fp:
    next(fp)
    for line in fp:
        table.append(line.strip().split(','))

    for r in table:
        instfrmt[r[0]] = [i for i in r[1:] if i != '']
table = []
    
# find all arguments and their types
with open(args.inst_file) as fp:
    next(fp)
    for line in fp:
        table.append(line.strip().split(','))

    for inst, kind, _, _, _ in table:
        ops[inst] = kind

# print metadata
print("""
/*
 *  carve_inst_ident.c
 *  Generated at {} using gencmp.py
 */
""".format(datetime.now()))

# include enums and struct information
print("""
#ifndef CARVE_PARSE_H
#include "parse.h"
#define CARVE_PARSE_H
#endif
""")

# set n instructions and enum for types and tokens
print("#define CARVE_N_INST\n".format(len(ops)))
print("enum CARVE_INST_TYPES = {")
for i in instfrmt:
    print("\tCARVE_INST_" + i + ",")
print("};\n")

# create constant sorted array of instructions
print("static const carve_inst INSTRUCTIONS[] = {")
for i in sorted([j for j in ops]):
    print("\t{{ \"{}\", CARVE_INST_{} }},".format(i, ops[i]))
print("};")

# 
for i in instfrmt:
    print("static int parse_{}(carve_tok* cur) {{".format(i))
    for j in instfrmt[i]:
        if j[0:3] == "imm":
            print("\tmatch imm; //{:032b};".format(genmask(*[int(i) for i in j[4:-1].split(":")])))
        else:
            print("\tmatch {};".format(j))
    print("}\n")

# search for string in carve_inst
print("""
static int bin_search(char* key, int len, carve_inst* instructions) {
    int left = 0, right = CARVE_N_INST, mid, res;
    while (left < right) {
        mid = left + ((right - left) / 2);
        if ((res = strncmp(key, INSTRUCTIONS[mid][0], len)) == 0) {
            return mid;
        } else if (res > 0) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return -1;
}
""")

#
print("""
int check_inst(carve_tok* tok) {
    // checkity time
}
""")