/* inst.c - RISC-V instruction encoding
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"



carve_inst carve_makeR(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst funct7) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD(funct3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(rs2, 20, 25)
        | CARVE_FIELD(funct7, 25, 32)
    ;
}

carve_inst carve_makeI(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst rs1, carve_inst imm_11_0) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD(funct3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(imm_11_0, 20, 32)
    ;
}
carve_inst carve_makeU(carve_inst opcode, carve_inst rd, carve_inst imm_31_12) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD(imm_31_12, 12, 32)
    ;
}
carve_inst carve_makeS(carve_inst opcode, carve_inst imm_4_0, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm_11_5) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(imm_4_0, 7, 12)
        | CARVE_FIELD(funct3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(rs2, 20, 25)
        | CARVE_FIELD(imm_11_5, 25, 32)
    ;
}
carve_inst carve_makeB(carve_inst opcode, carve_inst _11, carve_inst imm_4_1, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm_10_5, carve_inst _12) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(_11, 7, 8)
        | CARVE_FIELD(imm_4_1, 8, 12)
        | CARVE_FIELD(funct3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(rs2, 20, 25)
        | CARVE_FIELD(imm_10_5, 25, 31)
        | CARVE_FIELD(_12, 31, 32)
    ;
}
carve_inst carve_makeJ(carve_inst opcode, carve_inst rd, carve_inst imm_19_12, carve_inst _11, carve_inst imm_10_1, carve_inst _20) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD(imm_19_12, 12, 20)
        | CARVE_FIELD(_11, 20, 21)
        | CARVE_FIELD(imm_10_1, 21, 31)
        | CARVE_FIELD(_20, 31, 32)
    ;
}

/* Easier interfaces */
carve_inst carve_makeRr(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst funct7, carve_inst rs1, carve_inst rs2) {
    return carve_makeR(opcode, rd, funct3, rs1, rs2, funct7);
}
carve_inst carve_makeIr(carve_inst opcode, carve_inst rd, carve_inst funct3, carve_inst rs1, carve_inst imm) {
    return carve_makeI(opcode, rd, funct3, rs1, imm);
}
carve_inst carve_makeUr(carve_inst opcode, carve_inst rd, carve_inst imm) {
    return carve_makeU(opcode, rd, (imm & CARVE_MASK(12, 31)) >> 12);
}
carve_inst carve_makeSr(carve_inst opcode, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm) {
    return carve_makeS(opcode, imm & CARVE_MASK(0, 4), funct3, rs1, rs2, (imm & CARVE_MASK(5, 11)) >> 5);
}
carve_inst carve_makeBr(carve_inst opcode, carve_inst funct3, carve_inst rs1, carve_inst rs2, carve_inst imm) {
    return carve_makeB(opcode, (CARVE_MASK(11, 12) & imm) >> 11, (CARVE_MASK(1, 4) & imm) >> 1, funct3, rs1, rs2, (CARVE_MASK(5, 10) & imm) >> 5, (CARVE_MASK(12, 13) & imm) >> 12);
}
carve_inst carve_makeJr(carve_inst opcode, carve_inst rd, carve_inst imm) {
    return carve_makeJ(opcode, rd, (CARVE_MASK(12, 20) & imm) >> 12, (CARVE_MASK(11, 12) & imm) >> 11, (CARVE_MASK(1, 11) & imm) >> 1, (CARVE_MASK(20, 21) & imm) >> 20);
}
carve_inst carve_get_opcode(carve_inst inst) {
    return inst & CARVE_MASK(0, 7);
}

carve_inst carve_newimmI(carve_inst inst, carve_inst imm) {
    carve_inst opcode, rd, funct3, rs1, _;
    CARVE_DEC_I(inst, opcode, rd, funct3, rs1, _);
    return carve_makeIr(opcode, rd, funct3, rs1, imm);
}
carve_inst carve_newimmU(carve_inst inst, carve_inst imm) {
    carve_inst opcode, rd, _;
    CARVE_DEC_U(inst, opcode, rd, _);
    return carve_makeUr(opcode, rd, imm);
}
carve_inst carve_newimmJ(carve_inst inst, carve_inst imm) {
    carve_inst opcode, rd, _;
    CARVE_DEC_J(inst, opcode, rd, _, _, _, _);
    return carve_makeJr(opcode, rd, imm);
}
carve_inst carve_newimmS(carve_inst inst, carve_inst imm) {
    carve_inst opcode, funct3, rs1, rs2, _;
    CARVE_DEC_S(inst, opcode, _, funct3, rs1, rs2, _);
    return carve_makeSr(opcode, funct3, rs1, rs2, imm);
}
carve_inst carve_newimmB(carve_inst inst, carve_inst imm) {
    carve_inst opcode, funct3, rs1, rs2, _;
    CARVE_DEC_B(inst, opcode, _, _, funct3, rs1, rs2, _, _);
    return carve_makeBr(opcode, funct3, rs1, rs2, imm);
}
