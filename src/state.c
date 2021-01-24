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

    return self;
}

#define MAXBUF 100

char* carve_state_sri(carve_state self, int xi, int base) {
    assert(base == 10 || base == 16);
    char* res = malloc(MAXBUF + 1);
    if (base == 10) {
        snprintf(res, MAXBUF, "%llu", (long long unsigned int)self->x[xi]);
    } else {
        snprintf(res, MAXBUF, "0x%016llx", (long long unsigned int)(void*)self->x[xi]);
    }
    return res;
}
