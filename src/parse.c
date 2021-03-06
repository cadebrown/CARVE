/* parse.c - parsing code for RISC-V assembler
 *
 * @author: Gregory Croisdale <greg@kscript.org>
 * @author: Cade Brown <cade@kscript.org>
 */

#include "carve.h"


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
    } else if (b == 16) {
        return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
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




/** Lexer (splits into tokens) **/
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
        switch (c) {
            /* match comments */
            case '#': {
                do {
                    ADV();
                } while (pos < sl && src[pos] != '\n');
            } break;

            /* ignore whitespace */
            case ' ': case '\t': case '\r': {
                ADV();
            } break;

            /* single char matching */
            case '\n': {
                ADV();
                ADD(CARVE_TOK_NEWLINE);
            } break;
            case '.': {
                ADV();
                ADD(CARVE_TOK_DOT);
            } break;
            case ':': {
                ADV();
                ADD(CARVE_TOK_COL);
            } break;
            case ',': {
                ADV();
                ADD(CARVE_TOK_COM);
            } break;
            case '+': {
                ADV();
                ADD(CARVE_TOK_ADD);
            } break;
            case '-': {
                ADV();
                ADD(CARVE_TOK_SUB);
            } break;
            case '(': {
                ADV();
                ADD(CARVE_TOK_LPAR);
            } break;
            case ')': {
                ADV();
                ADD(CARVE_TOK_RPAR);
            } break;

            /* ident / const matching */
            default: {
                if (my_isident_s(c)) {
                    /* Match regex:
                    * [a-zA-Z_][a-zA-Z0-9_]*
                    */
                    do {
                        ADV();
                    } while (pos < sl && my_isident_m(src[pos]));

                    ADD(CARVE_TOK_IDENT);
                } else if (my_isdigit(c, 10)) {
                    /* Match regex:
                    * [0-9]+
                    * 0[xX][0-9a-fA-F]+
                    */

                    int base = 10;

                    /* 0[xX] prefix for base 16 */
                    if (pos+1 < sl && src[pos+1] == 'x' || src[pos+1] == 'X') {
                        ADV();
                        ADV();
                        base = 16;
                    }

                    do {
                        ADV();
                    } while (pos < sl && my_isdigit(src[pos], base));

                    ADD(CARVE_TOK_INT);
                } else {
                    /* Unknown character */
                    ADD(CARVE_TOK_IDENT);
                    fprintf(stderr, "Unexpected character: '%c'\n", c);
                    carve_printcontext(fname, src, toks[ntoks - 1]);
                    return false;
                }
            } break;
        }
    }

    spos = pos;
    sline = line;
    scol = col;
    /* Success */
    ADD(CARVE_TOK_EOF);
    return true;
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
        carve_printcontext(prog->fname, prog->src, t);
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
    carve_prog_add(prog, carve_makeR(opcode, f3, f7, rd, rs1, rs2));

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
    carve_prog_add(prog, carve_makeI(opcode, f3, rd, rs1, imm));

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
    carve_prog_add(prog, carve_makeS(opcode, f3, rs1, rs2, imm));

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
    carve_prog_add(prog, carve_makeB(opcode, f3, rs1, rs2, imm));

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
    carve_prog_add(prog, carve_makeU(opcode, rd, imm));

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
    carve_prog_add(prog, carve_makeJ(opcode, rd, imm));

    return true;
}

static bool parse_y(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, const char* name, int opcode) {
    if (strcmp(name, "ecall") == 0) {
        /* ecall # syscall */
        carve_prog_add(prog, carve_makeI(opcode, 0, 0, 0, 0 /* only thing that matters */));
    } else {
        fprintf(stderr, "Unsupported instruction!\n");
        carve_printcontext(prog->fname, prog->src, toks[toki - 1]);
        return false;
    }

    return true;
}

static bool parse_p(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp, int* tokip, const char* name) {
    if (*name == 'j') {
        /* j offset -> jal x0, offset */
        int imm;
        if ((imm = parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'J')) < 0) {
            return false;
        }
        carve_prog_add(prog, carve_makeJ(carve_getinst("jal", -1)->opcode, 0, imm));
    } else {
        fprintf(stderr, "Unsupported pseudoinstruction!\n");
        carve_printcontext(prog->fname, prog->src, toks[toki - 1]);
        return false;
    }

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

                struct carve_instdesc* id = carve_getinst(prog->src + t.pos, t.len);
                if (!id) {
                    fprintf(stderr, "Unknown instruction\n");
                    carve_printcontext(prog->fname, prog->src, t);
                    return false;
                }

                switch (id->kind) {
                    case 'R': {
                        if (!parse_R(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3, id->f7)) {
                            return false;
                        }
                    } break;
                    case 'I': {
                        if (!parse_I(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {
                            return false;
                        }
                    } break;
                    case 'S': {
                        if (!parse_S(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {
                            return false;
                        }
                    } break;
                    case 'B': {
                        if (!parse_B(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode, id->f3)) {
                            return false;
                        }
                    } break;
                    case 'U': {
                        if (!parse_U(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode)) {
                            return false;
                        }
                    } break;
                    case 'J': {
                        if (!parse_J(prog, ntoksp, toksp, nbackp, backp, &i, id->opcode)) {
                            return false;
                        }
                    } break;
                    case 'p': {
                        if (!parse_p(prog, ntoksp, toksp, nbackp, backp, &i, id->name)) {
                            return false;
                        }
                    } break;
                    case 'y': {
                        if (!parse_y(prog, ntoksp, toksp, nbackp, backp, &i, id->name, id->opcode)) {
                            return false;
                        }
                    } break;
                    default: {
                        fprintf(stderr, "TODO: Instruction type not implemented!\n");
                        carve_printcontext(prog->fname, prog->src, t);
                        return false;
                    } break;
                }
                /* Special cases */
                if (strcmp(id->name, "srai") == 0) {
                    /* See: https://stackoverflow.com/questions/39489318/risc-v-implementing-slli-srli-and-srai */
                    /* We need to set the 30th bit */
                    prog->inst[prog->ninst - 1] |= 1ULL << 30;
                }
            }

        } else if (t.kind == CARVE_TOK_DOT) {
            /* Directive */
        } else if (t.kind == CARVE_TOK_NEWLINE) {
            /* Skip */
        } else {
            /* Unknown */
            fprintf(stderr, "Unexpected token here\n");
            carve_printcontext(prog->fname, prog->src, t);
            return false;
        }
    }

    /* Add ebreak */
    carve_prog_add(prog, carve_makeI(0x73, 0, 0, 0, 1 /* ebreak */));

    /* Success */
    return true;
}


