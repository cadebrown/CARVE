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

#define CARVE_DO_LUI(_rd, _imm) REGS(_rd) = _imm;
#define CARVE_DO_AUIPC(_rd, _imm)
#define CARVE_DO_JAL(_rd, _imm) if (_rd != 0) { REGU(_rd) = PC; } PC += CARVE_SEXT(_imm, 20);
#define CARVE_DO_JALR(_rd, _rs1, _imm) do { \
    carve_int rr = PC;\
    PC = REGU(_rd) + CARVE_SEXT(_imm, 19); \
    if (_rd != 0) { REGU(_rd) = rr; } \
} while (0);
#define CARVE_DO_BEQ(_rs1, _rs2, _imm)
#define CARVE_DO_BNE(_rs1, _rs2, _imm)
#define CARVE_DO_BLT(_rs1, _rs2, _imm)
#define CARVE_DO_BGE(_rs1, _rs2, _imm) do { \
    if (REGS(_rs1) >= REGS(_rs2)) PC += CARVE_SEXT(_imm, 12); \
} while (0);
#define CARVE_DO_BLTU(_rs1, _rs2, _imm)
#define CARVE_DO_BGEU(_rs1, _rs2, _imm)
#define CARVE_DO_LB(_rd, _rs1, _imm) REGU(_rd) = *((carve_b*)(REGU(_rs1) + _imm));
#define CARVE_DO_LH(_rd, _rs1, _imm) REGU(_rd) = *((carve_h*)(REGU(_rs1) + _imm));
#define CARVE_DO_LW(_rd, _rs1, _imm) REGU(_rd) = *((carve_w*)(REGU(_rs1) + _imm));
#define CARVE_DO_LBU(_rd, _rs1, _imm)
#define CARVE_DO_LHU(_rd, _rs1, _imm)
#define CARVE_DO_SB(_rs1, _rs2, _imm) if (_rs1 != 0) { *((carve_b*)(REGU(_rs1) + _imm)) = REGU(_rs2); }
#define CARVE_DO_SH(_rs1, _rs2, _imm) if (_rs1 != 0) { *((carve_h*)(REGU(_rs1) + _imm)) = REGU(_rs2); }
#define CARVE_DO_SW(_rs1, _rs2, _imm) if (_rs1 != 0) { *((carve_w*)(REGU(_rs1) + _imm)) = REGU(_rs2); }
#define CARVE_DO_ADDI(_rd, _rs1, _imm) REGU(_rd) = REGU(_rs1) + _imm;
#define CARVE_DO_SLTI(_rd, _rs1, _imm) REGU(_rd) = REGS(_rs1) < _imm;
#define CARVE_DO_SLTIU(_rd, _rs1, _imm) REGU(_rd) = REGS(_rs1) < _imm;
#define CARVE_DO_XORI(_rd, _rs1, _imm) REGU(_rd) = REGU(_rs1) ^ _imm;
#define CARVE_DO_ORI(_rd, _rs1, _imm) REGU(_rd) = REGU(_rs1) | _imm;
#define CARVE_DO_ANDI(_rd, _rs1, _imm) REGU(_rd) = REGU(_rs1) & _imm;
#define CARVE_DO_SLLI(_rd, _rs1, _imm) REGU(_rd) = REGU(_rs1) << _imm;
#define CARVE_DO_SRLI(_rd, _rs1, _imm) do { \
    carve_int v = _imm; \
    carve_int shamt = _imm & 0x7F; \
    if (v == shamt) { \
        /* All higher bits are 0, so SRLI */ \
        REGU(_rd) = REGU(_rs1) >> shamt;\
    } else { \
        /* High one bit, so SRAI */ \
        REGS(_rd) = REGS(_rs1) / (1ULL << shamt);\
    } \
} while (0);
#define CARVE_DO_SRAI(_rd, _rs1, _imm) assert(false);

#define CARVE_DO_ADD(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) + REGU(_rs2);
#define CARVE_DO_SUB(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) - REGU(_rs2);
#define CARVE_DO_SLL(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) << REGU(_rs2);
#define CARVE_DO_SLT(_rd, _rs1, _rs2) REGU(_rd) = REGS(_rs1) < REGS(_rs2);
#define CARVE_DO_SLTU(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) < REGU(_rs2);
#define CARVE_DO_XOR(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) ^ REGU(_rs2);
#define CARVE_DO_SRL(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) >> REGU(_rs2);
#define CARVE_DO_SRA(_rd, _rs1, _rs2) REGS(_rd) = REGS(_rs1) / REGS(_rs2);
#define CARVE_DO_OR(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) | REGU(_rs2);
#define CARVE_DO_AND(_rd, _rs1, _rs2) REGU(_rd) = REGU(_rs1) & REGU(_rs2);

static inline void exec_inst(carve_state s, carve_inst inst) {
    carve_inst opcode, rd, f3, rs1, rs2, f7, imm_11_0, imm_31_12, imm_4_0, imm_11_5, _11, imm_4_1, imm_10_5, _12, imm_19_12, imm_10_1, _20, tmp0, tmp1, tmp2;
    switch (opcode = inst & 0x7F) {
        case 0x37:
            CARVE_DEC_U(inst, opcode, rd, imm_31_12);
            /* LUI */
            tmp0 = (imm_31_12 << 12);
            CARVE_DO_LUI(rd, tmp0);
            break;
        case 0x17:
            CARVE_DEC_U(inst, opcode, rd, imm_31_12);
            /* AUIPC */
            tmp0 = (imm_31_12 << 12);
            CARVE_DO_AUIPC(rd, tmp0);
            break;
        case 0x6f:
            CARVE_DEC_J(inst, opcode, rd, imm_19_12, _11, imm_10_1, _20);
            /* JAL */
            tmp0 = (imm_10_1 << 1) | (_11 << 11) | (imm_19_12 << 12) | (_20 << 20);
            CARVE_DO_JAL(rd, tmp0);
            break;
        case 0x67:
            CARVE_DEC_I(inst, opcode, rd, f3, rs1, imm_11_0);
            switch (f3) {
                case 0x0:
                    /* JALR */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_JALR(rd, rs1, tmp0);
                    break;
            }
            break;
        case 0x63:
            CARVE_DEC_B(inst, opcode, _11, imm_4_1, f3, rs1, rs2, imm_10_5, _12);
            switch (f3) {
                case 0x0:
                    /* BEQ */
                    tmp0 = (imm_4_1 << 1) | (imm_10_5 << 5) | (_11 << 11) | (_12 << 12);
                    CARVE_DO_BEQ(rs1, rs2, tmp0);
                    break;
                case 0x1:
                    /* BNE */
                    tmp0 = (imm_4_1 << 1) | (imm_10_5 << 5) | (_11 << 11) | (_12 << 12);
                    CARVE_DO_BNE(rs1, rs2, tmp0);
                    break;
                case 0x4:
                    /* BLT */
                    tmp0 = (imm_4_1 << 1) | (imm_10_5 << 5) | (_11 << 11) | (_12 << 12);
                    CARVE_DO_BLT(rs1, rs2, tmp0);
                    break;
                case 0x5:
                    /* BGE */
                    tmp0 = (imm_4_1 << 1) | (imm_10_5 << 5) | (_11 << 11) | (_12 << 12);
                    CARVE_DO_BGE(rs1, rs2, tmp0);

                    break;
                case 0x6:
                    /* BLTU */
                    tmp0 = (imm_4_1 << 1) | (imm_10_5 << 5) | (_11 << 11) | (_12 << 12);
                    CARVE_DO_BLTU(rs1, rs2, tmp0);
                    break;
                case 0x7:
                    /* BGEU */
                    tmp0 = (imm_4_1 << 1) | (imm_10_5 << 5) | (_11 << 11) | (_12 << 12);
                    CARVE_DO_BGEU(rs1, rs2, tmp0);
                    break;
            }
            break;
        case 0x3:
            CARVE_DEC_I(inst, opcode, rd, f3, rs1, imm_11_0);
            switch (f3) {
                case 0x0:
                    /* LB */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_LB(rd, rs1, tmp0);
                    break;
                case 0x1:
                    /* LH */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_LH(rd, rs1, tmp0);
                    break;
                case 0x2:
                    /* LW */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_LW(rd, rs1, tmp0);
                    break;
                case 0x4:
                    /* LBU */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_LBU(rd, rs1, tmp0);
                    break;
                case 0x5:
                    /* LHU */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_LHU(rd, rs1, tmp0);
                    break;
            }
            break;
        case 0x23:
            CARVE_DEC_S(inst, opcode, imm_4_0, f3, rs1, rs2, imm_11_5);
            switch (f3) {
                case 0x0:
                    /* SB */
                    tmp0 = (imm_4_0 << 0) | (imm_11_5 << 5);
                    CARVE_DO_SB(rs1, rs2, tmp0);
                    break;
                case 0x1:
                    /* SH */
                    tmp0 = (imm_4_0 << 0) | (imm_11_5 << 5);
                    CARVE_DO_SH(rs1, rs2, tmp0);
                    break;
                case 0x2:
                    /* SW */
                    tmp0 = (imm_4_0 << 0) | (imm_11_5 << 5);
                    CARVE_DO_SW(rs1, rs2, tmp0);
                    break;
            }
            break;
        case 0x13:
            CARVE_DEC_I(inst, opcode, rd, f3, rs1, imm_11_0);
            switch (f3) {
                case 0x0:
                    /* ADDI */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_ADDI(rd, rs1, tmp0);
                    break;
                case 0x2:
                    /* SLTI */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_SLTI(rd, rs1, tmp0);
                    break;
                case 0x3:
                    /* SLTIU */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_SLTIU(rd, rs1, tmp0);
                    break;
                case 0x4:
                    /* XORI */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_XORI(rd, rs1, tmp0);
                    break;
                case 0x6:
                    /* ORI */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_ORI(rd, rs1, tmp0);
                    break;
                case 0x7:
                    /* ANDI */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_ANDI(rd, rs1, tmp0);
                    break;
                case 0x1:
                    /* SLLI */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_SLLI(rd, rs1, tmp0);
                    break;
                case 0x5:
                    /* SRLI */
                    tmp0 = imm_11_0 << 0; 
                    CARVE_DO_SRLI(rd, rs1, tmp0);
                    break;
            }
            break;
        case 0x33:
            CARVE_DEC_R(inst, opcode, rd, f3, rs1, rs2, f7); 
            switch (f3) {
                case 0x0:
                    switch (f7) {
                        case 0x0:
                            /* ADD */
                            CARVE_DO_ADD(rd, rs1, rs2);
                            break;
                        case 0x20:
                            /* SUB */
                            CARVE_DO_SUB(rd, rs1, rs2);
                            break;
                    }
                case 0x1:
                    switch (f7) {
                        case 0x0:
                            /* SLL */
                            CARVE_DO_SLL(rd, rs1, rs2);
                            break;
                    }
                case 0x2:
                    switch (f7) {
                        case 0x0:
                            /* SLT */
                            CARVE_DO_SLT(rd, rs1, rs2);
                            break;
                    }
                case 0x3:
                    switch (f7) {
                        case 0x0:
                            /* SLTU */
                            CARVE_DO_SLTU(rd, rs1, rs2);
                            break;
                    }
                case 0x4:
                    switch (f7) {
                        case 0x0:
                            /* XOR */
                            CARVE_DO_XOR(rd, rs1, rs2);
                            break;
                    }
                case 0x5:
                    switch (f7) {
                        case 0x0:
                            /* SRL */
                            CARVE_DO_SRL(rd, rs1, rs2);
                            break;
                        case 0x10:
                            /* SRA */
                            CARVE_DO_SRA(rd, rs1, rs2);
                            break;
                    }
                case 0x6:
                    switch (f7) {
                        case 0x0:
                            /* OR */
                            CARVE_DO_OR(rd, rs1, rs2);
                            break;
                    }
                case 0x7:
                    switch (f7) {
                        case 0x0:
                            /* AND */
                            CARVE_DO_AND(rd, rs1, rs2);
                            break;
                    }
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
