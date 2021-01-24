/* prog.c - 'carve_prog' object implementation
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"

carve_prog carve_prog_new(const char* src) {
    carve_prog self = malloc(sizeof(*self));

    int sl = strlen(src);
    self->src = malloc(sl + 1);
    memcpy(self->src, src, sl);
    self->src[sl] = '\0';

    self->ninst = 0;
    self->inst = NULL;

    carve_prog_add(self, carve_makeU(0x37, 0x01, 0xCADE));

    return self;
}

void carve_prog_add(carve_prog self, carve_inst inst) {
    int i = self->ninst++;
    self->inst = realloc(self->inst, sizeof(*self->inst) * self->ninst);

    self->inst[i] = inst;
}

#define MAXBUF 100

char* carve_prog_ihs(carve_prog self, int i) {
    char* res = malloc(MAXBUF);

    snprintf(res, MAXBUF, "%08llx", (long long unsigned int)(void*)self->inst[i]);

    return res;
}


