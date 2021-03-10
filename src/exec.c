/* exec.c - RISC-V executor
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"


int carve_exec(carve_state s, carve_prog p) {
    s->is_halted = false;
    s->pc = (carve_int)p->inst;

    while (!s->is_halted) {
        carve_execinst(s, *(carve_inst*)s->pc);
        s->x[0] = 0;
        s->pc += sizeof(carve_inst);
    }

    return 0;
}
