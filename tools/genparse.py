#!/usr/bin/env python3
""" genparse.ks - Generates 'src/parse.c'

Download this as CSV: https://docs.google.com/spreadsheets/d/1qsFr-ZnfAd3_Hjj07Y0Iku6Z6y6NC2hjKmN0Ar4kTpk/edit?usp=sharing

Then run:

$ ./tools/genparse.py inst_file.csv instfrmt_file.csv

@author: Gregory Croisdale <greg@kscript.org>

"""

import argparse
from datetime import datetime

parser = argparse.ArgumentParser(description='Generate comparison code')
parser.add_argument('inst_file', help='csv file of all instructions and their types')
parser.add_argument('instfrmt_file', help='csv file of instruction format information based on type')

args = parser.parse_args()

##

print(f"""/* parse.c - AUTO GENERATED parsing code for RISC-V assembler
 *
 * DO NOT EDIT -- rerun the generator script
 *
 * Generated at {datetime.now()}
 *
 * @author: Gregory Croisdale <greg@kscript.org>
 * @author: Cade Brown <cade@kscript.org>
 */

#include "carve.h"

/* Instruction description */
struct instdesc {{

    /* Name of instruction (lowercase) */
    char* name;
    int nname;

    /* Kind of instruction, 'R', 'I', 'S', ..., 'y' */
    char kind;

    /* Used per different kind of instruction */
    carve_inst opcode, f3, f7;

}};

/* Easy-to-use macros, for pointer parameters */
#define ntoks (*ntoksp)
#define toks (*toksp)
#define toki (*tokip)
#define nback (*nbackp)
#define back (*backp)


#define TOK (toks[toki])
#define EAT() (toks[toki++])

/**  Utils **/

/* Is 'c' a valid digit in base 'b' */
static bool my_isdigit(int c, int b) {{
    if (b == 10) {{
        return '0' <= c && c <= '9';
    }}

    return false;
}}

/* Is 'c' a valid start of an identifier */
static bool my_isident_s(int c) {{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}}

/* Is 'c' a valid middle of an identifier */
static bool my_isident_m(int c) {{
    return my_isdigit(c, 10) || my_isident_s(c);
}}

""")



##


print(f"""

/** Lexer **/

bool carve_lex(const char* fname, const char* src, int* ntoksp, carve_tok** toksp) {{
    int pos = 0, sl = strlen(src);
    int line = 0, col = 0;

    /* Starting positions */
    int spos, sline, scol; 

    /* Advance one character */
    #define ADV() do {{ \\
        char _c = src[pos++]; \\
        if (_c == '\\n') {{ \\
            line++; \\
            col = 0; \\
        }} else {{ \\
            col++; \\
        }} \\
    }} while (0)

    /* Add token */
    #define ADD(_kind) do {{ \\
        int i = ntoks++; \\
        toks = realloc(toks, sizeof(*toks) * ntoks); \\
        toks[i] = (carve_tok) {{ _kind, sline, scol, spos, pos - spos }}; \\
    }} while (0)

    while (pos < sl) {{
        char c = src[pos];
        sline = line;
        scol = col;
        spos = pos;
        if (c == '\\n') {{
            ADV();
            ADD(CARVE_TOK_NEWLINE);
        }} else if (c == ';') {{
            do {{
                ADV();
            }} while (pos < sl && src[pos] != '\\n');
        }} else if (c == ' ' || c == '\\t' || c == '\\r') {{
            ADV();
        }} else if (c == '.') {{
            ADV();
            ADD(CARVE_TOK_DOT);
        }} else if (c == ':') {{
            ADV();
            ADD(CARVE_TOK_COL);
        }} else if (c == ',') {{
            ADV();
            ADD(CARVE_TOK_COM);
        }} else if (c == '+') {{
            ADV();
            ADD(CARVE_TOK_ADD);
        }} else if (c == '-') {{
            ADV();
            ADD(CARVE_TOK_SUB);
        }} else if (c == '(') {{
            ADV();
            ADD(CARVE_TOK_LPAR);
        }} else if (c == ')') {{
            ADV();
            ADD(CARVE_TOK_RPAR);
        }} else if (my_isident_s(c)) {{
            do {{
                ADV();
            }} while (pos < sl && my_isident_m(src[pos]));

            ADD(CARVE_TOK_IDENT);
        }} else if (my_isdigit(c, 10)) {{
            do {{
                ADV();
            }} while (pos < sl && my_isdigit(src[pos], 10));

            ADD(CARVE_TOK_INT);
        }} else {{
            /* Unknown character */
            ADD(CARVE_TOK_IDENT);
            fprintf(stderr, "Unexpected character: '%c'\\n", c);
            carve_printcontext(fname, src, toks[ntoks - 1]);
            return false;
        }}
    }}

    spos = pos;
    sline = line;
    scol = col;
    /* Success */
    ADD(CARVE_TOK_EOF);
    return true;
}}

""")

print(f"""

/** Generated Parser **/

""")


# Dictionary of instructions, sorted by name, with a tuple of:
# (kind, opcode, f3, f7)
# For each value
insts = {}

# Populate
with open(args.inst_file) as fp:
    # Skip first line
    next(fp)
    for line in fp:
        name, kind, opcode, f3, f7 = line.strip().split(',')
        opcode = hex(int('0b' + opcode, 2) if opcode else 0)
        f3 = hex(int('0b' + f3, 2) if f3 else 0)
        f7 = hex(int('0b' + f7, 2) if f7 else 0)
        name = name.lower()
        insts[name] = (kind, opcode, f3, f7)

# Sort according to keys
insts = {k: insts[k] for k in sorted(insts.keys())}


##

print(f"""

/* Database of instructions */

static int I_ninsts = {len(insts)};
static struct instdesc I_insts[{len(insts)}] = {{

""")

for name in insts:
    kind, opcode, f3, f7 = insts[name]
    print (f"""    (struct instdesc) {{ "{name}", {len(name)}, '{kind}', {opcode or 0}, {f3 or 0}, {f7 or 0} }},""")

print(f"""

}};

/* Retrieve instruction information */
static struct instdesc* getinst(char* src, int len) {{
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


##

print(f"""

/* Helper rules */

/* Assume the current token is of the given type, or throw error if not */
static bool parse_skip(carve_prog prog, int* ntoksp, carve_tok** toksp, int* tokip, int kind) {{
    if (TOK.kind == kind) {{
        EAT();
        return true;
    }} else {{
        fprintf(stderr, "Unexpected token\\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return false;
    }}
}}



/* Parse an immediate value, returns -1 if an error was thrown 
 * 'sbit' is the starting bit, 'nbit' is the number of bits
 */
static int parse_imm(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, char kind) {{
    if (TOK.kind == CARVE_TOK_INT) {{
        carve_tok t = EAT();
        int rv = 0;
        int i = 0;
        while (i < t.len) {{
            char d = prog->src[t.pos + i];
            rv = 10 * rv + (d - '0');
            i++;
        }}

        return rv;
    }} else if (TOK.kind == CARVE_TOK_IDENT) {{
        int tokidx = toki;
        carve_tok t = EAT();

        int bi = nback++;
        back = realloc(back, sizeof(*back) * nback);

        back[bi].inst = prog->ninst;
        back[bi].tok = tokidx;
        back[bi].kind = kind;

        return 0xFF;
    }} else {{
        fprintf(stderr, "Unexpected token, expected immediate value (i.e. an integer)\\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return -1;
    }}
}}

/* Parse an integer register, returns -1 if an error was thrown */
static int parse_reg_int(carve_prog prog, int* ntoksp, carve_tok** toksp, int* tokip) {{
    if (TOK.kind == CARVE_TOK_IDENT) {{
        carve_tok t = EAT();
    #define C(_str, _rv) else if (t.len == sizeof(_str) - 1 && strncmp(prog->src + t.pos, _str, t.len) == 0) {{ \\
        return _rv; \\
    }}

        if (false) {{}}

        C("x0", 0)
        C("x1", 1)
        C("x2", 2)
        C("x3", 3)
        C("x4", 4)
        C("x5", 5)
        C("x6", 6)
        C("x7", 7)
        C("x8", 8)
        C("x9", 9)
        C("x10", 10)
        C("x11", 11)
        C("x12", 12)
        C("x13", 13)
        C("x14", 14)
        C("x15", 15)
        C("x16", 16)
        C("x17", 17)
        C("x18", 18)
        C("x19", 19)
        C("x20", 20)
        C("x21", 21)
        C("x22", 22)
        C("x23", 23)
        C("x24", 24)
        C("x25", 25)
        C("x26", 26)
        C("x27", 27)
        C("x28", 28)
        C("x29", 29)
        C("x30", 30)
        C("x31", 31)

        C("zero", 0)
        C("ra", 1)
        C("sp", 2)
        C("gp", 3)
        C("tp", 4)
        C("t0", 5)
        C("t1", 6)
        C("t2", 7)
        C("s0", 8)
        C("fp", 8)
        C("s1", 9)
        C("a0", 10)
        C("a1", 11)
        C("a2", 12)
        C("a3", 13)
        C("a4", 14)
        C("a5", 15)
        C("a6", 16)
        C("a7", 17)
        C("s2", 18)
        C("s3", 19)
        C("s4", 20)
        C("s5", 21)
        C("s6", 22)
        C("s7", 23)
        C("s8", 24)
        C("s9", 25)
        C("s10", 26)
        C("s11", 27)
        C("t3", 28)
        C("t4", 29)
        C("t5", 30)
        C("t6", 31)

        #undef C

        fprintf(stderr, "Unknown register name\\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return -1;
    }} else {{
        fprintf(stderr, "Expected a register name\\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return -1;
    }}
}}


static bool parse_R(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3, int f7) {{
    int rd, rs1, rs2;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((rs2 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeRr(opcode, rd, f3, f7, rs1, rs2));

    return true;
}}

static bool parse_I(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3) {{
    int rd, rs1;
    int imm;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'I')) < 0) {{
        return false;
    }}

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeIr(opcode, rd, f3, rs1, imm));

    return true;
}}

static bool parse_S(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3) {{
    int rs1, rs2;
    int imm;
    if ((rs2 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'S')) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeSr(opcode, f3, rs1, rs2, imm));

    return true;
}}

static bool parse_B(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3) {{
    int rs1, rs2;
    int imm;
    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((rs2 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'B')) < 0) {{
        return false;
    }}

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeBr(opcode, f3, rs1, rs2, imm));

    return true;
}}

static bool parse_U(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode) {{
    int rd;
    int imm;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'U')) < 0) {{
        return false;
    }}

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeUr(opcode, rd, imm));

    return true;
}}

static bool parse_J(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode) {{
    int rd;
    int imm;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {{
        return false;
    }}

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {{
        return false;
    }}

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'J')) < 0) {{
        return false;
    }}

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeJr(opcode, rd, imm));

    return true;
}}



""")




##

print(f"""

bool carve_parse(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp) {{
    /* Current token */

    int i = 0;
    while (i < ntoks - 1) {{
        carve_tok t = toks[i++];
        if (t.kind == CARVE_TOK_IDENT) {{
            if (toks[i].kind == CARVE_TOK_COL) {{ 
                /* Label
                 * <ident>: 
                 */
                 i++;

                char* tmp = malloc(t.len + 1);
                memcpy(tmp, prog->src + t.pos, t.len);
                tmp[t.len] = '\\0';
                carve_prog_label_set(prog, tmp, prog->ninst);
                free(tmp);

            }} else {{
                /* Instruction
                 * <ident> args* \\n
                 */

                struct instdesc* id = getinst(prog->src + t.pos, t.len);
                if (!id) {{
                    fprintf(stderr, "Unknown instruction\\n");
                    carve_printcontext(prog->fname, prog->src, t);
                    return false;
                }}

                if (id->kind == 'R') {{
                    if (!parse_R(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3, id->f7)) {{
                        return false;
                    }}
                }} else if (id->kind == 'I') {{
                    if (!parse_I(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {{
                        return false;
                    }}
                }} else if (id->kind == 'S') {{
                    if (!parse_S(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {{
                        return false;
                    }}
                }} else if (id->kind == 'B') {{
                    if (!parse_B(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {{
                        return false;
                    }}
                }} else if (id->kind == 'U') {{
                    if (!parse_U(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode)) {{
                        return false;
                    }}
                }} else if (id->kind == 'J') {{
                    if (!parse_J(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode)) {{
                        return false;
                    }}
                }} else {{
                    fprintf(stderr, "TODO: We haven't implemented this instruction!\\n");
                    carve_printcontext(prog->fname, prog->src, t);
                    return false;
                }}
            }}
        }} else if (t.kind == CARVE_TOK_DOT) {{
            /* Directive
             */
        }} else if (t.kind == CARVE_TOK_NEWLINE) {{
            /* Skip */
        }} else {{
            /* Unknown */
            fprintf(stderr, "Unexpected token here\\n");
            carve_printcontext(prog->fname, prog->src, t);
            return false;
        }}
    }}

    /* Success */
    return true;
}}

""")
