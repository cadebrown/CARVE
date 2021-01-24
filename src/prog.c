/* prog.c - 'carve_prog' object implementation
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"


/* Internals */


/* Hash function for a NUL-terminated string */
static int my_hash(const char* src) {
    int res = 5831;
    while (*src) {
        res = res * 31 + *src++;
    } 
    return res;
}

/* Determine if 'x' is prime */
static bool my_isprime(int x) {
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;

    int i;
    for (i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }

    return true;
}

static void my_resize(carve_prog self) {
    
    /* Capture olds */
    int nold = self->nhtl;
    struct carve_htl* old = self->htl;

    /* Get next prime above 2 * N */
    self->nhtl = self->nhtl * 2 + 1;
    self->nhtlr = 0;
    while (!my_isprime(self->nhtl)) self->nhtl += 2;

    self->htl = malloc(sizeof(*self->htl) * self->nhtl);
    assert(self->htl != NULL);

    int i;
    for (i = 0; i < self->nhtl; ++i) {
        self->htl[i].key = NULL;
    }

    /* Re-hash */
    for (i = 0; i < nold; ++i) {
        if (old[i].key) {
            carve_prog_label_set(self, old[i].key, old[i].val);
            free(old[i].key);
        }
    }

    free(old);
}

carve_prog carve_prog_new(const char* src) {
    carve_prog self = malloc(sizeof(*self));

    int sl = strlen(src);
    self->src = malloc(sl + 1);
    memcpy(self->src, src, sl);
    self->src[sl] = '\0';

    self->ninst = 0;
    self->inst = NULL;

    self->nhtl = 0;
    self->nhtlr = 0;
    self->htl = NULL;

    carve_prog_add(self, carve_makeI(0x13, 1, 0x0, 0, 0));
    carve_prog_add(self, carve_makeI(0x13, 2, 0x0, 0, 10));
    carve_prog_add(self, carve_makeB(0x63, 0, (2 * 4) >> 1, 0x5, 1, 2, 0, 0));
    carve_prog_add(self, carve_makeI(0x13, 1, 0x0, 1, 1));
    carve_prog_add(self, carve_makeJr(0x6F, 0, -12));

    return self;
}

void carve_prog_add(carve_prog self, carve_inst inst) {
    int i = self->ninst++;
    self->inst = realloc(self->inst, sizeof(*self->inst) * self->ninst);

    self->inst[i] = inst;
}

bool carve_prog_label_set(carve_prog self, const char* key, int inst) {
    if (self->nhtl < 1 || self->nhtlr * 2 >= self->nhtl) {
        my_resize(self);
    }
    int hash = my_hash(key);

    int bi = hash % self->nhtl;
    int bi0 = bi;
    do {
        struct carve_htl* htl = &self->htl[bi];

        if (!htl->key) {
            /* Found empty spot, so add */
            int sl = strlen(key);
            htl->key = malloc(sl + 1);
            memcpy(htl->key, key, sl);
            htl->key[sl] = '\0';
            htl->hash = hash;

            htl->val = inst;

            self->nhtlr++;

            return false;
        } else if (htl->hash == hash && strcmp(htl->key, key) == 0) {
            /* Key already existed, so replace it */

            htl->val = inst;
            return true;
        }

        bi = (bi + 1) % self->nhtl;
    } while (bi0 != bi);

    /* Should never happen */
    assert(false);
    return false;
}

int carve_prog_label_get(carve_prog self, const char* key) {
    if (self->nhtl < 1) return -1;
    int hash = my_hash(key);

    int bi = hash % self->nhtl;
    int bi0 = bi;
    do {
        struct carve_htl* htl = &self->htl[bi];

        if (!htl->key) {
            /* Found empty spot, so not present */
            return -1;
        } else if (htl->hash == hash && strcmp(htl->key, key) == 0) {
            /* Key exists, so return value */
            return htl->val;
        }

        bi = (bi + 1) % self->nhtl;
    } while (bi0 != bi);

    /* Not found */
    return -1;
}



#define MAXBUF 100

char* carve_prog_ihs(carve_prog self, int i) {
    char* res = malloc(MAXBUF);

    snprintf(res, MAXBUF, "%08llx", (long long unsigned int)(void*)self->inst[i]);

    return res;
}


