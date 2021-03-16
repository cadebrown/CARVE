/* init.c - initialization code for libcarve
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"

void carve_init() {
    /*  */
    fprintf(stderr, "libcarve is initialized\n");
}

int carve_printcontext(const char* fname, const char* src, carve_tok tok) {

    /* Find the start of the line */
    int sl = tok.pos;
    while (sl > 0 && src[sl - 1] != '\n') {
        sl--;
    }

    /* Find the end of the line */
    int el = tok.pos + 1;
    while (src[el] != '\0' && src[el] != '\n') {
        el++;
    }

    fprintf(stderr, "%.*s\n", (int)(el - sl), src + sl);
    int i;
    for (i = 0; i < tok.col; ++i) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "^");
    for (i = 0; i < tok.len - 1; ++i) {
        fprintf(stderr, "~");
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "@ %s (L%i, C%i)\n", fname, tok.line + 1, tok.col + 1);

    return 0;
}



