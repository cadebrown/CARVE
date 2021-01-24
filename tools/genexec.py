#!/usr/bin/env python3
""" genexec.ks - Generate execution code for CSV description of instructions

Download this as CSV: https://docs.google.com/spreadsheets/d/1qsFr-ZnfAd3_Hjj07Y0Iku6Z6y6NC2hjKmN0Ar4kTpk/edit?usp=sharing

Then run:

$ ./tools/genexec.py file.csv

@author: Cade Brown <cade@kscript.org>

"""

import argparse
parser = argparse.ArgumentParser(description='Generate execution code')

parser.add_argument('src', help='an integer for the accumulator')

args = parser.parse_args()

table = []

fp = open(args.src)
next(fp)
for line in fp:
    table.append(line.strip().split(','))

fp.close()


seenop = set()

macros = ''

ind = '    '
print(ind * 0 + f"static inline void exec_inst(carve_state s, carve_inst inst) {{")

print(ind * 1 + "carve_inst opcode, rd, f3, rs1, rs2, f7, imm_11_0, imm_31_12, imm_4_0, imm_11_5, _11, imm_4_1, imm_10_5, _12, imm_19_12, imm_10_1, _20, tmp0, tmp1, tmp2;")
print(ind * 1 + "switch (opcode = inst & 0x7F) {")

for n, k, o, f3, f7 in table:
    if o and o not in seenop:
        seenop.add(o)
        print(ind * 2 + f"case {hex(int('0b'+o, 2))}:")

        if k == 'R':
            # Has f3 and f7
            print(ind * 3 + f"CARVE_DEC_R(inst, opcode, rd, f3, rs1, rs2, f7); ")
            print(ind * 3 + f"switch (f3) {{")
            
            seenf3 = set()
            for _n, _k, _o, _f3, _f7 in table:
                if k == _k and o == _o and not _f3 in seenf3:
                    seenf3.add(_f3)
                    seenf7 = set()

                    print(ind * 4 + f"case {hex(int('0b'+_f3, 2))}:")

                    print(ind * 5 + f"switch (f7) {{")

                    for __n, __k, __o, __f3, __f7 in table:
                        if o == __o and __f3 == _f3 and __f7 not in seenf7:
                            seenf7.add(__f7)
                            print(ind * 6 + f"case {hex(int('0b'+__f7, 2))}:")
                            print(ind * 7 + f"/* {__n} */")
                            print(ind * 7 + f"CARVE_DO_{__n}(rd, rs1, rs2);")
                            print(ind * 7 + f"break;")
                            macros += f'#define CARVE_DO_{__n}(_rd, _rs1, _rs2)\n'

                    print(ind * 5 + f"}}")
            print(ind * 3 + f"}}")

        elif k == 'I':
            # Has f3
            print(ind * 3 + f"CARVE_DEC_I(inst, opcode, rd, f3, rs1, imm_11_0);")
            print(ind * 3 + f"switch (f3) {{")
            
            seenf3 = set()
            for _n, _k, _o, _f3, _f7 in table:
                if k == _k and o == _o and _f3 not in seenf3:
                    seenf3.add(_f3)
                    seenf7 = set()

                    print(ind * 4 + f"case {hex(int('0b'+_f3, 2))}:")
                    print(ind * 5 + f"/* {_n} */")
                    print(ind * 5 + f"tmp0 = imm_11_0 << 0; ")
                    print(ind * 5 + f"CARVE_DO_{_n}(rd, rs1, tmp0);")
                    print(ind * 5 + f"break;")
                    macros += f'#define CARVE_DO_{_n}(_rd, _rs1, _imm)\n'

            print(ind * 3 + f"}}")
        elif k == 'U':
            # Neither
            print(ind * 3 + f"CARVE_DEC_U(inst, opcode, rd, imm_31_12);")
            print(ind * 3 + f"/* {n} */")
            print(ind * 3 + f"tmp0 = (imm_31_12 << 12);")
            print(ind * 3 + f"CARVE_DO_{n}(rd, tmp0);")
            macros += f'#define CARVE_DO_{n}(_rd, _imm)\n'

        elif k == 'S':
            # Has f3
            print(ind * 3 + f"CARVE_DEC_S(inst, opcode, imm_4_0, f3, rs1, rs2, imm_11_5);")
            print(ind * 3 + f"switch (f3) {{")
            seenf3 = set()
            for _n, _k, _o, _f3, _f7 in table:
                if k == _k and o == _o and _f3 not in seenf3:
                    seenf3.add(_f3)
                    seenf7 = set()

                    print(ind * 4 + f"case {hex(int('0b'+_f3, 2))}:")
                    print(ind * 5 + f"/* {_n} */")
                    print(ind * 5 + f"tmp0 = (imm_4_0 << 0) | (imm_11_5 << 5);")
                    print(ind * 5 + f"CARVE_DO_{_n}(rs1, rs2, tmp0);")
                    print(ind * 5 + f"break;")
                    macros += f'#define CARVE_DO_{_n}(_rs1, _rs2, _imm)\n'

            print(ind * 3 + f"}}")
        elif k == 'B':
            # Has f3
            print(ind * 3 + f"CARVE_DEC_B(inst, opcode, _11, imm_4_1, f3, rs1, rs2, imm_10_5, _12);")
            print(ind * 3 + f"switch (f3) {{")

            seenf3 = set()
            for _n, _k, _o, _f3, _f7 in table:
                if k == _k and o == _o and _f3 not in seenf3:
                    seenf3.add(_f3)
                    seenf7 = set()

                    print(ind * 4 + f"case {hex(int('0b'+_f3, 2))}:")
                    print(ind * 5 + f"/* {_n} */")
                    print(ind * 5 + f"tmp0 = (imm_4_1 << 1) | (imm_10_5 << 5) | (_11 << 11) | (_12 << 12);")
                    print(ind * 5 + f"CARVE_DO_{_n}(rs1, rs2, tmp0);")
                    print(ind * 5 + f"break;")
                    macros += f'#define CARVE_DO_{_n}(_rs1, _rs2, _imm)\n'

            print(ind * 3 + f"}}")
        elif k == 'J':
            # Has neither
            print(ind * 3 + f"CARVE_DEC_J(inst, opcode, rd, imm_19_12, _11, imm_10_1, _20);")
            print(ind * 3 + f"/* {n} */")
            print(ind * 3 + f"tmp0 = (imm_10_1 << 1) | (_11 << 11) | (imm_19_12 << 12) | (_20 << 20);")
            print(ind * 3 + f"CARVE_DO_{n}(rd, tmp0);")
            macros += f'#define CARVE_DO_{n}(_rd, _imm)\n'

        else:
            raise Exception('Unknown kind ' + repr(k))
        
        print(ind * 3 + f"break;")

print(ind * 1 + f"}}")
print(ind * 0 + f"}}")

print (macros)



