/* carve.h - header file for the C binding layer of CARVE web application (called libcarve)
 *
 * For more information, see: https://github.com/ChemicalDevelopment/CARVE
 * 
 * SEE: https://en.wikipedia.org/wiki/RISC-V#Design
 * 
 * @author: Cade Brown <cade@utk.edu>
 */

#pragma once
#ifndef CARVE_H__
#define CARVE_H__

/** Config **/

#ifndef __EMSCRIPTEN__
#error libcarve must be built with 'emcc'
#endif


/** Includes **/

/* C std */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <assert.h>


/* Emscripten API */
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

/* Define used before any exported symbols */
#define CARVE_API EMSCRIPTEN_KEEPALIVE
#else

#define CARVE_API

#endif






/** Constants **/


/* Number of integer registers */
#define CARVE_N_REG_INT 32

/* Token kinds */
enum {
    CARVE_TOK_EOF = 0,

    CARVE_TOK_IDENT,
    CARVE_TOK_INT,
    CARVE_TOK_FLOAT,

    /* String literal */
    CARVE_TOK_STR,

    /* Character literal */
    CARVE_TOK_CHR,

    CARVE_TOK_NEWLINE,

    CARVE_TOK_LPAR,
    CARVE_TOK_RPAR,

    CARVE_TOK_COL,
    CARVE_TOK_DOT,
    CARVE_TOK_COM,
    CARVE_TOK_ADD,
    CARVE_TOK_SUB

};



/** Types **/


/* Single byte, half, word, and doubleword types */
typedef uint8_t  carve_b;
typedef uint16_t carve_h;
typedef uint32_t carve_w;
typedef uint64_t carve_d;

/* Integer register type */
typedef uint64_t carve_int;
typedef  int64_t carve_sint;

/* Single RISC-V instruction, which must be enough to hold 32 bits 
 *
 * 
 */
typedef uint32_t carve_inst;



/* carve_state - Current emulator state of RISC-V abstract machine
 *
 */
typedef struct carve_state_s {

    /* Integer registers */
    carve_int x[CARVE_N_REG_INT];

    /* Program counter */
    carve_int pc;

}* carve_state;


/* Source code token
 *
 */
typedef struct {

    /* Kind of token (see 'CARVE_TOK_*') */
    int kind;

    /* Line and column (0 indexed) */
    int line, col;

    /* Position and length (in bytes) */
    int pos, len;

} carve_tok;


/* carve_prog - Program to be ran on an emulator
 *
 */
typedef struct carve_prog_s {
    
    /* Filename */
    char* fname;

    /* Source string used to generate the program */
    char* src;

    /* Number of instruction */
    int ninst;

    /* Instructions array */
    carve_inst* inst;

    /* Hash table of fabled labels cradled with gradled sables */
    /* (hash table of labels in 'inst') */

    /* Number of entries in the hash table (some may be empty) */
    int nhtl;

    /* Number of non-NULL entries in the hash table */
    int nhtlr;

    /* Array of entries in the hash table 
     */
    struct carve_htl {
        /* Allocated string of the hash table name 
         * If NULL, then this entry is empty
         */
        char* key;

        /* hash(key) */
        int hash;

        /* Value of the label, which is an index into 'inst' */
        int val;

    }* htl;

}* carve_prog;

/** Functions **/


/** Instruction Encoding/Decoding **/


/* Mask bits '_i' (inclusive) through '_j' (exclusive) */
#define CARVE_MASK(_i, _j) (carve_inst)(((1ULL << ((_j) - (_i))) - 1) << (_i))

/* Treat '_val' as a field, and shift it and format to start at '_i' (inclusive) and end at '_j' (exclusive) */
#define CARVE_FIELD(_val, _i, _j) (carve_inst)(((_val) & CARVE_MASK(0, (_j) - (_i))) << (_i))

/* Sign extent '_val', if the '_sb'th bit is set (the sign bit) */
#define CARVE_SEXT(_val, _sb) (carve_sint)(((_val) & (1ULL << (_sb))) ? (~CARVE_MASK(0, _sb) | (_val)) : (_val))


/* Make an instruction, from one of many families:
 *
 *   makeR: register/register
 *   makeI: Immediate
 *   makeU: Upper immediate
 *   makeS: Store
 *   makeB: Branch
 *   makeJ: Jump
 */
CARVE_API carve_inst carve_makeR(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst funct7);
CARVE_API carve_inst carve_makeI(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst rs1, carve_inst imm_11_0);
CARVE_API carve_inst carve_makeU(carve_inst opcode, carve_inst rd, carve_inst imm_31_12);
CARVE_API carve_inst carve_makeS(carve_inst opcode, carve_inst imm_4_0, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm_11_5);
CARVE_API carve_inst carve_makeB(carve_inst opcode, carve_inst _11, carve_inst imm_4_1, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm_10_5, carve_inst _12);
CARVE_API carve_inst carve_makeJ(carve_inst opcode, carve_inst rd, carve_inst imm_19_12, carve_inst _11, carve_inst imm_10_1, carve_inst _20);

CARVE_API carve_inst carve_makeRr(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst funct7, carve_inst rs1, carve_inst rs2);
CARVE_API carve_inst carve_makeIr(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst rs1, carve_inst imm);
CARVE_API carve_inst carve_makeUr(carve_inst opcode, carve_inst rd, carve_inst imm);
CARVE_API carve_inst carve_makeJr(carve_inst opcode, carve_inst rd, carve_inst imm);
CARVE_API carve_inst carve_makeSr(carve_inst opcode, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm);
CARVE_API carve_inst carve_makeBr(carve_inst opcode, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm);

/* Replace 'inst's immediate with 'imm', and return it
 * Suffix (I, U, J, ...) is the type of instruction 'inst' is
 */
CARVE_API carve_inst carve_newimmI(carve_inst inst, carve_inst imm);
CARVE_API carve_inst carve_newimmU(carve_inst inst, carve_inst imm);
CARVE_API carve_inst carve_newimmJ(carve_inst inst, carve_inst imm);
CARVE_API carve_inst carve_newimmS(carve_inst inst, carve_inst imm);
CARVE_API carve_inst carve_newimmB(carve_inst inst, carve_inst imm);

/* Decode specific field from bit '_i' (inclusive) to '_j' (exclusive)
 */
#define CARVE_DEC_FIELD(_inst, _to, _i, _j) do { \
    (_to) = ((_inst) & CARVE_MASK(_i, _j)) >> (_i); \
} while (0);

/* Decode instruction, inverse of 'carve_make*'
 *
 */
#define CARVE_DEC_R(_inst, _opcode, _rd, _funct3, _rs1, _rs2, _funct7) do { \
    CARVE_DEC_FIELD(_inst, _opcode, 0, 7) \
    CARVE_DEC_FIELD(_inst, _rd, 7, 12) \
    CARVE_DEC_FIELD(_inst, _funct3, 12, 15) \
    CARVE_DEC_FIELD(_inst, _rs1, 15, 20) \
    CARVE_DEC_FIELD(_inst, _rs2, 20, 25) \
    CARVE_DEC_FIELD(_inst, _funct7, 25, 32) \
} while (0)

#define CARVE_DEC_I(_inst, _opcode, _rd, _funct3, _rs1, _imm_11_0) do { \
    CARVE_DEC_FIELD(_inst, _opcode, 0, 7) \
    CARVE_DEC_FIELD(_inst, _rd, 7, 12) \
    CARVE_DEC_FIELD(_inst, _funct3, 12, 15) \
    CARVE_DEC_FIELD(_inst, _rs1, 15, 20) \
    CARVE_DEC_FIELD(_inst, _imm_11_0, 20, 32) \
} while (0)

#define CARVE_DEC_U(_inst, _opcode, _rd, _imm_31_12) do { \
    CARVE_DEC_FIELD(_inst, _opcode, 0, 7) \
    CARVE_DEC_FIELD(_inst, _rd, 7, 12) \
    CARVE_DEC_FIELD(_inst, _imm_31_12, 12, 32) \
} while (0)

#define CARVE_DEC_S(_inst, _opcode, _imm_4_0, _funct3, _rs1, _rs2, _imm_11_5) do { \
    CARVE_DEC_FIELD(_inst, _opcode, 0, 7) \
    CARVE_DEC_FIELD(_inst, _imm_4_0, 7, 12) \
    CARVE_DEC_FIELD(_inst, _funct3, 12, 15) \
    CARVE_DEC_FIELD(_inst, _rs1, 15, 20) \
    CARVE_DEC_FIELD(_inst, _rs2, 20, 25) \
    CARVE_DEC_FIELD(_inst, _imm_11_5, 25, 32) \
} while (0)

#define CARVE_DEC_B(_inst, _opcode, __11, _imm_4_1, _funct3, _rs1, _rs2, _imm_10_5, __12) do { \
    CARVE_DEC_FIELD(_inst, _opcode, 0, 7) \
    CARVE_DEC_FIELD(_inst, __11, 7, 8) \
    CARVE_DEC_FIELD(_inst, _imm_4_1, 8, 12) \
    CARVE_DEC_FIELD(_inst, _funct3, 12, 15) \
    CARVE_DEC_FIELD(_inst, _rs1, 15, 20) \
    CARVE_DEC_FIELD(_inst, _rs2, 20, 25) \
    CARVE_DEC_FIELD(_inst, _imm_10_5, 25, 31) \
    CARVE_DEC_FIELD(_inst, __12, 31, 32) \
} while (0)

#define CARVE_DEC_J(_inst, _opcode, _rd, _imm_19_12, __11, _imm_10_1, __20) do { \
    CARVE_DEC_FIELD(_inst, _opcode, 0, 7) \
    CARVE_DEC_FIELD(_inst, _rd, 7, 12) \
    CARVE_DEC_FIELD(_inst, _imm_19_12, 12, 20) \
    CARVE_DEC_FIELD(_inst, __11, 20, 21) \
    CARVE_DEC_FIELD(_inst, _imm_10_1, 21, 31) \
    CARVE_DEC_FIELD(_inst, __20, 31, 32) \
} while (0)


/* Returns tne opcode associated with an instruction
 *
 */
CARVE_API carve_inst carve_get_opcode(carve_inst inst);

/* Initialize the libcarve library
 * NOTE: This should be called before any other 'carve_*()' calls
 */
CARVE_API void carve_init();

/* Create a new 'carve_state', with everything initialized to empty
 */
CARVE_API carve_state carve_state_new();

/* Execute a program
 */
CARVE_API int carve_exec(carve_state s, carve_prog p);

/* SRI -> Returns a string describing integer register 'xi' in base 'base'
 * NOTE: must call 'free(res)' 
 */
CARVE_API char* carve_state_sri(carve_state self, int xi, int base);

/* Create a new program from a given source string
 *
 */
CARVE_API carve_prog carve_prog_new(const char* fname, const char* src);

/* Add instruction
 */
CARVE_API void carve_prog_add(carve_prog self, carve_inst inst);

/* IHS - Instruction Hex String of instruction 'i'
 */
CARVE_API char* carve_prog_ihs(carve_prog self, int i);


/* Adds a label named 'key' to 'inst', and return whether it already existed
 */
CARVE_API bool carve_prog_label_set(carve_prog self, const char* key, int inst);

/* Get the label named 'key', and return the index into the 'inst' array
 * If < 0, then there was no label named 'key'
 */
CARVE_API int carve_prog_label_get(carve_prog self, const char* key);


/* Prints the context around a given token
 */
CARVE_API int carve_printcontext(const char* fname, const char* src, carve_tok tok);


/* Turns a filename and source into a list of tokens, returns success
 */
CARVE_API bool carve_lex(const char* fname, const char* src, int* ntoksp, carve_tok** toksp);

/* Backpatch structure */
struct carve_backpatch {
    /* Instruction index */
    int inst;

    /* Instruction kind */
    char kind;

    /* Token index */
    int tok;
};

/* Parses a stream of tokens
 * 'nback' and 'back' are an array of instruction indicies that must be backpatched
 */
CARVE_API bool carve_parse(carve_prog prog, int* ntoksp, carve_tok** toksp, int* nbackp, struct carve_backpatch** backp);


#endif /* CARVE_H__ */
