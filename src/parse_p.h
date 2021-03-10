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
    case 'n': \
      switch (name[counter++]) { \
        case 'o': \
          switch (name[counter++]) { \
            case 'p': \
              switch (name[counter++]) { \
                case '\0': {CARVE_PSEUDO_nop(); return true;} break; \
              } break; \
            case 't': \
              switch (name[counter++]) { \
                case '\0': {CARVE_PSEUDO_not(); return true;} break; \
              } break; \
          } break; \
        case 'e': \
          switch (name[counter++]) { \
            case 'g': \
              switch (name[counter++]) { \
                case '\0': {CARVE_PSEUDO_neg(); return true;} break; \
              } break; \
          } break; \
      } break; \
    case 'l': \
      switch (name[counter++]) { \
        case 'i': \
          switch (name[counter++]) { \
            case '\0': {CARVE_PSEUDO_li(); return true;} break; \
          } break; \
      } break; \
    case 'm': \
      switch (name[counter++]) { \
        case 'v': \
          switch (name[counter++]) { \
            case '\0': {CARVE_PSEUDO_mv(); return true;} break; \
          } break; \
      } break; \
    case 's': \
      switch (name[counter++]) { \
        case 'e': \
          switch (name[counter++]) { \
            case 'q': \
              switch (name[counter++]) { \
                case 'z': \
                  switch (name[counter++]) { \
                    case '\0': {CARVE_PSEUDO_seqz(); return true;} break; \
                  } break; \
              } break; \
          } break; \
        case 'n': \
          switch (name[counter++]) { \
            case 'e': \
              switch (name[counter++]) { \
                case 'z': \
                  switch (name[counter++]) { \
                    case '\0': {CARVE_PSEUDO_snez(); return true;} break; \
                  } break; \
              } break; \
          } break; \
        case 'l': \
          switch (name[counter++]) { \
            case 't': \
              switch (name[counter++]) { \
                case 'z': \
                  switch (name[counter++]) { \
                    case '\0': {CARVE_PSEUDO_sltz(); return true;} break; \
                  } break; \
              } break; \
          } break; \
        case 'g': \
          switch (name[counter++]) { \
            case 't': \
              switch (name[counter++]) { \
                case 'z': \
                  switch (name[counter++]) { \
                    case '\0': {CARVE_PSEUDO_sgtz(); return true;} break; \
                  } break; \
              } break; \
          } break; \
      } break; \
  } \
  \
  fprintf(stderr, "Unsupported pseudoinstruction!\n"); \
  carve_printcontext(prog->fname, prog->src, (*toksp)[*tokip - 1]); \
  return false; } while (0)

