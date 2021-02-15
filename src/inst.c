/* inst.c - RISC-V instruction encoding
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include "carve.h"

carve_inst carve_makeR(carve_inst opcode, carve_inst f3, carve_inst f7, carve_inst rd, carve_inst rs1, carve_inst rs2) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD(f3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(rs2, 20, 25)
        | CARVE_FIELD(f7, 25, 32)
    ;
}

carve_inst carve_makeI(carve_inst opcode, carve_inst f3, carve_inst rd, carve_inst rs1, carve_inst imm) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD(f3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(imm, 20, 32)
    ;
}

carve_inst carve_makeU(carve_inst opcode, carve_inst rd, carve_inst imm) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD(imm >> 12, 12, 32)
    ;
}
carve_inst carve_makeJ(carve_inst opcode, carve_inst rd, carve_inst imm) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD(rd, 7, 12)
        | CARVE_FIELD((CARVE_MASK(12, 20) & imm) >> 12, 12, 20)
        | CARVE_FIELD((CARVE_MASK(11, 12) & imm) >> 11, 20, 21)
        | CARVE_FIELD((CARVE_MASK(1, 11) & imm) >> 1, 21, 31)
        | CARVE_FIELD((CARVE_MASK(20, 21) & imm) >> 20, 31, 32)
    ;
}
carve_inst carve_makeS(carve_inst opcode, carve_inst f3, carve_inst rs1, carve_inst rs2, carve_inst imm) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD((CARVE_MASK(0, 5) & imm), 7, 12)
        | CARVE_FIELD(f3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(rs2, 20, 25)
        | CARVE_FIELD((CARVE_MASK(5, 12) & imm) >> 5, 25, 32)
    ;
}


carve_inst carve_makeB(carve_inst opcode, carve_inst f3, carve_inst rs1, carve_inst rs2, carve_inst imm) {
    return 0
        | CARVE_FIELD(opcode, 0, 7)
        | CARVE_FIELD((CARVE_MASK(11, 12) & imm) >> 11, 7, 8)
        | CARVE_FIELD((CARVE_MASK(1, 5) & imm) >> 1, 8, 12)
        | CARVE_FIELD(f3, 12, 15)
        | CARVE_FIELD(rs1, 15, 20)
        | CARVE_FIELD(rs2, 20, 25)
        | CARVE_FIELD((CARVE_MASK(5, 11) & imm) >> 5, 25, 31)
        | CARVE_FIELD((CARVE_MASK(12, 13) & imm) >> 12, 31, 32)
    ;
}


carve_inst carve_newimmI(carve_inst inst, carve_inst imm) {
    carve_inst opcode, f3, rd, rs1, _;
    CARVE_DEC_I(inst, opcode, f3, rd, rs1, _);
    return carve_makeI(opcode, f3, rd, rs1, imm);
}
carve_inst carve_newimmU(carve_inst inst, carve_inst imm) {
    carve_inst opcode, rd, _;
    CARVE_DEC_U(inst, opcode, rd, _);
    return carve_makeU(opcode, rd, imm);
}
carve_inst carve_newimmJ(carve_inst inst, carve_inst imm) {
    carve_inst opcode, rd, _;
    CARVE_DEC_J(inst, opcode, rd, _);
    return carve_makeJ(opcode, rd, imm);
}
carve_inst carve_newimmS(carve_inst inst, carve_inst imm) {
    carve_inst opcode, f3, rs1, rs2, _;
    CARVE_DEC_S(inst, opcode, f3, rs1, rs2, _);
    return carve_makeS(opcode, f3, rs1, rs2, imm);
}
carve_inst carve_newimmB(carve_inst inst, carve_inst imm) {
    carve_inst opcode, f3, rs1, rs2, _;
    CARVE_DEC_B(inst, opcode, f3, rs1, rs2, _);
    return carve_makeB(opcode, f3, rs1, rs2, imm);
}
