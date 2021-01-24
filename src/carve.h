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


/* Integer register type */
typedef uint64_t carve_int;

/* Single RISC-V instruction, which must be enough to hold 32 bits 
 *
 * 
 */
typedef uint32_t carve_inst;


/** Types **/


/* carve_state - Current emulator state of RISC-V abstract machine
 *
 */
typedef struct carve_state_s {

    /* Integer registers */
    carve_int x[CARVE_N_REG_INT];

}* carve_state;


/* carve_prog - Program to be ran on an emulator
 *
 */
typedef struct carve_prog_s {

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

/* SRI -> Returns a string describing integer register 'xi' in base 'base'
 * NOTE: must call 'free(res)' 
 */
CARVE_API char* carve_state_sri(carve_state self, int xi, int base);

/* Create a new program from a given source string
 *
 */
CARVE_API carve_prog carve_prog_new(const char* src);

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


#endif /* CARVE_H__ */
