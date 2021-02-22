#!/usr/bin/env python3
""" gengetinst.ks - Generates table for 'src/getinst.c'

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


print(f"""/* getinst.c - Generated file that defines parse modes for RISC-V instructions
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT ** 
 */

#include <carve.h>

""")


print(f"""

/* Database of instructions */

static int I_ninsts = {len(insts)};
static struct carve_instdesc I_insts[{len(insts)}] = {{
""")

for name, kind, opcode, f3, f7 in insts:
    print (f"""    (struct carve_instdesc) {{ "{name}", {len(name)}, '{kind}', {opcode}, {f3}, {f7} }},""")

print(f"""
}};

""")


print(f"""

/* Retrieve instruction information */
struct carve_instdesc* carve_getinst(char* src, int len) {{
    int l = 0, r = I_ninsts - 1, m;
    while (l <= r) {{
        m = (l + r) / 2;

        /* Key we are comparing to */
        int nname = I_insts[m].nname;
        char* name = I_insts[m].name;

        int cmp = 0;
        
        if (len == nname) {{
            cmp = strncmp(src, name, nname);
        }} else {{
            int minl = len < nname ? len : nname;
            cmp = strncmp(src, name, minl);
            if (cmp == 0) {{
                cmp = len - nname;
            }}
        }}

        if (cmp == 0) {{
            return &I_insts[m];
        }} else if (cmp > 0) {{
            l = m + 1;
        }} else {{
            r = m - 1;
        }}
    }}

    /* Not-found */
    return NULL;
}}
""")
