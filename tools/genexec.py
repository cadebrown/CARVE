#!/usr/bin/env python3
""" genexec.ks - Generates 'src/exec.cc'

The general strategy used is decoding the opcode of the instruction, then discriminating based on
  our database of instructions. Once we know the kind, we decode that even further, and if the kind
  also requires funct3 and/or funct7, we have more discrimination layers until we hit a leaf node
  and then execute the code

Usage:
$ ./tools/genexec.ks > src/exec.cc

@author: Cade Brown <cade@utk.edu>
"""

# Contains instruction information
import riscvdata


print(f"""/* exec.cc - GENERATED - Execution implementation
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT **
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include <carve.hh>

namespace carve {{
""")

print(f"""

void State::exec(inst v) {{
    u64 op, f3, f7, imm;
    int rd, rs1, rs2;
    // Decode opcode
    switch (op = v & 0x7F) {{""")

# Now, populate a decision tree on 'op -> f3 -> f7' (for kinds that support it)

# tree[op][f3][f7] = name
# Each key indirection is an edge in the tree
tree = {}

# Map of opcodes to the kind of instruction
o2k = {}


for (ext, insts) in riscvdata.exts.items():
    for name, kind, op, f3, f7, doc in insts:
        if op == 0:
            continue
        o2k[op] = kind

        if op not in tree:
            tree[op] = {}
        
        if kind in ('R',):
            # f3, f7
            if f3 not in tree[op]:
                tree[op][f3] = {}
            tree[op][f3][f7] = name
        elif kind in ('I', 'S', 'B', 'y'):
            # f3
            if f3 in tree[op]:
                continue
            tree[op][f3] = name
        elif kind in ('J', 'U'):
            # Nothing extra, only opcode matters
            tree[op] = name
        elif kind in ('p',):
            # Skip psuedoinstructions
            continue
        else:
            raise Exception(f'Unknown instruction kind: {kind}')



# Now, generate code corresponding to the tree
for (op, sub) in tree.items():
    print(f"""        case {hex(op)}:""")

    # First, decode based on the kind
    kind = o2k[op]
    if kind in ('R',):
        print(f"""            dec_R(v, op, f3, f7, rd, rs1, rs2);""")
    elif kind in ('I',):
        print(f"""            dec_I(v, op, f3, rd, rs1, imm);""")
    elif kind in ('U',):
        print(f"""            dec_U(v, op, rd, imm);""")
    elif kind in ('S',):
        print(f"""            dec_S(v, op, f3, rs1, rs2, imm);""")
    elif kind in ('B',):
        print(f"""            dec_B(v, op, f3, rs1, rs2, imm);""")
    elif kind in ('J',):
        print(f"""            dec_J(v, op, rd, imm);""")
    elif kind in ('y',):
        print(f"""            dec_y(v, op, f3, rd, rs1, imm);""")


    # Outputs code to run the given instruction
    def run(name, ind):
        # Function name (i.e. what should be defined)
        # Can be a macro or function
        func = f"impl::_{name}"
        if kind in ('R',):
            print(f"""{ind}{func}(*this, rd, rs1, rs2);""")
        elif kind in ('I',):
            print(f"""{ind}{func}(*this, rd, rs1, imm);""")
        elif kind in ('U',):
            print(f"""{ind}{func}(*this, rd, imm);""")
        elif kind in ('S',):
            print(f"""{ind}{func}(*this, rs1, rs2, imm);""")
        elif kind in ('B',):
            print(f"""{ind}{func}(*this, rs1, rs2, imm);""")
        elif kind in ('J',):
            print(f"""{ind}{func}(*this, rd, imm);""")
        elif kind in ('y',):
            print(f"""{ind}{func}(*this, rd, rs1, imm);""")
        else:
            raise Exception(f'Unknown instruction kind: {kind}')

    if isinstance(sub, dict):
        # Has a subtree of f3 edges
        print(f"""            switch (f3) {{""")
        for (f3, subsub) in sub.items():
            print(f"""                case {hex(f3)}:""")
            if isinstance(subsub, dict):
                print(f"""                    switch (f7) {{""")
                # Has a subtree of f7 edges
                for (f7, subsubsub) in subsub.items():
                    print(f"""                        case {hex(f7)}:""")
                    run(subsubsub, "                            ")
                    print(f"""                            break;""")

                print(f"""                    }}""")

            else:
                # 'subsub' is the name
                run(subsub, "                    ")

            print(f"""                    break;""")


        print(f"""            }}""")

    else:
        # Has only opcode, 'sub' is the name
        run(sub, "            ")


    print(f"""            break;""")

print(f"""
    }}
}}

""")


print(f"""}}
""")
