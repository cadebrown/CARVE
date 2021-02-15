/* exec.c - RISC-V executor
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"

/* Helper macros */

/* Signed integer register '_x' */
#define REGS(_x) (((carve_sint*)s->x)[_x])

/* Unsigned integer register '_x' */
#define REGU(_x) (s->x[_x])

/* Program counter */
#define PC (s->pc)


/** RISC-V Instructions **/

#define CARVE_lui(rd, imm) do { \
    REGS(rd) = imm; \
} while (0)

#define CARVE_auipc(rd, imm) do { \
    PC += imm; \
} while (0)

#define CARVE_jal(rd, imm) do { \
    if (rd != 0) { \
        REGU(rd) = PC; \
    } \
    PC += CARVE_SEXT(imm, 20); \
} while (0)

#define CARVE_jalr(rd, rs1, imm) do { \
    carve_int rr = PC; \
    PC = REGU(rs1) + CARVE_SEXT(imm, 19); \
    if (rd != 0) { \
        REGU(rd) = rr; \
    } \
} while (0)


#define CARVE_beq(rs1, rs2, imm) do { \
    if (REGU(rs1) == REGU(rs2)) { \
        PC += CARVE_SEXT(imm, 12); \
    } \
} while (0)

#define CARVE_bne(rs1, rs2, imm) do { \
    if (REGU(rs1) != REGU(rs2)) { \
        PC += CARVE_SEXT(imm, 12); \
    } \
} while (0)

#define CARVE_blt(rs1, rs2, imm) do { \
    if (REGS(rs1) < REGS(rs2)) { \
        PC += CARVE_SEXT(imm, 12); \
    } \
} while (0)

#define CARVE_bge(rs1, rs2, imm) do { \
    if (REGS(rs1) >= REGS(rs2)) { \
        PC += CARVE_SEXT(imm, 12); \
    } \
} while (0)

#define CARVE_bltu(rs1, rs2, imm) do { \
    if (REGU(rs1) < REGU(rs2)) { \
        PC += CARVE_SEXT(imm, 12); \
    } \
} while (0)

#define CARVE_bgeu(rs1, rs2, imm) do { \
    if (REGU(rs1) >= REGU(rs2)) { \
        PC += CARVE_SEXT(imm, 12); \
    } \
} while (0)

#define CARVE_lb(rd, rs1, imm) do { \
    REGU(rd) = CARVE_SEXT(*((carve_b*)(REGU(rs1) + imm)), 7); \
} while (0)
#define CARVE_lh(rd, rs1, imm) do { \
    REGU(rd) = CARVE_SEXT(*((carve_h*)(REGU(rs1) + imm)), 15); \
} while (0)
#define CARVE_lw(rd, rs1, imm) do { \
    REGU(rd) = CARVE_SEXT(*((carve_w*)(REGU(rs1) + imm)), 31); \
} while (0)
#define CARVE_lbu(rd, rs1, imm) do { \
    REGU(rd) = *((carve_b*)(REGU(rs1) + imm)); \
} while (0)
#define CARVE_lhu(rd, rs1, imm) do { \
    REGU(rd) = *((carve_h*)(REGU(rs1) + imm)); \
} while (0)

#define CARVE_sb(rs1, rs2, imm) do { \
    if (rs1 != 0) { \
        *((carve_b*)(REGU(rs1) + imm)) = REGU(rs2); \
    } \
} while (0)
#define CARVE_sh(rs1, rs2, imm) do { \
    if (rs1 != 0) { \
        *((carve_h*)(REGU(rs1) + imm)) = REGU(rs2); \
    } \
} while (0)
#define CARVE_sw(rs1, rs2, imm) do { \
    if (rs1 != 0) { \
        *((carve_w*)(REGU(rs1) + imm)) = REGU(rs2); \
    } \
} while (0)

#define CARVE_addi(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) + imm; \
    } \
} while (0)

#define CARVE_slti(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGS(rd) = REGS(rs1) < imm; \
    } \
} while (0)
#define CARVE_sltiu(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) < imm; \
    } \
} while (0)

#define CARVE_xori(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) ^ imm; \
    } \
} while (0)
#define CARVE_ori(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) | imm; \
    } \
} while (0)
#define CARVE_andi(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) & imm; \
    } \
} while (0)
#define CARVE_slli(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) << imm; \
    } \
} while (0)

#define CARVE_srli(rd, rs1, imm) do { \
    if (rd != 0) { \
        carve_int v = imm; \
        carve_int shamt = imm & 0x7F; \
        if (v == shamt) { \
            /* All higher bits are 0, so SRLI */ \
            REGU(rd) = REGU(rs1) >> shamt;\
        } else { \
            /* High one bit, so SRAI */ \
            REGS(rd) = REGS(rs1) / (1ULL << shamt);\
        } \
    } \
} while (0)

#define CARVE_add(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) + REGU(rs2); \
    } \
} while (0)
#define CARVE_sub(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) - REGU(rs2); \
    } \
} while (0)


#define CARVE_sll(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) << REGU(rs2); \
    } \
} while (0)

#define CARVE_srl(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) >> REGU(rs2); \
    } \
} while (0)

#define CARVE_sra(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) >> REGU(rs2); \
    } \
} while (0)

#define CARVE_slt(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGS(rs1) < REGS(rs2); \
    } \
} while (0)

#define CARVE_sltu(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) < REGU(rs2); \
    } \
} while (0)

#define CARVE_xor(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) ^ REGU(rs2); \
    } \
} while (0)
#define CARVE_or(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) | REGU(rs2); \
    } \
} while (0)

#define CARVE_and(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) & REGU(rs2); \
    } \
} while (0)


static inline void exec_inst(carve_state s, carve_inst inst) {
    carve_inst opcode, f3, f7, rd, rs1, rs2, imm;    
    switch (opcode = inst & 0x7F) {
        case 0x37:
            CARVE_DEC_R(inst, opcode, f3, f7, rd, rs1, rs2);
            /* lui */
            CARVE_lui(rd, imm);
            break;
        case 0x17:
            CARVE_DEC_R(inst, opcode, f3, f7, rd, rs1, rs2);
            /* auipc */
            CARVE_auipc(rd, imm);
            break;
        case 0x6f:
            CARVE_DEC_R(inst, opcode, f3, f7, rd, rs1, rs2);
            /* jal */
            CARVE_jal(rd, imm);
            break;
        case 0x67:
            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    /* jalr */
                    CARVE_jalr(rd, rs1, imm);
                    break;
            }
            break;
        case 0x63:
            CARVE_DEC_B(inst, opcode, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                    /* beq */
                    CARVE_beq(rs1, rs2, imm);
                    break;
                case 0x1:
                    /* bne */
                    CARVE_bne(rs1, rs2, imm);
                    break;
                case 0x4:
                    /* blt */
                    CARVE_blt(rs1, rs2, imm);
                    break;
                case 0x5:
                    /* bge */
                    CARVE_bge(rs1, rs2, imm);
                    break;
                case 0x6:
                    /* bltu */
                    CARVE_bltu(rs1, rs2, imm);
                    break;
                case 0x7:
                    /* bgeu */
                    CARVE_bgeu(rs1, rs2, imm);
                    break;
            }
            break;
        case 0x3:
            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    /* lb */
                    CARVE_lb(rd, rs1, imm);
                    break;
                case 0x1:
                    /* lh */
                    CARVE_lh(rd, rs1, imm);
                    break;
                case 0x2:
                    /* lw */
                    CARVE_lw(rd, rs1, imm);
                    break;
                case 0x4:
                    /* lbu */
                    CARVE_lbu(rd, rs1, imm);
                    break;
                case 0x5:
                    /* lhu */
                    CARVE_lhu(rd, rs1, imm);
                    break;
            }
            break;
        case 0x23:
            CARVE_DEC_S(inst, opcode, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                    /* sb */
                    CARVE_sb(rs1, rs2, imm);
                    break;
                case 0x1:
                    /* sh */
                    CARVE_sh(rs1, rs2, imm);
                    break;
                case 0x2:
                    /* sw */
                    CARVE_sw(rs1, rs2, imm);
                    break;
            }
            break;
        case 0x13:
            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    /* addi */
                    CARVE_addi(rd, rs1, imm);
                    break;
                case 0x2:
                    /* slti */
                    CARVE_slti(rd, rs1, imm);
                    break;
                case 0x3:
                    /* sltiu */
                    CARVE_sltiu(rd, rs1, imm);
                    break;
                case 0x4:
                    /* xori */
                    CARVE_xori(rd, rs1, imm);
                    break;
                case 0x6:
                    /* ori */
                    CARVE_ori(rd, rs1, imm);
                    break;
                case 0x7:
                    /* andi */
                    CARVE_andi(rd, rs1, imm);
                    break;
                case 0x1:
                    /* slli */
                    CARVE_slli(rd, rs1, imm);
                    break;
                case 0x5:
                    /* srli */
                    CARVE_srli(rd, rs1, imm);
                    break;
            }
            break;
        case 0x33:
            CARVE_DEC_R(inst, opcode, f3, f7, rd, rs1, rs2);
            switch (f3) {
                case 0x0:
                    switch (f7) {
                        case 0x0:
                            /* add */
                            CARVE_add(rd, rs1, rs2);
                        break;
                        case 0x20:
                            /* sub */
                            CARVE_sub(rd, rs1, rs2);
                        break;
                    }
                    break;
                case 0x1:
                    switch (f7) {
                        case 0x0:
                            /* sll */
                            CARVE_sll(rd, rs1, rs2);
                        break;
                    }
                    break;
                case 0x2:
                    switch (f7) {
                        case 0x0:
                            /* slt */
                            CARVE_slt(rd, rs1, rs2);
                        break;
                    }
                    break;
                case 0x3:
                    switch (f7) {
                        case 0x0:
                            /* sltu */
                            CARVE_sltu(rd, rs1, rs2);
                        break;
                    }
                    break;
                case 0x4:
                    switch (f7) {
                        case 0x0:
                            /* xor */
                            CARVE_xor(rd, rs1, rs2);
                        break;
                    }
                    break;
                case 0x5:
                    switch (f7) {
                        case 0x0:
                            /* srl */
                            CARVE_srl(rd, rs1, rs2);
                        break;
                        case 0x10:
                            /* sra */
                            CARVE_sra(rd, rs1, rs2);
                        break;
                    }
                    break;
                case 0x6:
                    switch (f7) {
                        case 0x0:
                            /* or */
                            CARVE_or(rd, rs1, rs2);
                        break;
                    }
                    break;
                case 0x7:
                    switch (f7) {
                        case 0x0:
                            /* and */
                            CARVE_and(rd, rs1, rs2);
                        break;
                    }
                    break;
            }
            break;
    }
}


int carve_exec(carve_state s, carve_prog p) {
    s->pc = (carve_int)p->inst;

    int ct = 0;
    while (ct < 50) {
        exec_inst(s, *(carve_inst*)s->pc);
        s->pc += sizeof(carve_inst);

        ct++;
    }

    return 0;
}
