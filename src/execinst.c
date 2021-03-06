/* execinst.c - Generated file that defines executing RISC-V instructions
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT ** 
 */

#include "ext/common.h"


#ifdef RV32I
  #include "ext/RV32I.h"
#endif /* RV32I */

void carve_execinst(carve_state s, carve_inst inst) {
    carve_inst opcode, f3, f7, rd, rs1, rs2, imm;    
    switch (opcode = inst & 0x7F) {
        case 0x37:
            /* lui */
          #ifdef RV32I
            CARVE_lui(rd, imm);
          #else
             HALT("unsupported instruction 'lui' (extension RV32I not supported)"); 
          #endif /* RV32I */
            break;
        case 0x17:
            /* auipc */
          #ifdef RV32I
            CARVE_auipc(rd, imm);
          #else
             HALT("unsupported instruction 'auipc' (extension RV32I not supported)"); 
          #endif /* RV32I */
            break;
        case 0x6f:
            /* jal */
          #ifdef RV32I
            CARVE_jal(rd, imm);
          #else
             HALT("unsupported instruction 'jal' (extension RV32I not supported)"); 
          #endif /* RV32I */
            break;
        case 0x67:
            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                  #ifdef RV32I
                    /* jalr */
                    CARVE_jalr(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'jalr' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
            }
            break;
        case 0x63:
            CARVE_DEC_B(inst, opcode, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                  #ifdef RV32I
                    /* beq */
                    CARVE_beq(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'beq' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x1:
                  #ifdef RV32I
                    /* bne */
                    CARVE_bne(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'bne' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x4:
                  #ifdef RV32I
                    /* blt */
                    CARVE_blt(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'blt' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x5:
                  #ifdef RV32I
                    /* bge */
                    CARVE_bge(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'bge' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x6:
                  #ifdef RV32I
                    /* bltu */
                    CARVE_bltu(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'bltu' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x7:
                  #ifdef RV32I
                    /* bgeu */
                    CARVE_bgeu(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'bgeu' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
            }
            break;
        case 0x3:
            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                  #ifdef RV32I
                    /* lb */
                    CARVE_lb(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'lb' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x1:
                  #ifdef RV32I
                    /* lh */
                    CARVE_lh(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'lh' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x2:
                  #ifdef RV32I
                    /* lw */
                    CARVE_lw(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'lw' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x4:
                  #ifdef RV32I
                    /* lbu */
                    CARVE_lbu(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'lbu' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x5:
                  #ifdef RV32I
                    /* lhu */
                    CARVE_lhu(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'lhu' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
            }
            break;
        case 0x23:
            CARVE_DEC_S(inst, opcode, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                  #ifdef RV32I
                    /* sb */
                    CARVE_sb(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'sb' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x1:
                  #ifdef RV32I
                    /* sh */
                    CARVE_sh(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'sh' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x2:
                  #ifdef RV32I
                    /* sw */
                    CARVE_sw(rs1, rs2, imm);
                  #else
                    HALT("unsupported instruction 'sw' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
            }
            break;
        case 0x13:
            CARVE_DEC_I(inst, opcode, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                  #ifdef RV32I
                    /* addi */
                    CARVE_addi(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'addi' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x2:
                  #ifdef RV32I
                    /* slti */
                    CARVE_slti(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'slti' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x3:
                  #ifdef RV32I
                    /* sltiu */
                    CARVE_sltiu(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'sltiu' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x4:
                  #ifdef RV32I
                    /* xori */
                    CARVE_xori(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'xori' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x6:
                  #ifdef RV32I
                    /* ori */
                    CARVE_ori(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'ori' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x7:
                  #ifdef RV32I
                    /* andi */
                    CARVE_andi(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'andi' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x1:
                  #ifdef RV32I
                    /* slli */
                    CARVE_slli(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'slli' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
                case 0x5:
                  #ifdef RV32I
                    /* srli */
                    CARVE_srli(rd, rs1, imm);
                  #else
                    HALT("unsupported instruction 'srli' (extension RV32I not supported)"); 
                  #endif /* RV32I */
                    break;
            }
            break;
        case 0x33:
            CARVE_DEC_R(inst, opcode, f3, f7, rd, rs1, rs2);
            switch (f3) {
                case 0x0:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* add */
                            CARVE_add(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'add' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                        case 0x20:
                          #ifdef RV32I
                            /* sub */
                            CARVE_sub(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'sub' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
                case 0x1:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* sll */
                            CARVE_sll(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'sll' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
                case 0x2:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* slt */
                            CARVE_slt(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'slt' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
                case 0x3:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* sltu */
                            CARVE_sltu(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'sltu' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
                case 0x4:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* xor */
                            CARVE_xor(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'xor' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
                case 0x5:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* srl */
                            CARVE_srl(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'srl' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                        case 0x10:
                          #ifdef RV32I
                            /* sra */
                            CARVE_sra(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'sra' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
                case 0x6:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* or */
                            CARVE_or(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'or' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
                case 0x7:
                    switch (f7) {
                        case 0x0:
                          #ifdef RV32I
                            /* and */
                            CARVE_and(rd, rs1, rs2);
                          #else
                            HALT("unsupported instruction 'and' (extension RV32I not supported)"); 
                          #endif /* RV32I */
                            break;
                    }
                    break;
            }
            break;
        case 0x0:
            switch (f3) {
            }
            break;
    }
}
