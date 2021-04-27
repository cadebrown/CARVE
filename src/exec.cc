/* exec.cc - GENERATED - Execution implementation
 *  
 * ** THIS IS A GENERATED FILE, DO NOT EDIT **
 *
 * @author: Cade Brown <cade@utk.edu>
 */

#include <carve.hh>

namespace carve {



void State::exec(inst v) {
    u64 op, f3, f7, imm;
    int rd, rs1, rs2;
    // Decode opcode
    switch (op = v & 0x7F) {
        case 0x37:
            dec_U(v, op, rd, imm);
            impl::_lui(*this, rd, imm);
            break;
        case 0x17:
            dec_U(v, op, rd, imm);
            impl::_auipc(*this, rd, imm);
            break;
        case 0x6f:
            dec_J(v, op, rd, imm);
            impl::_jal(*this, rd, imm);
            break;
        case 0x67:
            dec_I(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    impl::_jalr(*this, rd, rs1, imm);
                    break;
            }
            break;
        case 0x63:
            dec_B(v, op, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                    impl::_beq(*this, rs1, rs2, imm);
                    break;
                case 0x1:
                    impl::_bne(*this, rs1, rs2, imm);
                    break;
                case 0x4:
                    impl::_blt(*this, rs1, rs2, imm);
                    break;
                case 0x5:
                    impl::_bge(*this, rs1, rs2, imm);
                    break;
                case 0x6:
                    impl::_bltu(*this, rs1, rs2, imm);
                    break;
                case 0x7:
                    impl::_bgeu(*this, rs1, rs2, imm);
                    break;
            }
            break;
        case 0x3:
            dec_I(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    impl::_lb(*this, rd, rs1, imm);
                    break;
                case 0x1:
                    impl::_lh(*this, rd, rs1, imm);
                    break;
                case 0x2:
                    impl::_lw(*this, rd, rs1, imm);
                    break;
                case 0x4:
                    impl::_lbu(*this, rd, rs1, imm);
                    break;
                case 0x5:
                    impl::_lhu(*this, rd, rs1, imm);
                    break;
            }
            break;
        case 0x23:
            dec_S(v, op, f3, rs1, rs2, imm);
            switch (f3) {
                case 0x0:
                    impl::_sb(*this, rs1, rs2, imm);
                    break;
                case 0x1:
                    impl::_sh(*this, rs1, rs2, imm);
                    break;
                case 0x2:
                    impl::_sw(*this, rs1, rs2, imm);
                    break;
            }
            break;
        case 0x13:
            dec_I(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    impl::_addi(*this, rd, rs1, imm);
                    break;
                case 0x2:
                    impl::_slti(*this, rd, rs1, imm);
                    break;
                case 0x3:
                    impl::_sltiu(*this, rd, rs1, imm);
                    break;
                case 0x4:
                    impl::_xori(*this, rd, rs1, imm);
                    break;
                case 0x6:
                    impl::_ori(*this, rd, rs1, imm);
                    break;
                case 0x7:
                    impl::_andi(*this, rd, rs1, imm);
                    break;
                case 0x1:
                    impl::_slli(*this, rd, rs1, imm);
                    break;
                case 0x5:
                    impl::_srli(*this, rd, rs1, imm);
                    break;
            }
            break;
        case 0x33:
            dec_R(v, op, f3, f7, rd, rs1, rs2);
            switch (f3) {
                case 0x0:
                    switch (f7) {
                        case 0x0:
                            impl::_add(*this, rd, rs1, rs2);
                            break;
                        case 0x20:
                            impl::_sub(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x1:
                    switch (f7) {
                        case 0x0:
                            impl::_sll(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x2:
                    switch (f7) {
                        case 0x0:
                            impl::_slt(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x3:
                    switch (f7) {
                        case 0x0:
                            impl::_sltu(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x4:
                    switch (f7) {
                        case 0x0:
                            impl::_xor(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x5:
                    switch (f7) {
                        case 0x0:
                            impl::_srl(*this, rd, rs1, rs2);
                            break;
                        case 0x10:
                            impl::_sra(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x6:
                    switch (f7) {
                        case 0x0:
                            impl::_or(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x7:
                    switch (f7) {
                        case 0x0:
                            impl::_and(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
            }
            break;
        case 0x73:
            dec_y(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    impl::_ebreak(*this, rd, rs1, imm);
                    break;
            }
            break;
        case 0x1b:
            dec_I(v, op, f3, rd, rs1, imm);
            switch (f3) {
                case 0x0:
                    impl::_addiw(*this, rd, rs1, imm);
                    break;
                case 0x1:
                    impl::_slliw(*this, rd, rs1, imm);
                    break;
                case 0x5:
                    impl::_srliw(*this, rd, rs1, imm);
                    break;
            }
            break;
        case 0x3b:
            dec_R(v, op, f3, f7, rd, rs1, rs2);
            switch (f3) {
                case 0x0:
                    switch (f7) {
                        case 0x0:
                            impl::_addw(*this, rd, rs1, rs2);
                            break;
                        case 0x20:
                            impl::_subw(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x1:
                    switch (f7) {
                        case 0x0:
                            impl::_sllw(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x2:
                    switch (f7) {
                        case 0x0:
                            impl::_srlw(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
                case 0x5:
                    switch (f7) {
                        case 0x10:
                            impl::_sraw(*this, rd, rs1, rs2);
                            break;
                    }
                    break;
            }
            break;

    }
}


}

