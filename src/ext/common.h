/* ext/common.h - Common header defining utilities for all RISC-V extensions
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#ifndef EXT_COMMON_H__
#define EXT_COMMON_H__

/* Type definitions */
#include <carve.h>


/* Magic variable that should be defined 
 */
#define STATE s

/* Signed integer register '_x' */
#define REGS(_x) (((carve_sint*)(STATE)->x)[_x])

/* Unsigned integer register '_x' */
#define REGU(_x) ((STATE)->x[_x])

/* Program counter */
#define PC ((STATE)->pc)

/* Halt the program */
#define HALT(_msg) do { \
    if (_msg != NULL) { \
        fprintf(stderr, "%s", _msg); \
    } \
    (STATE)->is_halted = true; \
} while (0)

/* Converts virtual address to real address in C */
#define V2R(_val) (STATE->vmem + ((_val) - CARVE_VMEM_START))


#endif /* EXT_COMMON_H__ */
