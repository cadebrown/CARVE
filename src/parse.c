/* parse.c - AUTO GENERATED parsing code for RISC-V assembler
 *
 * DO NOT EDIT -- rerun the generator script
 *
 * Generated at 2021-02-01 18:20:45.100661
 *
 * @author: Gregory Croisdale <greg@kscript.org>
 * @author: Cade Brown <cade@kscript.org>
 */

#include "carve.h"

/* Instruction description */
struct instdesc {

    /* Name of instruction (lowercase) */
    char* name;
    int nname;

    /* Kind of instruction, 'R', 'I', 'S', ..., 'y' */
    char kind;

    /* Used per different kind of instruction */
    carve_inst opcode, f3, f7;

};

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
static bool my_isdigit(int c, int b) {
    if (b == 10) {
        return '0' <= c && c <= '9';
    }

    return false;
}

/* Is 'c' a valid start of an identifier */
static bool my_isident_s(int c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

/* Is 'c' a valid middle of an identifier */
static bool my_isident_m(int c) {
    return my_isdigit(c, 10) || my_isident_s(c);
}




/** Lexer **/

bool carve_lex(const char* fname, const char* src, int* ntoksp, carve_tok** toksp) {
    int pos = 0, sl = strlen(src);
    int line = 0, col = 0;

    /* Starting positions */
    int spos, sline, scol; 

    /* Advance one character */
    #define ADV() do { \
        char _c = src[pos++]; \
        if (_c == '\n') { \
            line++; \
            col = 0; \
        } else { \
            col++; \
        } \
    } while (0)

    /* Add token */
    #define ADD(_kind) do { \
        int i = ntoks++; \
        toks = realloc(toks, sizeof(*toks) * ntoks); \
        toks[i] = (carve_tok) { _kind, sline, scol, spos, pos - spos }; \
    } while (0)

    while (pos < sl) {
        char c = src[pos];
        sline = line;
        scol = col;
        spos = pos;
        if (c == '\n') {
            ADV();
            ADD(CARVE_TOK_NEWLINE);
        } else if (c == ';') {
            do {
                ADV();
            } while (pos < sl && src[pos] != '\n');
        } else if (c == ' ' || c == '\t' || c == '\r') {
            ADV();
        } else if (c == '.') {
            ADV();
            ADD(CARVE_TOK_DOT);
        } else if (c == ':') {
            ADV();
            ADD(CARVE_TOK_COL);
        } else if (c == ',') {
            ADV();
            ADD(CARVE_TOK_COM);
        } else if (c == '+') {
            ADV();
            ADD(CARVE_TOK_ADD);
        } else if (c == '-') {
            ADV();
            ADD(CARVE_TOK_SUB);
        } else if (c == '(') {
            ADV();
            ADD(CARVE_TOK_LPAR);
        } else if (c == ')') {
            ADV();
            ADD(CARVE_TOK_RPAR);
        } else if (my_isident_s(c)) {
            do {
                ADV();
            } while (pos < sl && my_isident_m(src[pos]));

            ADD(CARVE_TOK_IDENT);
        } else if (my_isdigit(c, 10)) {
            do {
                ADV();
            } while (pos < sl && my_isdigit(src[pos], 10));

            ADD(CARVE_TOK_INT);
        } else {
            /* Unknown character */
            ADD(CARVE_TOK_IDENT);
            fprintf(stderr, "Unexpected character: '%c'\n", c);
            carve_printcontext(fname, src, toks[ntoks - 1]);
            return false;
        }
    }

    spos = pos;
    sline = line;
    scol = col;
    /* Success */
    ADD(CARVE_TOK_EOF);
    return true;
}




/** Generated Parser **/




/* Database of instructions */

static int I_ninsts = 40;
static struct instdesc I_insts[40] = {


    (struct instdesc) { "add", 3, 'R', 0x33, 0x0, 0x0 },
    (struct instdesc) { "addi", 4, 'I', 0x13, 0x0, 0x0 },
    (struct instdesc) { "and", 3, 'R', 0x33, 0x7, 0x0 },
    (struct instdesc) { "andi", 4, 'I', 0x13, 0x7, 0x0 },
    (struct instdesc) { "auipc", 5, 'U', 0x17, 0x0, 0x0 },
    (struct instdesc) { "beq", 3, 'B', 0x63, 0x0, 0x0 },
    (struct instdesc) { "bge", 3, 'B', 0x63, 0x5, 0x0 },
    (struct instdesc) { "bgeu", 4, 'B', 0x63, 0x7, 0x0 },
    (struct instdesc) { "blt", 3, 'B', 0x63, 0x4, 0x0 },
    (struct instdesc) { "bltu", 4, 'B', 0x63, 0x6, 0x0 },
    (struct instdesc) { "bne", 3, 'B', 0x63, 0x1, 0x0 },
    (struct instdesc) { "ebreak", 6, 'y', 0x0, 0x0, 0x0 },
    (struct instdesc) { "ecall", 5, 'y', 0x0, 0x0, 0x0 },
    (struct instdesc) { "fence", 5, 'y', 0x0, 0x0, 0x0 },
    (struct instdesc) { "jal", 3, 'J', 0x6f, 0x0, 0x0 },
    (struct instdesc) { "jalr", 4, 'I', 0x67, 0x0, 0x0 },
    (struct instdesc) { "lb", 2, 'I', 0x3, 0x0, 0x0 },
    (struct instdesc) { "lbu", 3, 'I', 0x3, 0x4, 0x0 },
    (struct instdesc) { "lh", 2, 'I', 0x3, 0x1, 0x0 },
    (struct instdesc) { "lhu", 3, 'I', 0x3, 0x5, 0x0 },
    (struct instdesc) { "lui", 3, 'U', 0x37, 0x0, 0x0 },
    (struct instdesc) { "lw", 2, 'I', 0x3, 0x2, 0x0 },
    (struct instdesc) { "or", 2, 'R', 0x33, 0x6, 0x0 },
    (struct instdesc) { "ori", 3, 'I', 0x13, 0x6, 0x0 },
    (struct instdesc) { "sb", 2, 'S', 0x23, 0x0, 0x0 },
    (struct instdesc) { "sh", 2, 'S', 0x23, 0x1, 0x0 },
    (struct instdesc) { "sll", 3, 'R', 0x33, 0x1, 0x0 },
    (struct instdesc) { "slli", 4, 'I', 0x13, 0x1, 0x0 },
    (struct instdesc) { "slt", 3, 'R', 0x33, 0x2, 0x0 },
    (struct instdesc) { "slti", 4, 'I', 0x13, 0x2, 0x0 },
    (struct instdesc) { "sltiu", 5, 'I', 0x13, 0x3, 0x0 },
    (struct instdesc) { "sltu", 4, 'R', 0x33, 0x3, 0x0 },
    (struct instdesc) { "sra", 3, 'R', 0x33, 0x5, 0x10 },
    (struct instdesc) { "srai", 4, 'I', 0x93, 0x5, 0x0 },
    (struct instdesc) { "srl", 3, 'R', 0x33, 0x5, 0x0 },
    (struct instdesc) { "srli", 4, 'R', 0x13, 0x5, 0x0 },
    (struct instdesc) { "sub", 3, 'R', 0x33, 0x0, 0x20 },
    (struct instdesc) { "sw", 2, 'S', 0x23, 0x2, 0x0 },
    (struct instdesc) { "xor", 3, 'R', 0x33, 0x4, 0x0 },
    (struct instdesc) { "xori", 4, 'I', 0x13, 0x4, 0x0 },


};

/* Retrieve instruction information */
static struct instdesc* getinst(char* src, int len) {
    int l = 0, r = I_ninsts - 1, m;
    while (l <= r) {
        m = (l + r) / 2;

        /* Key we are comparing to */
        int nname = I_insts[m].nname;
        char* name = I_insts[m].name;

        int cmp = 0;
        
        if (len == nname) {
            cmp = strncmp(src, name, nname);
        } else {
            int minl = len < nname ? len : nname;
            cmp = strncmp(src, name, minl);
            if (cmp == 0) {
                cmp = len - nname;
            }
        }

        if (cmp == 0) {
            return &I_insts[m];
        } else if (cmp > 0) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }

    /* Not-found */
    return NULL;
}





/* Helper rules */

/* Assume the current token is of the given type, or throw error if not */
static bool parse_skip(carve_prog prog, int* ntoksp, carve_tok** toksp, int* tokip, int kind) {
    if (TOK.kind == kind) {
        EAT();
        return true;
    } else {
        fprintf(stderr, "Unexpected token\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return false;
    }
}



/* Parse an immediate value, returns -1 if an error was thrown 
 * 'sbit' is the starting bit, 'nbit' is the number of bits
 */
static int parse_imm(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, char kind) {
    if (TOK.kind == CARVE_TOK_INT) {
        carve_tok t = EAT();
        int rv = 0;
        int i = 0;
        while (i < t.len) {
            char d = prog->src[t.pos + i];
            rv = 10 * rv + (d - '0');
            i++;
        }

        return rv;
    } else if (TOK.kind == CARVE_TOK_IDENT) {
        int tokidx = toki;
        carve_tok t = EAT();

        int bi = nback++;
        back = realloc(back, sizeof(*back) * nback);

        back[bi].inst = prog->ninst;
        back[bi].tok = tokidx;
        back[bi].kind = kind;

        return 0xFF;
    } else {
        fprintf(stderr, "Unexpected token, expected immediate value (i.e. an integer)\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return -1;
    }
}

/* Parse an integer register, returns -1 if an error was thrown */
static int parse_reg_int(carve_prog prog, int* ntoksp, carve_tok** toksp, int* tokip) {
    if (TOK.kind == CARVE_TOK_IDENT) {
        carve_tok t = EAT();
    #define C(_str, _rv) else if (t.len == sizeof(_str) - 1 && strncmp(prog->src + t.pos, _str, t.len) == 0) { \
        return _rv; \
    }

        if (false) {}

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

        fprintf(stderr, "Unknown register name\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return -1;
    } else {
        fprintf(stderr, "Expected a register name\n");
        carve_printcontext(prog->fname, prog->src, TOK);
        return -1;
    }
}


static bool parse_R(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3, int f7) {
    int rd, rs1, rs2;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((rs2 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeRr(opcode, rd, f3, f7, rs1, rs2));

    return true;
}

static bool parse_I(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3) {
    int rd, rs1;
    int imm;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'I')) < 0) {
        return false;
    }

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeIr(opcode, rd, f3, rs1, imm));

    return true;
}

static bool parse_S(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3) {
    int rs1, rs2;
    int imm;
    if ((rs2 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'S')) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeSr(opcode, f3, rs1, rs2, imm));

    return true;
}

static bool parse_B(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode, int f3) {
    int rs1, rs2;
    int imm;
    if ((rs1 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((rs2 = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'B')) < 0) {
        return false;
    }

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeBr(opcode, f3, rs1, rs2, imm));

    return true;
}

static bool parse_U(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode) {
    int rd;
    int imm;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'U')) < 0) {
        return false;
    }

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeUr(opcode, rd, imm));

    return true;
}

static bool parse_J(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, int opcode) {
    int rd;
    int imm;
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) < 0) {
        return false;
    }

    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) {
        return false;
    }

    if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'J')) < 0) {
        return false;
    }

    /* Add assembled instruction */
    carve_prog_add(prog, carve_makeJr(opcode, rd, imm));

    return true;
}






bool carve_parse(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp) {
    /* Current token */

    int i = 0;
    while (i < ntoks - 1) {
        carve_tok t = toks[i++];
        if (t.kind == CARVE_TOK_IDENT) {
            if (toks[i].kind == CARVE_TOK_COL) { 
                /* Label
                 * <ident>: 
                 */
                 i++;

                char* tmp = malloc(t.len + 1);
                memcpy(tmp, prog->src + t.pos, t.len);
                tmp[t.len] = '\0';
                carve_prog_label_set(prog, tmp, prog->ninst);
                free(tmp);

            } else {
                /* Instruction
                 * <ident> args* \n
                 */

                struct instdesc* id = getinst(prog->src + t.pos, t.len);
                if (!id) {
                    fprintf(stderr, "Unknown instruction\n");
                    carve_printcontext(prog->fname, prog->src, t);
                    return false;
                }

                if (id->kind == 'R') {
                    if (!parse_R(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3, id->f7)) {
                        return false;
                    }
                } else if (id->kind == 'I') {
                    if (!parse_I(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {
                        return false;
                    }
                } else if (id->kind == 'S') {
                    if (!parse_S(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {
                        return false;
                    }
                } else if (id->kind == 'B') {
                    if (!parse_B(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {
                        return false;
                    }
                } else if (id->kind == 'U') {
                    if (!parse_U(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode)) {
                        return false;
                    }
                } else if (id->kind == 'J') {
                    if (!parse_J(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode)) {
                        return false;
                    }
                } else {
                    fprintf(stderr, "TODO: We haven't implemented this instruction!\n");
                    carve_printcontext(prog->fname, prog->src, t);
                    return false;
                }
            }
        } else if (t.kind == CARVE_TOK_DOT) {
            /* Directive
             */
        } else if (t.kind == CARVE_TOK_NEWLINE) {
            /* Skip */
        } else {
            /* Unknown */
            fprintf(stderr, "Unexpected token here\n");
            carve_printcontext(prog->fname, prog->src, t);
            return false;
        }
    }

    /* Success */
    return true;
}


