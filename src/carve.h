/* carve.h - header file for the C binding layer of CARVE web application (called libcarve)
 *
 * For more information, see: https://github.com/ChemicalDevelopment/CARVE
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
#include <string.h>
#include <limits.h>
#include <assert.h>


/* Emscripten API */
#include <emscripten.h>



/* Define used before any exported symbols */
#define CARVE_API EMSCRIPTEN_KEEPALIVE


/** Constants **/


#define CARVE_N_REG_INT 32


/* Integer register type */
typedef uint64_t carve_int;



/** Types **/


/* carve_state - Current emulator state of RISC-V abstract machine
 *
 */
typedef struct carve_state_s {

    /* Integer registers */
    carve_int x[CARVE_N_REG_INT];

}* carve_state;


/** Functions **/

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




#endif /* CARVE_H__ */
