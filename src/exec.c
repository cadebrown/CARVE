/* exec.c - RISC-V executor
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"


int carve_exec(carve_state s, carve_prog p) {
    s->pc = (carve_int)p->inst;

    int ct = 0;
    while (ct < 50) {
        carve_execinst(s, *(carve_inst*)s->pc);
        s->pc += sizeof(carve_inst);

        ct++;
    }

    return 0;
}
