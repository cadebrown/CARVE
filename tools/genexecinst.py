#!/usr/bin/env python3
""" genexecinst.ks - Generate function to execute a single RISC-V instruction

$ ./tools/genexecinst.py > src/execinst.c

You can copy that and put it in 'src/exec.c'

@author: Cade Brown <cade@kscript.org>

"""

import argparse

# Contains instruction information
import riscvdata


""" Strategy:

Find a tree that is keyed on:

 opcode -> f3 -> f7
 (f3 and f7 only used on instruction types which require them)

And then generate a nested switch/case

"""


# Tree showing how to decode instructions
tree = {}

# Mapping of opcodes to '(extension, instkind)'
kinds = {}

for ext in riscvdata.exts:
    for (name, kind, opcode, f3, f7) in riscvdata.exts[ext]:
        if opcode not in tree:
            # May be reset to just the name of an instruction
            tree[opcode] = {}
            kinds[opcode] = (ext, kind)
        
        if kind == 'R':
            # Use f3 and f7
            if f3 not in tree[opcode]:
                tree[opcode][f3] = {}

            # Shouldn't have two exact matches
            assert f7 not in tree[opcode][f3]

            # Leaf node of the instruction
            tree[opcode][f3][f7] = name
        elif kind == 'I':
            # Shouldn't have two exact matches
            if f3 in tree[opcode]:
                continue

            # Leaf node of the instruction
            tree[opcode][f3] = name
        elif kind == 'S':
            # Shouldn't have two exact matches
            assert f3 not in tree[opcode]

            # Leaf node of the instruction
            tree[opcode][f3] = name
        elif kind == 'B':
            # Shouldn't have two exact matches
            assert f3 not in tree[opcode]

            # Leaf node of the instruction
            tree[opcode][f3] = name
        elif kind == 'J':
            # Neither

            # Leaf node of the instruction
            tree[opcode] = name
        elif kind == 'U':
            # Neither

            # Leaf node of the instruction
            tree[opcode] = name
        elif kind == 'p':
            pass
        elif kind == 'y':
            tree[opcode] = name

        else:
            raise Exception('Unknown kind: ' + repr(kind))


print(f"""/* execinst.c - Generated file that defines executing RISC-V instructions
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT ** 
 */

#include "ext/common.h"

""")

for ext in riscvdata.exts:
    print(f"""#ifdef {ext}
  #include "ext/{ext}.h"
#endif /* {ext} */
""")

print(f"""void carve_execinst(carve_state s, carve_inst inst) {{
    carve_inst opcode, f3, f7, rd, rs1, rs2, imm;    
    switch (opcode = inst & 0x7F) {{""")


for opcode in tree:
    # Returns string of code to execute for 'name'
    def output_exec(name):
        res = f"CARVE_{name}("
        kind = riscvdata.get_kind(name)
        if kind == 'R':
            res += 'rd, rs1, rs2'
        elif kind == 'I':
            res += 'rd, rs1, imm'
        elif kind == 'U':
            res += 'rd, imm'
        elif kind == 'S':
            res += 'rs1, rs2, imm'
        elif kind == 'B':
            res += 'rs1, rs2, imm'
        elif kind == 'J':
            res += 'rd, imm'
        res += ");"
        return res

    val = tree[opcode]
    ext, kind = kinds[opcode]
    print(f"""        case {hex(opcode)}:""")
    if isinstance(val, dict):
        # 'val' is subtree, keyed on 'f3'

        if kind == 'R':
            print(f"""            CARVE_DEC_R(inst, opcode, f3, f7, rd, rs1, rs2);""")
        elif kind == 'I':
            print(f"""            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);""")
        elif kind == 'U':
            print(f"""            CARVE_DEC_U(inst, opcode, rd, imm);""")
        elif kind == 'S':
            print(f"""            CARVE_DEC_S(inst, opcode, f3, rs1, rs2, imm);""")
        elif kind == 'B':
            print(f"""            CARVE_DEC_B(inst, opcode, f3, rs1, rs2, imm);""")
        elif kind == 'J':
            print(f"""            CARVE_DEC_J(inst, opcode, rd, imm);""")

        print(f"""            switch (f3) {{""")

        for f3 in val:
            val_ = val[f3]
            print(f"""                case {hex(f3)}:""")

            if isinstance(val_, dict):
                # 'val_' is a subtree, keyed on 'f7'
                print(f"""                    switch (f7) {{""")
                for f7 in val_:
                    val__ = val_[f7]
                    print(f"""                        case {hex(f7)}:""")
                    print(f"""                          #ifdef {ext}""")
                    print(f"""                            /* {val__} */""")
                    print(f"""                            {output_exec(val__)}""")
                    print(f"""                          #else""")
                    print(f"""                            HALT("unsupported instruction '{val__}' (extension {ext} not supported)"); """)
                    print(f"""                          #endif /* {ext} */""")
                    print(f"""                            break;""")

                print(f"""                    }}""")

            else:
                # 'val_' is a instruction
                print(f"""                  #ifdef {ext}""")
                print(f"""                    /* {val_} */""")
                print(f"""                    {output_exec(val_)}""")
                print(f"""                  #else""")
                print(f"""                    HALT("unsupported instruction '{val_}' (extension {ext} not supported)"); """)
                print(f"""                  #endif /* {ext} */""")

            print(f"""                    break;""")


        print(f"""            }}""")

    else:
        # 'val' is instruction
        if kind == 'R':
            print(f"""            CARVE_DEC_R(inst, opcode, f3, f7, rd, rs1, rs2);""")
        elif kind == 'I':
            print(f"""            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);""")
        elif kind == 'U':
            print(f"""            CARVE_DEC_U(inst, opcode, rd, imm);""")
        elif kind == 'S':
            print(f"""            CARVE_DEC_S(inst, opcode, f3, rs1, rs2, imm);""")
        elif kind == 'B':
            print(f"""            CARVE_DEC_B(inst, opcode, f3, rs1, rs2, imm);""")
        elif kind == 'J':
            print(f"""            CARVE_DEC_J(inst, opcode, rd, imm);""")
        print(f"""            /* {val} */""")
        print(f"""          #ifdef {ext}""")
        print(f"""            {output_exec(val)}""")
        print(f"""          #else""")
        print(f"""             HALT("unsupported instruction '{val}' (extension {ext} not supported)"); """)
        print(f"""          #endif /* {ext} */""")

    print(f"""            break;""")

print(f"    }}")
print(f"}}")
