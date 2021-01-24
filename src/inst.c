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

carve_inst carve_get_opcode(carve_inst inst) {
    return inst & CARVE_MASK(0, 7);
}
