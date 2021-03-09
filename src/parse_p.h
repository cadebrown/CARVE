/* parse_p.h - Generated file that defines parse modes for RISC-V pseudoinstructions
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT ** 
 */

#include <carve.h>
#ifdef RV32I
  #include "ext/RV32I.h"
#endif /* RV32I */

#define CARVE_parse_p() do {\
  int counter = 0; \
  switch (name[counter++]) { \
    case 'j': \
      switch (name[counter++]) { \
        case '\0': {CARVE_PSEUDO_j(); return true;} break; \
      } break; \
  } \
  \
  fprintf(stderr, "Unsupported pseudoinstruction!\n"); \
  carve_printcontext(prog->fname, prog->src, (*toksp)[*tokip - 1]); \
  return false; } while (0)

