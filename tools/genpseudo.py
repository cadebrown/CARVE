#!/usr/bin/env python3

""" genpseudo.ks - Generates table for 'src/parse_p.h'

@author: Gregory Croisdale <greg@kscript.org> 
"""

# Contains instruction information
import riscvdata


# Sorted list of all instructions containing rows of:
#   (name, kind, opcode, f3, f7)
# Sorted on 'name'
pseudos = {p: f"CARVE_PSEUDO_{p}" for p in riscvdata.pseudos}

def switchcase_search(keys: dict):
    tree = {}
    for k in keys:
        cur = tree
        for c in k:
            if c not in cur:
                cur[c] = dict()
            cur = cur[c]
        cur["default"] = keys[k]
    
    def visit(node, depth, tree):
        out = ""
        if node == "default":
            out += f"{'  ' * (depth + 1)}case '\\0': {{{tree[node]}(); return true;}} break; \\\n"
        else:
            depth += 1
            out += f"{'  ' * depth}case \'{node}\': \\\n"
            depth += 1
            out += f"{'  ' * depth}switch (name[counter++]) {{ \\\n"
            for k in tree[node]:
                out += visit(k, depth, tree[node])
            out += f"{'  ' * (depth)}}} break; \\\n"
        return out
    
    return "switch (name[counter++]) { \\\n" + "".join([visit(k, 1, tree) for k in tree]) + "  }"

print(f"""/* parse_p.h - Generated file that defines parse modes for RISC-V pseudoinstructions
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT ** 
 */

#include <carve.h>""")

for ext in riscvdata.exts:
    print(f"""#ifdef {ext}
  #include "ext/{ext}.h"
#endif /* {ext} */
""")

print(f"""#define CARVE_parse_p() do {{\\
  int counter = 0; \\
  {switchcase_search(pseudos)} \\
  \\
  fprintf(stderr, "Unsupported pseudoinstruction!\\n"); \\
  carve_printcontext(prog->fname, prog->src, (*toksp)[*tokip - 1]); \\
  return false; }} while (0)
""")