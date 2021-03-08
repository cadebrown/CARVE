/* state.c - 'carve_state' object implementation
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"

carve_state carve_state_new() {
    carve_state self = malloc(sizeof(*self));

    int i;
    for (i = 0; i < CARVE_N_REG_INT; ++i) {
        self->x[i] = 0;
    }

    self->easteregg = 0;

    self->nvmem = CARVE_DEFAULT_NVMEM;
    self->vmem = malloc(self->nvmem);

    /* initialize virtual memory */
    for (i = 0; i < self->nvmem; ++i) {
        self->vmem[i] = 0xFF;
    }

    /* Set stack pointer to the top of the virtual memory space */
    self->x[2] = CARVE_VMEM_START + self->nvmem;

    /* Set heap pointer */
    self->hp = CARVE_VMEM_START;

    return self;
}
int carve_easteregg(carve_state s) {
    int res = s->easteregg;
    s->easteregg = 0;
    return res;
}

#define MAXBUF 100

char* carve_state_sri(carve_state self, int xi, int base) {
    assert(base == 10 || base == 16);
    char* res = malloc(MAXBUF + 1);
    if (base == 10) {
        snprintf(res, MAXBUF, "%lld", (long long int)self->x[xi]);
    } else {
        snprintf(res, MAXBUF, "0x%016llx", (long long int)self->x[xi]);
    }
    return res;
}