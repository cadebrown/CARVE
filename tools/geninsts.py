#!/usr/bin/env python3
""" geninsts.ks - Generates 'src/insts.cc'

Usage:
$ ./tools/geninsts.ks > src/insts.cc

@author: Cade Brown <cade@utk.edu>
"""

# Contains instruction information
import riscvdata


print(f"""/* insts.cc - GENERATED - List of instructions, as a map ('carve::insts')
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT **
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include <carve.hh>

namespace carve {{
""")


print(f"""
// List of all instructions
unordered_map<string, instdesc> insts = {{
""")


for (ext, insts) in riscvdata.exts.items():
    print(f"""    // ext: {ext}""")
    for name, kind, op, f3, f7, doc in insts:
        print(f"""    {{ \"{name}\", {{ \"{name}\", \"{kind}\", {hex(op)}, {hex(f3)}, {hex(f7)} }} }},""")


print(f"""}};
""")

print(f"""}}
""")
