/* ext/RV32I.h - RISC-V 'RV32I' extension set
 *
 * @author: Cade Brown <cade@utk.edu>
 * @author: Gregory Croisdale <greg@kscript.org>
 */

#ifndef EXT_RV32I_H__
#define EXT_RV32I_H__

#define SEXT(v, b) ((v << ((sizeof(v) * 8) - (b))) >> (((sizeof(v) * 8) - (b))))

#define CARVE_lui(rd, imm) do { \
    REGS(rd) = imm; \
} while (0)

#define CARVE_auipc(rd, imm) do { \
    PC += imm; \
} while (0)

#define CARVE_jal(rd, imm) do { \
    REGU(rd) = PC; \
    PC += CARVE_SEXT(imm, 20); \
} while (0)

#define CARVE_jalr(rd, rs1, imm) do { \
    carve_int rr = PC; \
    PC = REGU(rs1) + CARVE_SEXT(imm, 19); \
    REGU(rd) = rr; \
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
    void* addr = V2R(REGU(rs1) + imm); \
    REGU(rd) = *(carve_b*)addr; \
} while (0)

#define CARVE_lh(rd, rs1, imm) do { \
    void* addr = V2R(REGU(rs1) + imm); \
    REGU(rd) = CARVE_SEXT(*(carve_h*)addr, 15); \
} while (0)

#define CARVE_lw(rd, rs1, imm) do { \
    void* addr = V2R(REGU(rs1) + imm); \
    REGU(rd) = CARVE_SEXT(*(carve_w*)addr, 31); \
} while (0)

#define CARVE_lbu(rd, rs1, imm) do { \
    void* addr = V2R(REGU(rs1) + imm); \
    REGU(rd) = *(carve_b*)addr; \
} while (0)

#define CARVE_lhu(rd, rs1, imm) do { \
    void* addr = V2R(REGU(rs1) + imm); \
    REGU(rd) = *(carve_h*)addr; \
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
    REGS(rd) = REGS(rs1) + CARVE_SEXT(imm, 11); \
} while (0)

#define CARVE_slti(rd, rs1, imm) do { \
    REGS(rd) = REGS(rs1) < CARVE_SEXT(imm, 11); \
} while (0)

#define CARVE_sltiu(rd, rs1, imm) do { \
    REGU(rd) = REGU(rs1) < imm; \
} while (0)

#define CARVE_xori(rd, rs1, imm) do { \
    REGS(rd) = REGS(rs1) ^ CARVE_SEXT(imm, 11); \
} while (0)

#define CARVE_ori(rd, rs1, imm) do { \
    REGS(rd) = REGS(rs1) | CARVE_SEXT(imm, 11); \
} while (0)

#define CARVE_andi(rd, rs1, imm) do { \
    REGS(rd) = REGS(rs1) & CARVE_SEXT(imm, 11); \
} while (0)

#define CARVE_slli(rd, rs1, imm) do { \
    REGU(rd) = REGU(rs1) << imm; \
} while (0)

#define CARVE_srli(rd, rs1, imm) do { \
    carve_int v = imm; \
    carve_int shamt = imm & 0x7F; \
    if (v == shamt) { \
        /* All higher bits are 0, so SRLI */ \
        REGU(rd) = REGU(rs1) >> shamt;\
    } else { \
        /* High one bit, so SRAI */ \
        REGS(rd) = REGS(rs1) / (1ULL << shamt);\
    } \
} while (0)

#define CARVE_add(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) + REGU(rs2); \
} while (0)

#define CARVE_sub(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) - REGU(rs2); \
} while (0)

#define CARVE_sll(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) << REGU(rs2); \
} while (0)

#define CARVE_srl(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) >> REGU(rs2); \
} while (0)

#define CARVE_sra(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) >> REGU(rs2); \
} while (0)

#define CARVE_slt(rd, rs1, rs2) do { \
    REGU(rd) = REGS(rs1) < REGS(rs2); \
} while (0)

#define CARVE_sltu(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) < REGU(rs2); \
} while (0)

#define CARVE_xor(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) ^ REGU(rs2); \
} while (0)

#define CARVE_or(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) | REGU(rs2); \
} while (0)

#define CARVE_and(rd, rs1, rs2) do { \
    REGU(rd) = REGU(rs1) & REGU(rs2); \
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

/* INTEGER PSEUDOINSTRUCTIONS */

#define CARVE_PSEUDO_nop() do { \
    /* nop -> addi x0, x0, 0 */ \
    struct carve_instdesc* inst = carve_getinst("addi", -1); \
    carve_prog_add(prog, carve_makeI(inst->opcode, inst->f3, 0, 0, 0)); \
} while (0)

#define CARVE_PSEUDO_li() do { \
    /* if small, li rd, imm -> addi rd, x0, imm */ \
    /* otherwise, lui & addi */ \
    int rd, rs = 0; \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    int imm; \
    if (!parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'I', &imm)) { \
        return false; \
    } \
    struct carve_instdesc* inst; \
    if (imm & ~((1 << 12) - 1)) { \
        inst = carve_getinst("lui", -1); \
        carve_prog_add(prog, carve_makeU(inst->opcode, rd, imm + (1 << 11))); \
        rs = rd; \
    } \
    inst = carve_getinst("addi", -1); \
    carve_prog_add(prog, carve_makeI(inst->opcode, inst->f3, rd, rs, imm & ((1 << 12) - 1))); \
} while (0)

#define CARVE_PSEUDO_mv() do { \
    /* mov rd, rs -> addi rd, rs, 0 */ \
    int rd, rs; \
    struct carve_instdesc* inst = carve_getinst("addi", -1); \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    if ((rs = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeI(inst->opcode, inst->f3, rd, rs, 0)); \
} while (0)

#define CARVE_PSEUDO_not() do { \
    /* not rd, rs -> xori rd, rs, -1 */ \
    int rd, rs; \
    struct carve_instdesc* inst = carve_getinst("xori", -1); \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    if ((rs = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeI(inst->opcode, inst->f3, rd, rs, -1)); \
} while (0)

#define CARVE_PSEUDO_neg() do { \
    /* neg rd, rs -> sub rd, x0, rs */ \
    int rd, rs; \
    struct carve_instdesc* inst = carve_getinst("sub", -1); \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    if ((rs = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeR(inst->opcode, inst->f3, inst->f7, rd, 0, rs)); \
} while (0)

#define CARVE_PSEUDO_seqz() do { \
    /* seqz rd, rs -> sltiu rd, rs, 1 */ \
    int rd, rs; \
    struct carve_instdesc* inst = carve_getinst("sltiu", -1); \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    if ((rs = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeI(inst->opcode, inst->f3, rd, rs, 1)); \
} while (0)

#define CARVE_PSEUDO_snez() do { \
    /* snez rd, rs -> sltu rd, x0, rs */ \
    int rd, rs; \
    struct carve_instdesc* inst = carve_getinst("sltu", -1); \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    if ((rs = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeI(inst->opcode, inst->f3, rd, 0, rs)); \
} while (0)

#define CARVE_PSEUDO_sltz() do { \
    /* sltz rd, rs -> slt rd, rs, x0 */ \
    int rd, rs; \
    struct carve_instdesc* inst = carve_getinst("slt", -1); \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    if ((rs = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeR(inst->opcode, inst->f3, inst->f7, rd, rs, 0)); \
} while (0)

#define CARVE_PSEUDO_sgtz() do { \
    /* sgtz rd, rs -> slt rd, x0, rs */ \
    int rd, rs; \
    struct carve_instdesc* inst = carve_getinst("slt", -1); \
    if ((rd = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    if (!parse_skip(prog, ntoksp, toksp, tokip, CARVE_TOK_COM)) { \
        return false; \
    } \
    if ((rs = parse_reg_int(prog, ntoksp, toksp, tokip)) == -1) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeR(inst->opcode, inst->f3, inst->f7, rd, 0, rs)); \
} while (0)

/* JUMP PSEUDOINSTRUCTIONS */

#define CARVE_PSEUDO_j() do { \
    /* j offset -> jal x0, offset */ \
    int imm; \
    if (!parse_imm(prog, ntoksp, toksp, nbackp, backp, tokip, 'J', &imm)) { \
        return false; \
    } \
    carve_prog_add(prog, carve_makeJ(carve_getinst("jal", -1)->opcode, 0, imm)); \
} while (0)

#endif /* EXT_RV32I__ */
