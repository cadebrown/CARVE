/* ext/RV32I.h - RISC-V 'RV32I' extension set
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#ifndef EXT_RV32I_H__
#define EXT_RV32I_H__

#define CARVE_lui(rd, imm) do { \
    if (rd != 0) { \
        REGS(rd) = imm; \
    } \
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
    if (REGU(rs1) + imm == 0x1A4) { \
        STATE->easteregg = 1; \
    } \
    if (rd != 0) { \
        void* addr = V2R(REGU(rs1) + imm); \
        REGU(rd) = *(carve_b*)addr; \
    } \
} while (0)

#define CARVE_lh(rd, rs1, imm) do { \
    if (rd != 0) { \
        void* addr = V2R(REGU(rs1) + imm); \
        REGU(rd) = CARVE_SEXT(*(carve_h*)addr, 15); \
    } \
} while (0)

#define CARVE_lw(rd, rs1, imm) do { \
    if (rd != 0) { \
        void* addr = V2R(REGU(rs1) + imm); \
        REGU(rd) = CARVE_SEXT(*(carve_w*)addr, 31); \
    } \
} while (0)

#define CARVE_lbu(rd, rs1, imm) do { \
    if (rd != 0) { \
        void* addr = V2R(REGU(rs1) + imm); \
        REGU(rd) = *(carve_b*)addr; \
    } \
} while (0)

#define CARVE_lhu(rd, rs1, imm) do { \
    if (rd != 0) { \
        void* addr = V2R(REGU(rs1) + imm); \
        REGU(rd) = *(carve_h*)addr; \
    } \
} while (0)

#define CARVE_sb(rs1, rs2, imm) do { \
    void* addr = V2R(REGU(rs1) + imm); \
    *(carve_b*)addr = REGU(rs2); \
} while (0)

#define CARVE_sh(rs1, rs2, imm) do { \
    void* addr = V2R(REGU(rs1) + imm); \
    *(carve_h*)addr = REGU(rs2); \
} while (0)

#define CARVE_sw(rs1, rs2, imm) do { \
    void* addr = V2R(REGU(rs1) + imm); \
    *((carve_w*)(REGU(rs1) + imm)) = REGU(rs2); \
} while (0)

#define CARVE_addi(rd, rs1, imm) do { \
    if (rd != 0) { \
        REGS(rd) = REGS(rs1) + CARVE_SEXT(imm, 11); \
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

#define CARVE_add(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) + REGU(rs2); \
    } \
} while (0)

#define CARVE_sub(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) - REGU(rs2); \
    } \
} while (0)

#define CARVE_sll(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) << REGU(rs2); \
    } \
} while (0)

#define CARVE_srl(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) >> REGU(rs2); \
    } \
} while (0)

#define CARVE_sra(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) >> REGU(rs2); \
    } \
} while (0)

#define CARVE_slt(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGS(rs1) < REGS(rs2); \
    } \
} while (0)

#define CARVE_sltu(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) < REGU(rs2); \
    } \
} while (0)

#define CARVE_xor(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) ^ REGU(rs2); \
    } \
} while (0)

#define CARVE_or(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) | REGU(rs2); \
    } \
} while (0)

#define CARVE_and(rd, rs1, rs2) do { \
    if (rd != 0) { \
        REGU(rd) = REGU(rs1) & REGU(rs2); \
    } \
} while (0)


#define CARVE_ebreak(rd, rs1, imm) do { \
    /* ebreak and ecall are here */ \
    if (imm == 0) { \
        /* ecall */ \
    } else if (imm == 1) { \
        /* ebreak */ \
        STATE->is_halted = true; \
    } \
} while (0)

#define CARVE_PSEUDO_j() do { \
    /* j offset -> jal x0, offset */ \
    int imm; \
    if (!parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'J', &imm)) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeJ(carve_getinst("jal", -1)->opcode, 0, imm)); \
} while (0)


#endif /* EXT_RV32I__ */
