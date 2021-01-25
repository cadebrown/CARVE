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

# checks for appropriate register tokens 
def check_imm(imm):
    print("\t// look for {} immediate with 0x{:x} mask".format(imm, genmask(*[int(i) for i in j[4:-1].split(":")])))

# checks for appropriate register tokens 
def check_reg(reg):
    print("\t// look for {} register".format(reg))

# find argument format for all defined types
with open(args.instfrmt_file) as fp:
    next(fp)
    for line in fp:
        table.append(line.strip().split(','))

    itr = 0
    for r in table:
        instfrmt[r[0]] = ([i for i in r[1:] if i != ''], itr)
        itr += 1
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
    print("\tCARVE_INST_" + i + " = " + str(instfrmt[i][1]) + ",")
print("};\n")

# create constant sorted array of instructions
print("static const carve_inst INSTRUCTIONS[] = {")
for i in sorted([j for j in ops]):
    print("\t{{ \"{}\", CARVE_INST_{} }},".format(i, ops[i]))
print("};\n")

# create parsing methods
for i in instfrmt:
    print("static int parse_{}(carve_tok* cur) {{".format(i))
    for j in instfrmt[i][0]:
        if j[0:3] == "imm":
            check_imm(j) #print("\tmatch imm; //{:032b};".format()
        elif j[0] == "r":
            check_reg(j)
        else:
            print("\tUNCAUGHT {};".format(j))
    print("}\n")

# search for string in carve_inst
print("""
static int bin_search(char* key, int len, carve_inst* instructions) {
    int left = 0, right = CARVE_N_INST, mid, res;
    while (left < right) {
        mid = left + ((right - left) / 2);
        if ((res = strncmp(key, instructions[mid][0], len)) == 0) {
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

# find instruction type and parse arguments
print("""
int check_inst(carve_tok* tok) {
    // checkity time
    int idx = binsearch(tok->kind);

    if (idx < 0) {
        return idx;
    }

    switch (INSTRUCTIONS[idx][i]) {""")

for i in instfrmt:
    print("\tcase CARVE_INST_{}:".format(i))
    print("\t\treturn parse_{}(tok);\n\t\tbreak;".format(i))

print("""\tdefault:
\t\treturn -1;
\t\tbreak;
    }
}""")