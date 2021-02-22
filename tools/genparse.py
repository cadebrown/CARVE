#!/usr/bin/env python3
""" genparse.ks - Generates table for 'src/parse.c'

@author: Gregory Croisdale <greg@kscript.org>
@author: Cade Brown <cade@kscript.org>
 
"""

from datetime import datetime

# Contains instruction information
import riscvdata


# Sorted list of all instructions containing rows of:
#   (name, kind, opcode, f3, f7)
# Sorted on 'name'
insts = sorted(riscvdata.insts, key=lambda x: x[0])

print(f"""

/* Database of instructions */

static int I_ninsts = {len(insts)};
static struct instdesc I_insts[{len(insts)}] = {{
""")

for name, kind, opcode, f3, f7 in insts:
    print (f"""    (struct instdesc) {{ "{name}", {len(name)}, '{kind}', {opcode}, {f3}, {f7} }},""")

print(f"""
}};

""")
