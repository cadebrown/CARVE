/* getinst.c - Generated file that defines parse modes for RISC-V instructions
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT ** 
 */

#include <carve.h>




/* Database of instructions */

static int I_ninsts = 40;
static struct carve_instdesc I_insts[40] = {

    (struct carve_instdesc) { "add", 3, 'R', 51, 0, 0 },
    (struct carve_instdesc) { "addi", 4, 'I', 19, 0, 0 },
    (struct carve_instdesc) { "and", 3, 'R', 51, 7, 0 },
    (struct carve_instdesc) { "andi", 4, 'I', 19, 7, 0 },
    (struct carve_instdesc) { "auipc", 5, 'U', 23, 0, 0 },
    (struct carve_instdesc) { "beq", 3, 'B', 99, 0, 0 },
    (struct carve_instdesc) { "bge", 3, 'B', 99, 5, 0 },
    (struct carve_instdesc) { "bgeu", 4, 'B', 99, 7, 0 },
    (struct carve_instdesc) { "blt", 3, 'B', 99, 4, 0 },
    (struct carve_instdesc) { "bltu", 4, 'B', 99, 6, 0 },
    (struct carve_instdesc) { "bne", 3, 'B', 99, 1, 0 },
    (struct carve_instdesc) { "ebreak", 6, 'y', 115, 0, 0 },
    (struct carve_instdesc) { "ecall", 5, 'y', 115, 0, 0 },
    (struct carve_instdesc) { "j", 1, 'p', 0, 0, 0 },
    (struct carve_instdesc) { "jal", 3, 'J', 111, 0, 0 },
    (struct carve_instdesc) { "jalr", 4, 'I', 103, 0, 0 },
    (struct carve_instdesc) { "lb", 2, 'I', 3, 0, 0 },
    (struct carve_instdesc) { "lbu", 3, 'I', 3, 4, 0 },
    (struct carve_instdesc) { "lh", 2, 'I', 3, 1, 0 },
    (struct carve_instdesc) { "lhu", 3, 'I', 3, 5, 0 },
    (struct carve_instdesc) { "lui", 3, 'U', 55, 0, 0 },
    (struct carve_instdesc) { "lw", 2, 'I', 3, 2, 0 },
    (struct carve_instdesc) { "or", 2, 'R', 51, 6, 0 },
    (struct carve_instdesc) { "ori", 3, 'I', 19, 6, 0 },
    (struct carve_instdesc) { "sb", 2, 'S', 35, 0, 0 },
    (struct carve_instdesc) { "sh", 2, 'S', 35, 1, 0 },
    (struct carve_instdesc) { "sll", 3, 'R', 51, 1, 0 },
    (struct carve_instdesc) { "slli", 4, 'I', 19, 1, 0 },
    (struct carve_instdesc) { "slt", 3, 'R', 51, 2, 0 },
    (struct carve_instdesc) { "slti", 4, 'I', 19, 2, 0 },
    (struct carve_instdesc) { "sltiu", 5, 'I', 19, 3, 0 },
    (struct carve_instdesc) { "sltu", 4, 'R', 51, 3, 0 },
    (struct carve_instdesc) { "sra", 3, 'R', 51, 5, 16 },
    (struct carve_instdesc) { "srai", 4, 'I', 19, 5, 0 },
    (struct carve_instdesc) { "srl", 3, 'R', 51, 5, 0 },
    (struct carve_instdesc) { "srli", 4, 'I', 19, 5, 0 },
    (struct carve_instdesc) { "sub", 3, 'R', 51, 0, 32 },
    (struct carve_instdesc) { "sw", 2, 'S', 35, 2, 0 },
    (struct carve_instdesc) { "xor", 3, 'R', 51, 4, 0 },
    (struct carve_instdesc) { "xori", 4, 'I', 19, 4, 0 },

};




/* Retrieve instruction information */
struct carve_instdesc* carve_getinst(char* src, int len) {
    if (len < 0) len = strlen(src);
    int l = 0, r = I_ninsts - 1, m;
    while (l <= r) {
        m = (l + r) / 2;

        /* Key we are comparing to */
        int nname = I_insts[m].nname;
        char* name = I_insts[m].name;

        int cmp = 0;
        
        if (len == nname) {
            cmp = strncmp(src, name, nname);
        } else {
            int minl = len < nname ? len : nname;
            cmp = strncmp(src, name, minl);
            if (cmp == 0) {
                cmp = len - nname;
            }
        }

        if (cmp == 0) {
            return &I_insts[m];
        } else if (cmp > 0) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }

    /* Not-found */
    return NULL;
}

